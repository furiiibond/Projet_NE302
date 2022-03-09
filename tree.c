#include "tree.h"

node* treeRoot = NULL; // global

/*
 *      Returns the pointer to the global root
 */
void *getTreeRoot() {
    return treeRoot;
}

/*
 *      Allocates and return a new node for the http tree after initializing it
 */
node *generateNode(char* tag, char* value, int len_value) {
    struct node *node = malloc(sizeof(struct node));
    if (node == NULL) {
        perror("generateNode: malloc failed");
        exit(1);
    }
    strcpy(node->tag,tag);
    node->value = value;
    node->len_value = len_value;
    node->child = NULL;
    node->brother = NULL;
    return node;
}

/*
 *      Allocates and return a new Token
 */
_Token *generateToken(node* noeud) {
    _Token *tok = malloc(sizeof(_Token));
    if (tok == NULL) {
        perror("generateToken: malloc failed");
        exit(1);
    }
    tok->node = noeud;
    tok->next = NULL;
    return tok;
}

/*
 *      Initalize the global root of the http tree
 */
void createRoot(char* tag, char* value, int len_value) {
    treeRoot = generateNode(tag, value, len_value);
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


/*
 *      Traversal of the Tree and adding a new node to the linked list tok when matching name
 */
void _searchTree(node *start, char *name, _Token **tok) {
    if (start == NULL)
        return;

    if (strcmp(start->tag, name) == 0) {
        if (*tok == NULL)
            *tok = generateToken(start);
        else {
            _Token *tmp = *tok;
            while (tmp->next)
                tmp = tmp->next;
            tmp->next = generateToken(start);
        }
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
 *      Displays the whole tree in the standard output
 */
void display_tree(node* noeud, int n) {	//1er appel : noeud=treeRoot, n=0
    for (int i = 0; i < n; i++)
        printf("  ");

    printf("[%d]%s = \"", n, noeud->tag);
    fflush(stdout);
    int len = noeud->len_value;
    if (len >= DISPLAY_MAX)
        len = DISPLAY_MAX;
    int i = 0;
    while (i < len) {
        if (noeud->value[i] != '\n' && noeud->value[i] != '\r')
            putchar(noeud->value[i]);
        else
            putchar('_'); // in order to actually see the CRLF without a line feed
        i++;
    }
    if (len < noeud->len_value) // to reduce the char printed if the string is too long
        printf("..");
    printf("\"\n");

    if (noeud->child) {
        display_tree(noeud->child, n+1);
    }
    if (noeud->brother) {
        display_tree(noeud->brother, n);
    }
}