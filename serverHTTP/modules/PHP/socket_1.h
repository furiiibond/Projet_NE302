#include "../../annexe/global.h"

/** Envoie une requète au serveur PHP et copie sa réponse dans PHP_data */
int executePHP(struct Options* site_param, HeaderStruct* headers, Fichier* file, HTML_Rep* reponse, Header_List* PHP_data);
