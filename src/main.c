#include "request.h"
#include "response.h"
#include "constants.h"
#include "socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int handle_client(Socket *client_socket)
{
	char buffer[8 KB];
    int recieved_bytes = 0;
    char *response_string;
    char file_path[64];
    Request request;
    Response response;

    memset(file_path, 0, sizeof(file_path));

    recieved_bytes = recieve(client_socket, buffer);
    printf("Request: \n%s\n", buffer);

    parse_request(&request, buffer);
    initialize_response(&response);
    strcat(file_path, FILE_DIR);
    strcat(file_path, "/index.html");
    set_response_body(&response, file_path);
    response_string = get_response_string(&response);

    (void) respond(client_socket, response_string);
    destroy_socket(client_socket);
    return 0;
}

int main()
{
    Socket tcp_socket;
    Socket client_socket;
    int return_value = 0;

    bool reuse_address = true;

    create_socket(&tcp_socket);
    bind_socket(&tcp_socket, PORT);
    listen_for_connections(&tcp_socket);

//    while (true)
//    {
//    }
    printf("waiting for connection...\n\n");
    client_socket = accept_connection(&tcp_socket);

    if (handle_client(&client_socket) == -1)
    {
    	destroy_socket(&client_socket);
     	printf("client socket error\n");
    }
   	destroy_socket(&tcp_socket);
}
