#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32

#include <winsock2.h>
typedef struct {
	SOCKET descriptor;
	int port;
	struct sockaddr_in address;
} socket_t;

#else

#include <netinet/in.h>
typedef struct {
	int descriptor;
	int port;
	struct sockaddr_in address;
} socket_t;

#endif

void initialize_socket();
void create_socket(socket_t *);
void bind_socket(socket_t *, int);
void listen_for_connections(socket_t *);
socket_t accept_connection(socket_t *);
void destroy_socket(socket_t *);
void clean_up_sockets();

int recieve(socket_t *sock, char *);
int respond(socket_t *sock, const char *);

#endif
