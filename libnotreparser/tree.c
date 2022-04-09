#include "tree.h"

//Couleurs pour l'arbre
#include "color.h"
#define C_LVL YEL
#define C_TAG CYN
#define C_STR GRN


node* treeRoot = NULL; // global

/*
 *      Returns the pointer to the global root
 */
void *getRootTree() {
    return treeRoot;
}

/*
 *      Free all the structure node (http tree)
 */
void purgeTree(void *root) {
    struct node *tmp = root;
    if (tmp == NULL)
		return;
  	purgeTree(tmp->brother);
  	purgeTree(tmp->child);
  #if DEBUG_TREE_FREE
  	printf("free: [%p] of node [%s] in purgeTree()\n", tmp, tmp->tag);
  #endif

    free(tmp);
}

/*
 *      Allocates and return a new node for the http tree after initializing it
 */
node *generateNode(char* tag, char* value, int len_value) {
    struct node *node = malloc(sizeof(struct node));
	#if DEBUG_TREE_MALLOC
		printf("malloc: [%p] in generateNode()\n", node);
	#endif
    if (node == NULL) {
        perror("generateNode: malloc failed");
        exit(1);
    }
    strcpy(node->tag, tag);
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
 *      Initalizes the global root of the http tree
 */
void createRoot(char* tag, char* value, int len_value) {
    treeRoot = generateNode(tag, value, len_value);
}

/*
 *      Generate a new node and adds it to the current node as a brother
 */
node *addNodeAsBrother(char* tag, char* value, int len_value, node* current) {
    if (current != NULL) { // check if the current node has been malloc
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

    if (cur->child)
        _searchTree(cur->child, name, tok);
    if (cur->brother)
        _searchTree(cur->brother, name, tok);
}

/*
 *      Displays the whole tree in the standard output
 */
void display_tree(node* noeud, int n) {	//first call: noeud=treeRoot, n=0
    for (int i = 0; i < n; i++)
        printf("  ");

    printf("["C_LVL"%d"NC":"C_TAG"%s"NC"] = \""C_STR, n, noeud->tag);
    fflush(stdout);
    int len = noeud->len_value;
    if (len >= DISPLAY_MAX)
        len = DISPLAY_MAX;
    int i = 0;
    while (i < len) {
        if (noeud->value[i] != '\n' && noeud->value[i] != '\r')
            putchar(noeud->value[i]);
        else
            putchar('_'); // in order to actually see the CRLF
        i++;
    }
    if (len < noeud->len_value) // to reduce the char printed if the string is too long
        printf("..");
    printf(NC"\"\n");

    if (noeud->child && n < MAX_DEPTH) {
        display_tree(noeud->child, n+1);
    }
    if (noeud->brother) {
        display_tree(noeud->brother, n);
    }
}
