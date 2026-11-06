#include "router.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

bool file_exists(const char *file_path)
{
	FILE *f = fopen(file_path, "r");

	if (f != NULL) {
		fclose(f);
		return true;
	}
	return false;
}

void get_file_path(char *file_path, const char *uri)
{
	strcat(file_path, ROOT_DIR);
	strcat(file_path, uri);

    if (strcmp(uri, "/") == 0) {
    	strcat(file_path, "index.html");
     	return;
    }

    if (strchr(uri, '.') == NULL)
     	strcat(file_path, ".html");

    if (!file_exists(file_path)) {
    	strcpy(file_path, ROOT_DIR);
     	strcat(file_path, NOT_FOUND_FILE);
     	return;
    }
}
