#include "error_handler.h"

/*
	Lorsqu'un module de traitement de requête (ex: traiter_GET)
	retourne un code d'erreur, cette fonction se charge de remplir
	le buffer de la réponse HTML en fonction.
*/

void ErrorHandler(HTML_Rep* reponse,Fichier* file, int error_code){
	
	strcpy(reponse->content, TAB_ERROR[-1*error_code]);
	reponse->len = strlen(TAB_ERROR[-1*error_code]);
	
}