#include <stdio.h>	// printf
#include <stdlib.h>	// malloc
#include <string.h>	// strlen
#include <stdint.h>

#include "request_handler.h"
#include "../ErrorHandler/error_handler.h"
#include "../Headers/header_parser.h"
#include "../GET/GET.h"
#include "../../annexe/error_mes.h" //Deprecated

//parser api
#include "api.h"



int RequestHandler(message *requete, HeaderStruct* headers, HTML_Rep* reponse,Fichier* file){
	int res;
	if ((res=parseur(requete->buf,requete->len))) {
		_Token *root;
		int status_code;

		printf(GRN "Requete parsée\n" NC);
		
		root=getRootTree();
		
		if( (status_code=traiter_Header(root, headers)) < 0){
			ErrorHandler(reponse, file, status_code );
		}
		
		
		
		
		switch(headers->method){
			case GET:
			case HEAD:
				status_code = traiter_GET(root, reponse, file);
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
				status_code = ERR_505; //Erreur 501 méthode non reconnue
		}
		if(status_code < 0)
			ErrorHandler(reponse, file, status_code);
		// Less cluttered and
		// probably more efficient / less redundant
		
		
		purgeTree(root);
		
		} else {
			printf(RED "Requete non parsée\n" NC);
			/* ERROR BAD REQUEST */
			ErrorHandler(reponse, file, ERR_400);
		}
	
	
	
	
	return headers->method;
}