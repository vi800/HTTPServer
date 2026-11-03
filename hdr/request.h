#ifndef REQUEST_H
#define REQUEST_H

typedef struct {
    char *method;
    char *uri;
} Request;

void set_request_method(Request *, const char *);
void set_request_uri(Request *, const char *);
void parse_request(Request *, char[]);
void destroy_request(Request *);

#endif
