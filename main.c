#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define PORT 1337

int handle_client(int client_socket)
{
    int recieved_bytes = 0;
    char buffer[1024];
    const char *response = "HTTP/1.1 200 OK\r\n\r\nhello";

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        recieved_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (recieved_bytes == -1) {
            printf("recv error\n");
            return -1;
        }
        if (recieved_bytes == 0) {
            printf("connection closed\n");
            break;
        }

        printf("%s", buffer);

        if (recieved_bytes < sizeof(buffer) - 1) {
            break;
        }
        printf("recieved %d bytes\n", recieved_bytes);
    }
    printf("\n");

    (void) send(client_socket, response, strlen(response), 0);
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

    (void) setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, (void *) reuse_address, sizeof(reuse_address)); 

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(tcp_socket, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == -1) {
        printf("bind error\n");
        return_value = 1;
        goto exit;
    }

    if (client_socket = listen(tcp_socket, SOMAXCONN) == -1) {
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
