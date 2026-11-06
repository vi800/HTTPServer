#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PORT 1337
#define KB *1024
#define RECV_MAX_LENGTH 1 KB
#define HTTP_VERSION 1.1
#define FILE_DIR "tst"

typedef enum {
	GET,
	POST,
	PUT,
	PATCH
} http_method;

#endif
