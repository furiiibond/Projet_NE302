
#include "../../annexe/global.h"
#ifndef MAXCLIENT //défini dans request.h
#include "request.h"
#endif
#include "api.h"

int traiter_Header(_Token *root, HeaderStruct* headers);