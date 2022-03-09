#include "parser.h"
#include "annexe.h"
#include "tree.h"

/*
 *      Returns the pointer to the global root
 */
void *getRootTree() {
    return getTreeRoot();
}

/*
 *      Search for all the nodes matching the tag = name
 */
_Token *searchTree(void *start, char *name) {
        node *tmp = start;
        if (tmp == NULL)
            tmp = getRootTree();
        _Token *r = NULL;
        _searchTree(tmp, name, &r);
        return r;
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
}

/*
 *      Free all the structure node (http tree)
 */
void purgeTree(void *root) {
    struct node *tmp = root;
    node *child = tmp->child;
    while (child) {
        node *next = child->brother;
        purgeTree(child);
        child = next;
    }

    free(tmp);
}

int parseur(char *req, int len) {
    
    puts("OK");

    mem = req;
    no_go_zone = mem + len;
	createRoot("", NULL, 0);
    int r = construire("HTTP-message",getRootTree());

    printf("Valid:");
    truth(r);

    return r;
}
