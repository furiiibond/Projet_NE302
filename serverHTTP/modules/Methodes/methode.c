#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "methode.h"
#include "module.image_magick.h"

//Prototypes
Header_List* set_VerAndStatus( Header_List* reponseHL, HeaderStruct* headers);
Header_List* add_ContentType( Header_List* reponseHL, Fichier* file);



/*
	Fais appel aux autres modules pour valider la requête, semantiquement
*/
int traiter_GET( HeaderStruct* headers, HTML_Rep* reponse, Header_List* reponseHL, Fichier* file){

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
	reponse->len += snprintf(reponse->content+reponse->len, HEADER_LEN_MAX,
		" 200 OK\r\nContent-type:%s\r\nContent-length:%ld\r\n",
			file->type, file->length);
	//Connection
	if(!headers->connection.keepAlive)
		reponse->len += snprintf(reponse->content+reponse->len,
		HEADER_LEN_MAX-reponse->len,
		"Connection: close\r\n");

	/** DYNAMIC ALLOCATION --------------------- */
	//Fonction pour remplir la header list
	set_VerAndStatus(reponseHL,headers);
	add_ContentType(reponseHL,file);
	/*
	add_ContentLength(reponseHL,headers,File);
	add_Connection(reponseHL,headers);
	*/
	/** ---------------------------------------- */

	return OK;
}

/** DYNAMIC ALLOCATION ------------------------------------------ */
/* ------ Header List func ------ */

// C'est marrant pcq on a l'impression de donner plus de liberté (/flexibilité)
// En utilisant des mallocs, mais au final la plupart des champs sont cappé par
// un max (un define dans parametres.h)
// Étant donné qu'on a controle sur ça, ne vaudrait-t-il mieux pas rester juste
// sur un buffer de taille fixe ???
// Exemple pour Content Type:
//	-	"Content-Type: " -> 14 char
//	-	"%s",buffer[TYPE_LEN_MAX] -> y char
//	-	"\r\n"			 -> 2 char
//  TYPE_LEN_MAX = 64
//	MAX = 14 + 64 + 2 (supérieur à 16+y)

Header_List* set_VerAndStatus( Header_List* reponseHL, HeaderStruct* headers){
	reponseHL->header.data[7] = '0' + (headers->httpVersion >= 11);
	strncpy(reponseHL->header.data + 9,  "200 OK\r\n",  100-9);
	reponseHL->header.count = 19;
	return reponseHL;
}

Header_List* add_ContentType( Header_List* reponseHL, Fichier* file){
	Header_List* ptr = reponseHL;
	while(ptr->next) ptr=ptr->next;
	ptr->next = malloc(sizeof(Header_List));
	ptr=ptr->next;
	ptr->header.count = 16 + strlen(file->type);
	ptr->header.data = malloc( sizeof(char) * ptr->header.count );
	ptr->next=NULL;
	snprintf(ptr->header.data , ptr->header.count,
		"Content-type:%s\r\n",
			file->type);
	return ptr;
}
