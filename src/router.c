#include "router.h"

#include <stdlib.h>
#include <string.h>

void initialize_router(Router *router)
{
    router->routes = (Route *) malloc(sizeof(Route) * INITIAL_ROUTER_CAPACITY);
    router->size = 0;
    router->capacity = INITIAL_ROUTER_CAPACITY;
}

void add_route(Router *router, const char *uri, const char *path)
{
    if (router->size >= router->capacity) {
        router->capacity = 2 * router->capacity;
        router->routes = (Route *) realloc(router->routes, sizeof(Route) * router->capacity);
    }

    set_route(&router->routes[router->size++], uri, path);
}

void remove_route(Router *router, const char *uri)
{
    Route route;
    for (int i = 0; i < router->size; ++i)
    {
        route = router->routes[i];
        if (strcmp(route.uri, uri) == 0) {
            destroy_route(&router->routes[i]);
            for (i = i+1; i < router->size; ++i)
                router->routes[i-1] = router->routes[i];
        }
    }
    --router->size;
}

const char *get_route_path(Router *router, const char *uri)
{
    Route route;
    for (int i = 0; i < router->size; ++i)
    {
        route = router->routes[i];
        if (strcmp(route.uri, uri) == 0) {
            return route.path;
        }
    }
}

void set_route(Route *route, const char *uri, const char *path)
{
    route->uri = (char *) malloc(sizeof(char) * strlen(uri));
    route->path = (char *) malloc(sizeof(char) * strlen(path));
}

void destroy_route(Route *route)
{
    free(route->uri);
    free(route->path);
}

void destroy_router(Router *router)
{
    for (int i = 0; i < router->size; ++i)
        destroy_route(&router->routes[i]);
    free(router->routes);
    router->size = 0;
    router->capacity = 0;
}
