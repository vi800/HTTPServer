#include "request.h"
#include "response.h"
#include "constants.h"
#include "socket.h"
#include "router.h"
#include "thread_pool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <threads.h>

void *handle_client(void *client_socket)
{
	char buffer[8 KB];
    int recieved_bytes = 0;
    char *response_string;
    char file_path[64];
    request_t request;
    response_t response;

    memset(file_path, 0, sizeof(file_path));

    recieved_bytes = recieve(client_socket, buffer);
    parse_request(&request, buffer);

    get_file_path(file_path, request.uri);

    initialize_response(&response);
    set_response_body(&response, file_path);
    response_string = get_response_string(&response);

    (void) respond(client_socket, response_string);
    destroy_socket(client_socket);

    return NULL;
}

int main()
{
    socket_t tcp_socket;
    socket_t client_socket;

    thread_pool_t thread_pool;

    initialize_socket();

    create_socket(&tcp_socket);
    bind_socket(&tcp_socket, PORT);
    listen_for_connections(&tcp_socket);

    initialize_thread_pool(&thread_pool);

    int delete_this = 3;
    while (delete_this--)
    {
        printf("waiting for connection...\n\n");
        client_socket = accept_connection(&tcp_socket);

        handle_client(&client_socket);

        destroy_socket(&client_socket);
    }
    destroy_socket(&tcp_socket);

    clean_up_sockets();
}
