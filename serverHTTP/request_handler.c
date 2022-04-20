#include <stdio.h>	// printf
#include <stdlib.h>	// malloc
#include <string.h>	// strlen

#include "request_handler.h"

//parser api
#include "api.h"



int RequestHandler(message *requete, HTML_Rep* reponse,Fichier* file){
	int res;
	if ((res=parseur(requete->buf,requete->len))) {
			_Token *r,*r2,*tok,*root;

			printf(GRN "Requete parsée\n" NC);
			
			strcpy(reponse->content, REPONSE);
			reponse->len = strlen(REPONSE);
			
			root=getRootTree();
			
			// ECHO
			r  = searchTree(root, "request-line");
			r2 = searchTree(r->node, "method");
			tok=r2;
			int l;
			char *s;
			// Utilisation de getElementValue() pour traiter le type opaque 
			s = getElementValue(tok->node, &l);
			
			if(!strncmp(s,"GET",l)){
				printf("GET\n");
				 
				
			}
			
			file->path[0]='\0';
			
			
			purgeElement(&r);
			purgeElement(&r2);
		purgeTree(root);
		} else {
			strcpy(reponse->content, ERROR);
			reponse->len = strlen(ERROR);
		}
	
	
	
	
	return 0;
}