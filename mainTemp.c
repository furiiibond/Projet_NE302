#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "api.h"


typedef struct node {
    char* tag;
    char* value; // pointer to the first char of the value
    int len_value;  // number of char starting from pointer value
    struct node *child; // pointer to the first child or NULL
    struct node *brother; // pointer to the next brother node or NULL
} node;

node *addNodeAsBrother(char* tag, char* value, int len_value, node* current);
node *addNodeAsChild(char* tag, char* value, int len_value, node* current);
void createRoot(char* tag, char* value, int len_value);
char *getElementValue(void *node, int *len);
void display_tree(node* noeud, int n);

int main() {
    char req[]="GET / HTTP/1.0\r\nHost: www.google.com\r\nTransfer-Encoding: gzip\r\n\r\n";
    //parseur(req,strlen(req));
    //char req[] = "starttestbdbdyb";
    //parseur(req, strlen(req));
	int len = strlen(req);
    createRoot("HTTP_message", req, len);
    node *node1 = addNodeAsChild("start_line", "GET / HTTP/1.0\r\n", 16, getRootTree());    
    addNodeAsBrother("header_field", "Host: www.google.com\r\n", 22, node1);
	addNodeAsChild("header_field", "Host: www.go\r\n", 14, node1);
    display_tree(getRootTree(), 0);
    _Token *r, *r2;
    r = searchTree(getRootTree(), "header_field");
    r2 = r;
	int a;
	char *toprint;
    puts("Matching the search:");
    while (r) {
		toprint = getElementValue(r->node, &a);
        write(STDOUT_FILENO, toprint, a);
		r = r->next;
    }
    purgeTree(getRootTree());
    purgeElement(&r2);
    return 0;
}
