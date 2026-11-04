#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32

#include <winsock2.h>
typedef struct {
	SOCKET descriptor;
	int port;
	struct sockaddr_in address;
} Socket;

#else

#include <netinet/in.h>
typedef struct {
	int descriptor;
	int port;
	struct sockaddr_in address;
} Socket;

#endif

void initialize_socket();
void create_socket(Socket *);
void bind_socket(Socket *, int);
void listen_for_connections(Socket *);
Socket accept_connection(Socket *);
void destroy_socket(Socket *);

int recieve(Socket *sock, char *);
int respond(Socket *sock, const char *);

#endif
