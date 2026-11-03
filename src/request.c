#include "request.h"
#include "constants.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void set_request_method(Request *request, const char *method)
{
    int method_length = strlen(method) + 1;
    request->method = (char *) malloc(sizeof(char) * method_length);
    strcpy(request->method, method);
}

void set_request_uri(Request *request, const char *uri)
{
    int uri_length = strlen(uri) + 1;
    request->uri = (char *) malloc(sizeof(char) * uri_length);
    strcpy(request->uri, uri);
}

void parse_request(Request *request, char buffer[])
{
    char *delimiters = " \r\n";
    char *token = strtok(buffer, delimiters);

    set_request_method(request, token);
    token = strtok(NULL, delimiters);
    set_request_uri(request, token);
}

void destroy_request(Request *request)
{
    free(request->method);
    free(request->uri);
}


