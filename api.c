#include <stdio.h>
#include <string.h>
#include "api.h"

int parseur(char *req, int len)
{
    char delim[] = "\r\n";

    char *ptr = strtok(req, delim);

    while(ptr != NULL)
    {
        printf("'%s'\n", ptr);
        ptr = strtok(NULL, delim);
    }
    return 0; // TODO
}

