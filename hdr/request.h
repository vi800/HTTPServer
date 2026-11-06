#ifndef REQUEST_H
#define REQUEST_H

typedef struct {
    char *method;
    char *uri;
} request_t;

void set_request_method(request_t *, const char *);
void set_request_uri(request_t *, const char *);
void parse_request(request_t *, char[]);
void destroy_request(request_t *);

#endif
