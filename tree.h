#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "api.h"

//Constants defined here
#include "parametres.h"


// represents a node in the tree
typedef struct node {
    char tag[TAG_LEN];
    char* value; // pointer to the first char of the value
    int len_value;  // number of char starting from pointer value
    struct node *child; // pointer to the first child or NULL
    struct node *brother; // pointer to the next brother node or NULL
} node;

// returns root of the tree
void *getTreeRoot();

// initialize a struct node
node *generateNode(char* tag, char* value, int len_value);

// initialize a new token in the token list
_Token *generateToken(node* noeud);

// initialize the root of the tree
void createRoot(char* tag, char* value, int len_value);

// initialize a node and change current->brother to point to the newly created node
node *addNodeAsBrother(char* tag, char* value, int len_value, node* current);

// initialize a node and change current->child to point to the newly created node
node *addNodeAsChild(char* tag, char* value, int len_value, node* current);

// search for a node with a given name
void _searchTree(node *start, char *name, _Token **tok);

// display the tree with node hirarchy of nodes
void display_tree(node* noeud, int n);

#endif