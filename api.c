#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "abnf.h"

typedef struct node {
    char* tag;
    char* value; // pointer to the first char of the value
    int len;  // number of char starting from pointer value
    struct node *child; // pointer to the first child or NULL
    struct node *brother; // pointer to the next brother node or NULL
} node;

node *generateNodes(char* value, int len, char nextDelim[]) {
    node *node = malloc(sizeof(node));
    if (node == NULL) {
        perror("malloc failed");
        exit(1);
    }
    //node->tag     calculated
    node->len = len;
    node->value = value;

    return node;
}

int parseur(char* req, int len) {
   /* char delim[] = "\r\n";
    char *ptr = strtok(req, delim);

    while (ptr != NULL) {
        printf("'%s'\n", ptr);
        ptr = strtok(NULL, delim);
    }
    return 0; // TODO*/
    int leng = httpMessage(req, len);
    req += leng;
    printf("%s", req);
    return leng;
}