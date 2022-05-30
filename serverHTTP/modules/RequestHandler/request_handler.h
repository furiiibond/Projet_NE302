#ifndef MAXCLIENT //défini dans request.h
#include "request.h"
#endif
#include "../../annexe/global.h"
/*
	Est utile comme interprète principal entre le serveur et
		les implémentations de méthodes comme GET ou HEAD.
*/


/*
	Le request handler remplis une zone de mémoire (de taille constante HEADER_LEN_MAX)
	RequestHandler parse la requête puis le reste du travail est relégué à d'autre programmes
	RequestHandler return avec le numéro correspondant à la requète si tout c'est bien passé
	puis le serveur (server.c) envoie donc la réponse construite et effectue ensuite
	l'action correspondante à la méthode.
*/

int RequestHandler(message *requete, HeaderStruct* headers, HTML_Rep* reponse, Header_List* reponseHL, Fichier* file, Header_List* PHP_data);

