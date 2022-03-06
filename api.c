#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "abnf.h"

typedef struct node {
    char* tag;
    char* value; // pointer to the first char of the value
    int len_value;  // number of char starting from pointer value
    struct node *child; // pointer to the first child or NULL
    struct node *brother; // pointer to the next brother node or NULL
} node;

node* root = NULL; // global

/*
 *      Allocates and return a new node for the http tree after initializing it
 */
node *generateNode(char* tag, char* value, int len_value) {
    struct node *node = malloc(sizeof(struct node));
    if (node == NULL) {
        perror("generateNode: malloc failed");
        exit(1);
    }
    node->tag = tag;
    node->value = value;
    node->len_value = len_value;
    node->child = NULL;
    node->brother = NULL;
    return node;
}

/*
 *      Initalize the global root of the http tree
 */
void createRoot(char* tag, char* value, int len_value) {
    root = generateNode(tag, value, len_value);
}

/*
 *      Generate a new node and adds it to the current node as a brother
 */
node *addNodeAsBrother(char* tag, char* value, int len_value, node* current) {
    if (current != NULL) { // check if the current node as been malloc
        node *node = generateNode(tag, value, len_value);
        current->brother = node;
    }
    else {
        perror("addNodeAsBrother: current node was NULL");
        exit(1);
    }
    return current->brother;
}

/*
 *      Generate a new node and adds it to the current node as a child
 */
node *addNodeAsChild(char* tag, char* value, int len_value, node* current) {
    if (current != NULL) { // check if the current node as been malloc
        node *node = generateNode(tag, value, len_value);
        current->child = node;
    }
    else {
        perror("addNodeAsChild: current node was NULL");
        exit(1);
    }
    return current->child;
}

void *getRootTree() {
    return root;
}

/*
 *      Allocates and return a new Token
 */
_Token *generateToken(node* nod) {
    _Token *tok = malloc(sizeof(_Token));
    if (tok == NULL) {
        perror("generateToken: malloc failed");
        exit(1);
    }
    tok->node = nod;
    tok->next = NULL;
    return tok;
}

/*
 *      Traversal of the Tree and adding a new node to the linked list tok when matching name
 */
void _searchTree(node *start, char *name, _Token *tok) {
        if (start == NULL)
            return;
		
        if (strcmp(start->tag, name) == 0) {
            tok->next = generateToken(start);
			tok = tok->next;
        }

        node *cur = start;

        while (cur) {
            if (cur->child)
                _searchTree(cur->child, name, tok);
			if (cur->brother)
                _searchTree(cur->brother, name, tok);
            cur = cur->brother;
        }
}

/*
 *      Search for all the nodes matching the tag = name
 */
_Token *searchTree(void *start, char *name) {
        node *tmp = start;
        if (tmp == NULL)
            return NULL;
        _Token sentinelle = { NULL, NULL };
        _searchTree(tmp, name, &sentinelle);
        return sentinelle.next;
}

/*
 *      Returns the pointer to the tag (name of the rulename) of the pointed node and stores its length
 */
char *getElementTag(void *node, int *len) {
    struct node *tmp = node;
    *len = strlen(tmp->tag);
    return tmp->tag;
}

/*
 *      Returns the pointer to the value (rulename) of the pointed node and stores its length
 */
char *getElementValue(void *node, int *len) {
    struct node *tmp = node;
    *len = tmp->len_value;
    return tmp->value;
}

/*
 *      Free the linked list of the response of searchTree
 */
void purgeElement(_Token **r) {
    if (*r == NULL) // empty _Token list
        return;

    _Token *tmp;
    while (*r) {
        tmp = (*r)->next;
        free(*r);
        *r = tmp;
    }

    free(r);
}

/*
 *      Free all the structure node (http tree)
 */
void purgeTree(void *root) {
    struct node *tmp = root;
    node *child = (tmp)->child;
    while (child) {
        node *next = child->brother;
        purgeTree(child);
        child = next;
    }

    free(tmp);
}

int parseur(char* req, int len) {
    createRoot("HTTP-message", req, len);
   /* char delim[] = "\r\n";
    char *ptr = strtok(req, delim);

    while (ptr != NULL) {
        printf("'%s'\n", ptr);
        ptr = strtok(NULL, delim);
    }
    return 0; // TODO*/
    //int leng = httpMessage(req, len);
    //printf("%d ==> %s", leng, req+leng);
    //return leng;
	
	return 0;
}
