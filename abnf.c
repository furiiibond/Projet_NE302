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

int body(char* req, int len) {
    int res = 0;

    for (int i = 0; i < 2; i++)
    {
        int leng_mot = mot(req, len);
        if (leng_mot != 0) {
            req += leng_mot;
            len -= leng_mot;
            int leng_ponc = ponct(req, len);
            req += leng_ponc;
            len -= leng_ponc;

            res += leng_mot + leng_ponc;
        }
        else {
            int leng_num = nombre(req, len);
            req += leng_num;
            len -= leng_num;
            int leng_sep = separateur(req, len);
            req += leng_sep;
            len -= leng_sep;

            res += leng_num + leng_sep;
        }
    }
    return res;
}

int mot(char* req, int len) {
    int i = 0;
    while (i < len && 'A' <= req[i] && req[i] <= 'Z')
        i++;

    int leng_separ = 0;
    if (i != 0) {
        req += i;
        len -= i;
        leng_separ = separateur(req, len);
    }
    return i + leng_separ;
}

int nombre(char* req, int len) {
    int i = 0;
    while (i < len && '0' <= req[i] && req[i] <= '9')
        i++;

    return i;
}

int separateur(char* req, int len) {
    char separ[] = " \t-_";
    int i = 0;

    while (i < len && strchr(separ, req[i]))
        i++;

    return i;
}

int ponct(char* req, int len) {
    char ponctu[] = ",.!?:";
    int i = 0;

    while (i < len && strchr(ponctu, req[i]))
        i++;

    return i;
}

int fin(char *req, int len) {
    char *fin = malloc(len);
    strcpy(fin, req);
    fin[3] = 0;
    if (strcmp(fin, "fin") == 0) {
        free(fin);
        return 3;
    }
    free(fin);
    return 0;
}