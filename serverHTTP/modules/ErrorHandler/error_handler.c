#include "error_handler.h"

/*
	Lorsqu'un module de traitement de requête (ex: traiter_GET)
	retourne un code d'erreur, cette fonction se charge de remplir
	le buffer de la réponse HTML en fonction.
*/
// TAB_ERROR, est défini dans annexe/error_mes.h

void ErrorHandler(HTML_Rep* reponse, HeaderStruct* headers/*,Fichier* file*/, int error_code){
	
	strcpy(reponse->content, SERV_VERSION(headers->httpVersion));
	strcat(reponse->content, TAB_ERROR[-1*error_code]);
	reponse->len = strlen(reponse->content);
	
}