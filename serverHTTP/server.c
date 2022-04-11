#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// for librequest
#include "request.h"

// for parser

// this will declare internal type used by the parser
#include "httpparser.h"
#include "api.h"


//
#include "parametres.h"


int main(int argc, char *argv[])
{
	message *requete;
	int res;
	load_gramm_rule(HTTP_RULES,FIRST_TAG);
	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
		if ((requete=getRequest(8080)) == NULL ) return -1;

		// Affichage de debug
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId);
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);
		if (res=parseur(requete->buf,requete->len)) {
			_Token *r,*tok,*root;

			// get the root of the tree this is no longer opaque since we know the internal type with httpparser.h
			//void *root;
			writeDirectClient(requete->clientId,REPONSE,strlen(REPONSE));
			root=getRootTree();
			r=searchTree(root,"HTTP-message");
			tok=r;
			while (tok) {
				int l;
				char *s;
				// node is no longer opaque
				/* Lnode *node;
				node=(Lnode *)tok->node; */
				/** Utilisation de getElementValue() pour traiter le type opaque */
				s = getElementValue(tok->node, &l);
				writeDirectClient(requete->clientId,s,l);
				tok=tok->next;
			}
			purgeElement(&r);
		purgeTree(root);
		} else {
			writeDirectClient(requete->clientId,ERROR,strlen(ERROR));
		}
		endWriteDirectClient(requete->clientId);
		requestShutdownSocket(requete->clientId);
	// on ne se sert plus de requete a partir de maintenant, on peut donc liberer...
	freeRequest(requete);
	}
	printf("exit?\n");
	close_gramm_rule();
	return (1);
}
