#include <stdio.h>	// printf
#include <stdlib.h>	// malloc
#include <string.h>	// strlen
#include <stdint.h>

#include "request_handler.h"
#include "GET.h"
#include "error_mes.h"

//parser api
#include "api.h"



int RequestHandler(message *requete, HTML_Rep* reponse,Fichier* file){
	int res;
	uint8_t method=-1;
	if ((res=parseur(requete->buf,requete->len))) {
		_Token *r,*r2,*tok,*root;

		printf(GRN "Requete parsée\n" NC);
		
		root=getRootTree();
		
		// Obtention du type de méthode
		r  = searchTree(root, "request-line");
		r2 = searchTree(r->node, "method");
		tok=r2;
		int l;
		char *s;
		// Utilisation de getElementValue() pour traiter le type opaque 
		s = getElementValue(tok->node, &l);
		
		
		// pas beau mais nécessaire (peut-être cacher ça dans une fonction)
		if(!strncmp(s,"GET",l)){
			printf("GET\n");
			method = GET;
		}else if(!strncmp(s,"HEAD",l)){
			printf("HEAD\n");
			method = HEAD;
		}else if(!strncmp(s,"POST",l)){
			printf("POST\n");
			method = POST;
		}else if(!strncmp(s,"PUT",l)){
			printf("PUT\n");
			method = PUT;
		}else if(!strncmp(s,"DELETE",l)){
			printf("DELETE\n");
			method = DELETE;
		}else if(!strncmp(s,"CONNECT",l)){
			printf("CONNECT\n");
			method = CONNECT;
		}else if(!strncmp(s,"OPTIONS",l)){
			printf("OPTIONS\n");
			method = OPTIONS;
		}else if(!strncmp(s,"TRACE",l)){
			printf("TRACE\n");
			method = TRACE;
		}
		
		
		int status_code;
		switch(method){
			case GET:
			case HEAD:
				status_code = traiter_GET(root, file);
				//Note: si on implémente les ERR, passer sur un switch(res_get)
				/* Contre-note: La gestion des erreurs va vite devenir redondante
					Proposition: faire un if(status == OK) or if(status <0);
					Puis gestion des erreurs par un ErrorHandler()
					"Plus propre et lisible à mon avis ;] "
				Exemple:
				if(status <0){
					ErrorHandler(message *requete, HTML_Rep* reponse,Fichier* file);
				} else {
					// J'avoue penser que ce serait mieux si c'était traiter_GET qui remplissait la réponse
					sprintf(reponse->content, "HTTP/1.0 200 OK");
					reponse->len = strlen(reponse->content);
				}
				Eventuellement déplacer le ErrorHandler(...) en dehors du switch(method)
				Pour éviter plus de redondance
				*/
				switch (status_code){
					case OK:	//200 OK
						sprintf(reponse->content, ERROR_200);
						reponse->len = strlen(reponse->content);
					break;
					case ERR_NOT_FOUND: //404 Not Found
					case ERR_INTERNAL_SERVER:	//500 Internal Server Error
					case ERR_HTTP_VERSION: //505 HTTP Version Not Supported - PAS ENCORE IMPLEMENTE
						strcpy(reponse->content, TAB_ERROR[-1*status_code]);
						reponse->len = strlen(TAB_ERROR[-1*status_code]);
				}
			break;
			case POST:
			case PUT:
			case DELETE:
			case CONNECT:
			case OPTIONS:
			case TRACE:	//501 Not Implemented
				strcpy(reponse->content, ERROR_501);
				reponse->len = strlen(ERROR_501);
			break;
			default:	//400 Bad Request
				strcpy(reponse->content, ERROR_400);
				reponse->len = strlen(ERROR_400);
			break;
		}
		
		/*
		Proposition de réimplémentation:
		switch(method){
			case GET:
			case HEAD:
				status_code = traiter_GET(root, file);
			break;
			case POST:
			case PUT:
				status_code = -405; //Erreur 500 méthode non supporté
			break;
			default:
				status_code = -501; //Erreur 501 méthode non reconnue
		}
		if(status_code < 0)
			ErrorHandler(message *requete, HTML_Rep* reponse,Fichier* file);
		
		Less cluttered and
		probably more efficient / less redundant
		*/
		
		
		
		
		purgeElement(&r);
		purgeElement(&r2);
		purgeTree(root);
		
		} else {
			printf(RED "Requete non parsée\n" NC);
			/* ERROR BAD REQUEST */
			strcpy(reponse->content, ERROR);
			reponse->len = strlen(ERROR);
		}
	
	
	
	
	return method;
}