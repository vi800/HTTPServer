#include <iostream>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define PORT 1337

int handle_client(int client_socket)
{
    int recieved_bytes;
    char buffer[128];
    
    memset(buffer, 0, sizeof(buffer));

    while (true)
    {
        recieved_bytes = recv(client_socket, buffer, sizeof(buffer), MSG_WAITALL);
        
        if (recieved_bytes == -1)
        {
            std::cout << "recv error\n";
            return -1;
        }

        if (recieved_bytes == 0)
        {
            return 0;
        }

        std::cout << buffer;
    }
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
        std::cout << "socket error\n";
        return 1;
    }

    (void) setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, (void *) reuse_address, sizeof(reuse_address)); 

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(tcp_socket, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == -1) {
        std::cout << "bind error\n";
        return_value = 1;
        goto exit;
    }

    if (client_socket = listen(tcp_socket, SOMAXCONN) == -1) {
        std::cout << "listen\n";
        return_value = 1;
        goto exit;
    }

    while (true)
    {
        std::cout << "waiting for connection...\n";
        client_socket = accept(tcp_socket, NULL, NULL);

        std::cout << "connected!\n";
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
