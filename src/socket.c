#include "socket.h"
#include "constants.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
// WINDOWS

#include <ws2tcpip.h>

void initialize_socket()
{
	WSADATA wsa_data;

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data)) {
		printf("windows startup error\n");
		exit(1);
	}
}

void create_socket(socket_t *sock)
{
	sock->descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (sock->descriptor == INVALID_SOCKET) {
		printf("socket creation error\n");
		exit(1);
	}
}

socket_t accept_connection(socket_t *sock)
{
	socket_t client_socket;
	client_socket.descriptor = accept(sock->descriptor, NULL, NULL);

	if (client_socket.descriptor == INVALID_SOCKET) {
		printf("accept error\n");
		printf("%d\n", WSAGetLastError());
	}

	return client_socket;
}

void destroy_socket(socket_t *sock)
{
	closesocket(sock->descriptor);
}

void clean_up_sockets()
{
	WSACleanup();
}

#else
// POSIX

#include <sys/socket.h>
#include <unistd.h>

void initialize_socket()
{}

void create_socket(socket_t *sock)
{
	sock->descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (sock->descriptor == -1) {
		printf("socket_creation_error\n");
		exit(1);
	}
}

socket_t accept_connection(socket_t *sock)
{
	socket_t client_socket;
	client_socket.descriptor = accept(sock->descriptor, NULL, NULL);

	if (client_socket.descriptor == -1) {
		printf("accept error\n");
		destroy_socket(sock);
	}

	return client_socket;
}

void destroy_socket(socket_t *sock)
{
	close(sock->descriptor);
}

void clean_up_sockets()
{}

#endif
// COMMON

void bind_socket(socket_t *sock, int port)
{
// FOR TESTING ONLY. REMOVE WHEN POSSIBLE
bool reuse_address = true;
(void) setsockopt(sock->descriptor, SOL_SOCKET, SO_REUSEADDR, (void *) &reuse_address, sizeof(reuse_address));

	memset(&sock->address, 0, sizeof(struct sockaddr_in));
	sock->address.sin_family = AF_INET;
	sock->address.sin_port = htons(port);
	sock->address.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock->descriptor, (struct sockaddr *) &sock->address, sizeof(struct sockaddr_in)) == -1) {
        printf("bind error\n");
        destroy_socket(sock);
	}
}

void listen_for_connections(socket_t *sock)
{
	if (listen(sock->descriptor, SOMAXCONN) == -1) {
		printf("listen error\n");
		destroy_socket(sock);
	}
}

// IT WORKS ONLY FOR THIS SITUATION AND WILL FAIL IN A LOT OF CASES BECAUSE SIZE OF BUFFER IS NOT CONSIDERED
int recieve(socket_t *sock, char *buffer)
{
	int recieved_bytes = 0;
	int n = 0;

	while (true)
    {
        n = recv(sock->descriptor, buffer + recieved_bytes, RECV_MAX_LENGTH, 0);
        recieved_bytes += n;

        if (n == -1) {
            printf("recv error\n");
            return -1;
        }
        if (n == 0) {
            printf("connection closed\n");
            break;
        }
        if (n < RECV_MAX_LENGTH) {
            break;
        }
        if (recieved_bytes >= 8 KB) {
        	printf("damm son\n");
         	break;
        }
    }
    buffer[recieved_bytes] = 0;

    return recieved_bytes;
}

int respond(socket_t *sock, const char *string)
{
	return send(sock->descriptor, string, strlen(string), 0);
}
