#include <stdio.h>	// printf
#include <stdlib.h>	// malloc
#include <string.h>	// strlen
#include <stdint.h>

#include "request_handler.h"
#include "GET.h"

//parser api
#include "api.h"



int RequestHandler(message *requete, HTML_Rep* reponse,Fichier* file){
	int res;
	if ((res=parseur(requete->buf,requete->len))) {
			_Token *r,*r2,*tok,*root;

			printf(GRN "Requete parsée\n" NC);
			
			root=getRootTree();
			
			// ECHO
			r  = searchTree(root, "request-line");
			r2 = searchTree(r->node, "method");
			tok=r2;
			int l;
			char *s;
			// Utilisation de getElementValue() pour traiter le type opaque 
			s = getElementValue(tok->node, &l);
			
			uint8_t method=-1;
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
			
			switch(method){
				case GET:
				case HEAD:
					traiter_GET(root, file);
					strcpy(reponse->content, REPONSE);
					reponse->len = strlen(REPONSE);
				break;
				default:
				break;
			}
			
			
			
			
			file->path[0]='\0';
			
			
			purgeElement(&r);
			purgeElement(&r2);
		purgeTree(root);
		} else {
			printf(RED "Requete non parsée\n" NC);
			strcpy(reponse->content, ERROR);
			reponse->len = strlen(ERROR);
		}
	
	
	
	
	return 0;
}