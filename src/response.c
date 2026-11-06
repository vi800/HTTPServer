#include "response.h"
#include "constants.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *get_status_phrase(int status_code)
{
    switch (status_code)
    {
        case 200:
            return "OK";
        default:
            return "ERROR";
    }
}

void initialize_response(response_t *response)
{
    char *status_phrase = get_status_phrase(200);

    response->version = HTTP_VERSION;
    response->status_code = 200;
    response->status_phrase = (char *) malloc(sizeof(char) * strlen(status_phrase));
    strcpy(response->status_phrase, status_phrase);
}

void destroy_response(response_t *response)
{
    free(response->status_phrase);
    free(response->body);
}

void set_response_body(response_t *response, const char *file_path)
{
    int file_size;
    FILE *f = fopen(file_path, "rb");

    if (f == NULL) {
        printf("could not open file\n");
        return;
    }

    file_size = ftell(f);
    fseek(f, 0, SEEK_END);
    file_size = ftell(f) - file_size;
    fseek(f, 0, SEEK_SET);

    response->body = (char *) malloc(sizeof(char) * (file_size + 1));
    if (fread(response->body, 1, file_size, f) != file_size) {
        printf("error reading file\n");
    }
    response->body[file_size] = 0;

    fclose(f);
}

const char *get_response_string(response_t *response)
{
    char buffer[8 KB];
    const char *pointer;

    memset(buffer, 0, sizeof(buffer));

    snprintf(buffer, sizeof(buffer), "HTTP/%.1f %d %s\r\n\r\n%s", response->version, response->status_code, response->status_phrase, response->body);
    pointer = buffer;
    return pointer;
}
