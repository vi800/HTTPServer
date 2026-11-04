#ifndef ROUTER_H
#define ROUTER_H

#define INITIAL_ROUTER_CAPACITY 8

typedef struct {
    char *uri;
    char *path;
} Route;

typedef struct {
    Route *routes;
    int size;
    int capacity;
} Router;

void initialize_router(Router *);
void add_route(Router *, const char *, const char *);
void remove_route(Router *, const char *);
const char *get_route_path(Router *, const char *);
void set_route(Route *, const char *, const char *);
void destroy_route(Route *);
void destroy_router(Router *);

#endif
