#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
    float version;
    int status_code;
    char *status_phrase;
    char *body;
} response_t;

void initialize_response(response_t *);
const char *get_response_string(response_t *);
void set_response_body(response_t *, const char *);
void destroy_response(response_t *);

#endif
