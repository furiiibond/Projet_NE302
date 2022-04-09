/** 
\file request.h 
\author Quentin Giorgi 
\version   1.0
\date       13 Decembre 2015

\brief      Interface de traitement des requetes et reponses HTTP.
 
Fichier d'interface entre la bibliothèque et votre programme.
Votre programme doit inclure ce fichier d'entete  #include <request.h> 
La compilation doit inclure l'option -L. -lrequest

Exemple de programme: 
\code{.c}
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"   

int main(int argc, char *argv[])
{
	message *requete; 
	message *reponse;

	while ( 1 ) {
		// on attend la reception d'une requete 
		requete=getRequest(8080);

		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId); 
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);  

		// Si on a une reponse a faire
		if (argv[1]) {
			if (reponse=malloc(sizeof(message))) { 
				reponse->buf=argv[1]; // on devrait l'allouer sinon
				reponse->len=strlen(argv[1]); 
				reponse->clientId=requete->clientId; 
				sendReponse(reponse); 
				// reponse est recopiée on peut tout de suite liberer la memoire
				free(reponse); 
				//optionnel, ici on clot la connexion tout de suite (HTTP/1.0) 
				requestShutdownSocket(reponse->clientId); 
			}
		} 		
		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
		freeRequest(requete); 
	}

	return (1);
}
\endcode

*/

#define MAXCLIENT 10

/**
 * \struct message
 * \brief Objet message qui peut etre soit une requete, soit une reponse. utilisé pour communiquer entre le programme et la bibliothèque.
 *
 * Pour les requetes l'allocation du pointeur buf est fait par la librairie, le programme recoit l'information par la fonction getRequest, et une fois le message traité par le programme, il doit libérer explicitement le message par la fonction freeRequest. 
 * 
 * Pour les reponses, la librairie recopie les données pointées dans une structure de message interne, le programme peut donc liberer la memoire si necessaire tout de suite après l'appel à sendReponse. 


*/

typedef struct {
	char *buf;				/**< buf Un pointeur vers le message recu */
	unsigned int len;			/**< len La longueur du message recu */
	unsigned int clientId;			/**< clientId identifiant du client, recu dans une requete, doit etre recopier dans la reponse correspondante*/
	struct sockaddr_in *clientAddress; 	/**< iclientAddress pointeur vers une structure permettant de recuperer l'adresse IP et le port du client HTTP */
} message;


/** 
\fn message *getRequest(short int port)
 * \brief Fonction de recuperation d'une requete, cette fonction est bloquante et doit etre appelée dans la boucle principale du programme.
 *  Cette fonction essaie de récupérer une requête entière pour la livrer à votre programme, mais si des cas d'erreur se produisent,elle livre ce qui a été recu à l'instant t sans filtrage, c'est votre programme qui devra traiter ces cas d'erreurs. 
 * \param port port d'écoute de la socket, utilisé qu'au premier appel de la fonction, ensuite ce parametre est ignoré dans les appels successifs.  
 * \return un pointeur vers une structure message.
 */

message *getRequest(short int port); 

/** 
\fn void freeRequest(message *r)
* \brief Procedure de libération de la memoire quand le programme en a fini avec une requete (message *). 
* \param r Le message à libérer. 
*/
void freeRequest(message *r); 

/** 
\fn void sendReponse(message *r)
* \brief Procedure d'envoi d'un message au client HTTP. Ici la réponse est totalement formée (entête et body) dans un buffer en mémoire avant d'être envoyée au client. 
*  cette fonction recopie les données dans un buffer et s'assure de les envoyer quand la socket est disponible (la procédure appelante put tout de suite libérer la mémoire) 
* \param r Le message à envoyer (recopié par la bibliothèque) 
*/
void sendReponse(message *r); 

/** 
\fn void writeDirectClient(int i,char *buf,unsigned int len)
* \brief Procedure (expérimentale) [alternative à sendReponse] d'envoi d'un buffer au client i. Il est parfois pratique d'écrire au client au fur et à mesure du traitement de la requête.
*  La librairie ne peut pas determiner toute seule la fin de la réponse. Si vous utilisez cette fonction il faut OBLIGATOIREMENT utiliser la fonction endWriteDirectClient quand la réponse est finie.
*  L'intérêt ici est de ne pas avoir à stocker dans des buffers la totalité de la réponse. 
* \param i Le client 
* \param buf Le message à envoyer (non recopié par la bibliothèque)
* \param len La longueur du message à envoyer 
*/
void writeDirectClient(int i,char *buf,unsigned int len);

/** 
\fn void endWriteDirectClient(int i)
* \brief Procedure (expérimentale) de déclaration de la fin de la réponse envoyée au client i HTTP. doit être utilisés si et seulement si vous utilisez writeDirectClient
* \param i Le client 
*/
void endWriteDirectClient(int i);


/** 
\fn void requestShutdownSocket(int i)
* \brief Procedure de demande de cloture de la connexion, si la bibliothèque à encore des données à envoyer d'un sendReponse précédent, la connexion ne sera fermée qu'à la fin de cet envoi.  
* \param i L'Id du client dont on doit fermer la connexion. 
*/
void requestShutdownSocket(int i); 



