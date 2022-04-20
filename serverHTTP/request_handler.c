#include "request_handler.h"

int RequestHandler(message *requete, HTML_Rep* reponse,Fichier* file){
	
	if ((res=parseur(requete->buf,requete->len))) {
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
	
	
	
	
	return 0;
}