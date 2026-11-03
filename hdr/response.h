#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
    float version;
    int status_code;
    char *status_phrase;
    char *body;
} Response;

void initialize_response(Response *);
const char *get_response_string(Response *);
void set_response_body(Response *, const char *);
void destroy_response(Response *);

#endif
