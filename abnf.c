#include <stdlib.h>
#include "abnf.h"

int httpMessage(char* req, int len) {
    // ajouter un noeud à l'arbre. La racine
    int leng = debut(req, len);
    req += leng;
    return leng;
}

int debut(char* req, int len) {
    char *start = malloc(len);
    strcpy(start, req);
    start[5] = 0;
    if (strcmp(start, "start") == 0) {
        free(start);
        return 5;
    }
    free(start);
    return 0;
}
