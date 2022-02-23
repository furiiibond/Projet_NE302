#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "api.h"

typedef struct node {
    char *tag;
    char *value; // Value of the field
    int len;  // number of char
    struct node *child; // point on a child node.
    struct node *brother; // point on a brother node
} node;



node *generateNodes(char* value ,int len , char nextDelim[]){
    node *node= malloc(sizeof(node));
    //node->tag     calculated
    node->len = len;
    node->value = value;

    return node;
}

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

