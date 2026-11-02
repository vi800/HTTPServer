#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define PORT 1337
#define KB *1024
#define RECV_MAX_LENGTH 1 KB
#define HTTP_VERSION 1.1

enum {
    GET,
    POST,
    PUT
};

typedef struct {
    int method;
    char *uri;
} Request;

typedef struct {
    float version;
    int status_code;
    char *status_phrase;
} Response;

const char *get_http_response_string(Response *response)
{
    char buffer[8 KB];
    const char *pointer;

    memset(buffer, sizeof(buffer), 0);

    snprintf(buffer, sizeof(buffer), "HTTP/%.1f %d %s\r\n\r\nhello nigga", response->version, response->status_code, response->status_phrase);
    pointer = buffer;
    return pointer;
}

void set_http_request_method(Request *request, char *method)
{
    if (strcmp(method, "GET") == 0) {
        request->method = GET;
    } else if (strcmp(method, "POST") == 0) {
        request->method = POST;
    } else if (strcmp(method, "PUT") == 0) {
        request->method = PUT;
    }
}

const char *get_http_request_method_string(Request *request)
{
    switch (request->method)
    {
        case GET:
            return "GET";
        case POST:
            return "POST";
        case PUT:
            return "PUT";
    }
}

void set_http_request_uri(Request *request, char *uri)
{
    int uri_length = strlen(uri);
    request->uri = (char *) malloc(sizeof(char) * uri_length);
}

Request parse_http_request(char buffer[8 KB])
{
    Request request;

    char *delimiters = " \r\n";
    char *token = strtok(buffer, delimiters);

    set_http_request_method(&request, token);
    token = strtok(NULL, delimiters);
    set_http_request_uri(&request, token);

    return request;
}

const char *get_status_phrase(int status_code)
{
    switch (status_code)
    {
        case 200:
            return "OK";
    }
}

void initialize_http_response(Response *response)
{
    char *status_phrase = get_status_phrase(200);

    response->version = HTTP_VERSION;
    response->status_code = 200;
    response->status_phrase = (char *) malloc(sizeof(char) * strlen(status_phrase));
    strcpy(response->status_phrase, status_phrase);
}

void destroy_request(Request *request)
{
    free(request->uri);
}

void destroy_response(Response *response)
{
    free(response->status_phrase);
}

int handle_client(int client_socket)
{
    char buffer[8 KB];
    int recieved_bytes = 0;
    int n = 0;
    char *response_string;
    Response response;

    while (true)
    {
        n = recv(client_socket, buffer + recieved_bytes, RECV_MAX_LENGTH, 0);
        recieved_bytes += n;

        if (n == -1) {
            printf("recv error\n");
            return -1;
        }
        if (n == 0) {
            printf("connection closed\n");
            break;
        }

        printf("%s", buffer);
        printf("recieved %d bytes\n", recieved_bytes);

        if (recieved_bytes < RECV_MAX_LENGTH) {
            break;
        }
    }
    printf("\n");

    Request request = parse_http_request(buffer);
    initialize_http_response(&response);
    response_string = get_http_response_string(&response);

    (void) send(client_socket, response_string, strlen(response_string), 0);
    close(client_socket);
    return 0;
}

int main()
{
    struct sockaddr_in address;
    int tcp_socket = 0; 
    int client_socket = 0;
    int return_value = 0;

    bool reuse_address = true;

    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket == -1) {
        printf("socket error\n");
        return 1;
    }

    (void) setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, (void *) &reuse_address, sizeof(reuse_address)); 

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(tcp_socket, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == -1) {
        printf("bind error\n");
        return_value = 1;
        goto exit;
    }

    if (listen(tcp_socket, SOMAXCONN) == -1) {
        printf("listen error\n");
        return_value = 1;
        goto exit;
    }

    while (true)
    {
        printf("waiting for connection\n\n");
        client_socket = accept(tcp_socket, NULL, NULL);

        if (handle_client(client_socket) == -1)
        {
            return_value = 1;
            goto exit;
        }
    }
    
exit:
    close(tcp_socket);
    return return_value;
}
