#ifndef GET_H
#define GET_H

#include "api.h"
#include "../../annexe/global.h"

/*
	Ce fichier définit les prototypes des fonctions qui traiteront les différentes méthodes
*/

//Même si elle s'appelle traiter GET, cette fonction est aussi utilisé pour traiter HEAD (et POST dans certain cas)
int traiter_GET( HeaderStruct* headers, HTML_Rep* reponse, Fichier* file);

#endif
