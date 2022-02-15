#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
/// Incluez ici les fichiers d'entête nécessaires pour l'execution de ce programme.
/// La fonction parseur doit être dans un autre fichier .c
/// N'ajouter aucun autre code dans ce fichier.
int main()
{
    char req[]="GET / HTTP/1.0\r\nHost: www.google.com\r\nTransfer-Encoding: gzip\r\n\r\n";
    parseur(req,strlen(req));
    return 0;
}