//## ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ##
#ifndef MAXCLIENT //défini dans request.h
#include "request.h"
#endif
#include "../../annexe/global.h"
/*
	Est utile comme interprète principal entre le serveur et
		les implémentations de méthodes comme GET ou HEAD.
*/


// IMPORTANT Décider à quel endroit se fait le parsing de la requète:
//		(server.c ou request_handler.c)
// +++++++++ Décider si le RequestHandler à le droit de répondre directement au client
// 		ou si c'est le travail du serveur (à ce moment là le ReqH renvoirait une zone mem. )
/*
	Réponse de Jean:
	Idéalement je pense qu'on peut avoir notre Handler et notre Serveur en parallèle
	et dès que le Handler aurait un quelque chose à envoyer, il le ferait passer au Serveur 
	à l'aide d'un pipe par exemple (il faudrait éviter les recopies de données).
	Autre idée, le Handler peut juste retourner avec un code spécial, qui ordonnerait au serveur
	de répondre à la requète avec le fichier dont le chemin aurait été placé dans une zone spécial
	(comme ça la recopie serait limitée au nom du fichier)
*/
/*
	Ce qu'on a fait finalement:
	Le request handler remplis une zone de mémoire (de taille constante HEADER_LEN_MAX)
	RequestHandler parse la requête puis le reste du travail est relégué à d'autre programmes
	RequestHandler return avec le numéro correspondant à la requète si tout c'est bien passé
	puis le serveur (server.c) envoie donc la réponse construite et effectue ensuite
	l'action correspondante à la méthode.
*/

int RequestHandler(message *requete, HeaderStruct* headers, HTML_Rep* reponse,Fichier* file);

