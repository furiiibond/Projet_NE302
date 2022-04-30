#include <stdio.h>	// printf
#include <stdlib.h>	// malloc
#include <string.h>	// strlen
#include <stdint.h>

#include "request_handler.h"
#include "../ErrorHandler/error_handler.h"
#include "../Headers/header_parser.h"
#include "../Semantique/semantique.h"
#include "../Methodes/methode.h"
#include "../../annexe/error_mes.h" //Deprecated

//parser api
#include "api.h"



int RequestHandler(message *requete, HeaderStruct* headers, HTML_Rep* reponse,Fichier* file){
	int res,status_code;
	if ((res=parseur(requete->buf,requete->len))) {
		_Token *root;

		printf(GRN "Requete parsée\n" NC);
		
		root=getRootTree();
		
		traiter_Header(root, headers);
		
		status_code = verifSemantique(headers);
		
		if (status_code == OK)
		switch(headers->method){
			case GET:
			case HEAD:
				status_code = traiter_GET(headers, reponse, file);
			break;
			case POST:
			case PUT:
			case DELETE:
			case CONNECT:
			case OPTIONS:
			case TRACE:
				status_code = ERR_405; //Erreur 405 méthode non supporté
			break;
			default:
				status_code = ERR_501; //Erreur 501 méthode non reconnue
		}
		// Less cluttered and
		// probably more efficient / less redundant
		
		
		purgeTree(root);
		
	} else {
		printf(RED "Requete non parsée\n" NC);
		/* ERROR BAD REQUEST */
		status_code = ERR_400;
	}

	if(status_code < 0)
		ErrorHandler(reponse, /*file,*/ status_code);
	
	
	return headers->method;
}