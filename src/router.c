#include "router.h"

#include <string.h>

void get_file_path(char *file_path, const char *uri)
{
	strcat(file_path, uri);

    if (strchr(uri, '.') == NULL)
     	strcat(file_path, ".html");
}
