#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "api.h"

// nb of char displayed by line in display_tree()
#define DISPLAY_MAX 30

typedef struct node {
    char* tag;
    char* value; // pointer to the first char of the value
    int len_value;  // number of char starting from pointer value
    struct node *child; // pointer to the first child or NULL
    struct node *brother; // pointer to the next brother node or NULL
} node;

void *getTreeRoot();

node *generateNode(char* tag, char* value, int len_value);

_Token *generateToken(node* noeud);

void createRoot(char* tag, char* value, int len_value);

node *addNodeAsBrother(char* tag, char* value, int len_value, node* current);

node *addNodeAsChild(char* tag, char* value, int len_value, node* current);

void _searchTree(node *start, char *name, _Token **tok);

void display_tree(node* noeud, int n);

#endif