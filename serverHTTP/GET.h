#ifndef GET_H
#define GET_H

#include "api.h"
#include "parametres.h"

#define OK 1
#define ERR_WRONG_VERSION 2
#define ERR_HOST_HEADER_MISSING 3
#define ERR_NON_EXISTING_HOST 4
#define ERR_PATH_UNREACHABLE 5
#define ERR_FSTAT 6

int traiter_GET(_Token *root, Fichier* file);

#endif