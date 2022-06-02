#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "methode.h"
#include "module.image_magick.h"



/*
	Fais appel aux autres modules pour valider la requête, semantiquement
*/
int traiter_GET( HeaderStruct* headers, HTML_Rep* reponse, Fichier* file){

	int fichier;
	//vérifie qu'on peut ouvrir le fichier
	if ((fichier = open(file->path, O_RDWR)) == -1) {
		printf(RED"ERROR"NC" file unreachable\n");
		file->to_send = 0;
		return ERR_404; //File not found
	}

	//Met le MIME type du fichier dans file->type
	if (get_mime_type(file) == -1){
		close(fichier);
		return ERR_500; //Internal server error
	}

	struct stat st;
    if (fstat(fichier, &st) == -1){
		close(fichier);
		return ERR_500; //Internal server error
	}

	file->length = st.st_size;	//Longueur du fichier

	close(fichier);


	/* On rempli la réponse */
	//HTTP Version
	reponse->len = snprintf(reponse->content, HEADER_LEN_MAX, SERV_VERSION(headers->httpVersion) );
	//Status Code + Content-Type + Content-Length
	reponse->len += snprintf(reponse->content+reponse->len, HEADER_LEN_MAX-reponse->len,
		" 200 OK\r\nContent-type:%s\r\nContent-length:%ld\r\n",
			file->type, file->length);
	//Connection
	if(headers->connection.keepAlive)
		reponse->len += snprintf(reponse->content+reponse->len,
			HEADER_LEN_MAX-reponse->len,
			"Connection: keep-alive\r\nKeep-Alive: timeout=5, max=2\r\n");
	else
		reponse->len += snprintf(reponse->content+reponse->len,
		HEADER_LEN_MAX-reponse->len,
		"Connection: close\r\n");

	reponse->len += snprintf(reponse->content+reponse->len,
		HEADER_LEN_MAX-reponse->len, "\r\n");


	return OK;
}
