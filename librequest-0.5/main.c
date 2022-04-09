#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"   

#define REPONSE "HTTP/1.0 200 OK\r\n\r\n"

#ifndef LEGACY
// ceci decrit la methode experimentale de renvoi de la reponse Cf API

#define REPONSE1 "HTTP/1.0 200 OK\r\n"
#define REPONSE2 "\r\n"

int main(int argc, char *argv[])
{
	message *requete; 

	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest. 
		if ((requete=getRequest(8080)) == NULL ) return -1; 

		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId); 
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);  

		writeDirectClient(requete->clientId,REPONSE1,strlen(REPONSE1)); 
		writeDirectClient(requete->clientId,REPONSE2,strlen(REPONSE2)); 
		endWriteDirectClient(requete->clientId); 
		requestShutdownSocket(requete->clientId); 
	// on ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
	freeRequest(requete); 
	}
	return (1);
}

#else  

// ceci decrit la methode legacy de renvoi de la reponse  Cf API
#define REPONSE "HTTP/1.0 200 OK\r\n\r\n"

int main(int argc, char *argv[])
{
	message *requete; 

	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest. 
		if ((requete=getRequest(8080)) == NULL ) return -1; 

		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId); 
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);  

// ceci decrit la methode legacy de renvoi de la reponse  Cf API
		// avant d'envoyer une réponse on doit allouer de la mémoire pour la reponse  
		if ((reponse=malloc(sizeof(message))) != NULL ) { 
			reponse->len=strlen(REPONSE); 

			// attention reponse->buf doit aussi pointer vers un espace memoire valide. 
			if ((reponse->buf=malloc(strlen(REPONSE))) != NULL ) {
				// on recopie la donnée dans la reponse 
				strncpy(reponse->buf,REPONSE,reponse->len); 
				reponse->clientId=requete->clientId; 
				// on envoi la reponse au client 
				sendReponse(reponse); 
				// reponse est recopiée par la bibliothèque, on peut tout de suite liberer la memoire
				free(reponse->buf); 
			}
			free(reponse); 
			//optionnel, ici on clot la connexion tout de suite (HTTP/1.0) 
			requestShutdownSocket(reponse->clientId); 
		}
#endif 		
