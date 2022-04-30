#include "../../annexe/global.h"
#include "../../annexe/error_mes.h"
#include <string.h>

/*
	Lorsqu'un module de traitement de requête (ex: traiter_GET)
	retourne un code d'erreur, cette fonction se charge de remplir
	le buffer de la réponse HTML en fonction.
*/

void ErrorHandler(HTML_Rep* reponse/*,Fichier* file*/, int error_code);