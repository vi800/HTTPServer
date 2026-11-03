#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "request.h"
#include "response.h"
#include "constants.h"


int handle_client(int client_socket)
{
    char buffer[8 KB];
    int recieved_bytes = 0;
    int n = 0;
    char *response_string;
    Request request;
    Response response;
    char file_path[64];

    memset(file_path, 0, sizeof(file_path));
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

    parse_request(&request, buffer);
    initialize_response(&response);
    strcat(file_path, FILE_DIR);
    strcat(file_path, "/index.html");
    set_response_body(&response, file_path);
    response_string = get_response_string(&response);

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

//    while (true)
//    {
        printf("waiting for connection\n\n");
        client_socket = accept(tcp_socket, NULL, NULL);

        if (handle_client(client_socket) == -1)
        {
            return_value = 1;
            goto exit;
        }
//    }
    
exit:
    close(tcp_socket);
    return return_value;
}
