#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "methode.h"
#include "module.image_magick.h"

//Prototypes
struct Options* get_host_ptr(HeaderStruct* headers);

/*
	On va faire plus de choses là dedans, donc mieux commencer à découper
*/
int traiter_GET( HeaderStruct* headers, HTML_Rep* reponse, Fichier* file){
	struct Options* host_ptr;
	
	host_ptr = get_host_ptr(headers);
	
	int root_path = strlen(host_ptr->DocumentRoot);
	int total_path = headers->absolutePath.count + root_path;
	
	if(total_path > PATH_LEN_MAX){
		return ERR_400; //Chemin trop long
	}
	
	/* Percent encoding */
	// Youpi%20!
	
	//Concaténation du path du dossier avec le absolute-path du fichier
	strcpy(file->path,host_ptr->DocumentRoot);
	strncat(file->path, headers->absolutePath.data, headers->absolutePath.count);
	file->path[total_path] = '\0';//Ajout de la sentinelle
	
	/* Sanitize Path */
	// If below root: 403 Forbidden
	// Compter les dossiers traversé:
	//		dossier normal > +=1
	//		.			   > +=0
	//		-			   > -=1
	// Si négatif à un moment -> ERR_403
	
	char last_char = *(file->path +total_path-1);
	if (last_char == '/'){
		//get_default_page(file->path,total_path)
		strncpy(file->path+total_path,
				"index.html",
				PATH_LEN_MAX-total_path );
	}
	

	int fichier;
	//vérifie qu'on peut ouvrir le fichier
	if ((fichier= open(file->path, O_RDWR)) == -1) {
		close(fichier);
		printf(RED"ERROR"NC" file unreachable\n");
		return ERR_404; //File not found    
	}
	
	//Met le MIME type du fichier dans file->type
	if (get_mime_type(file) == -1)
		return ERR_500; //Internal server error
    
	struct stat st;
    if (fstat(fichier, &st) == -1){
		close(fichier);
		return ERR_500; //Internal server
	}
	
	file->length = st.st_size;	//Longueur du fichier
	
	close(fichier);
	
	
	/* On rempli la réponse */
	reponse->len = snprintf(reponse->content, HEADER_LEN_MAX,
		"HTTP/1.0 200 OK\r\nContent-type:%s\r\nContent-length:%ld\r\n",
			file->type, file->length);
	
	if(!headers->connection.keepAlive)
		reponse->len += snprintf(reponse->content+reponse->len,
		HEADER_LEN_MAX-reponse->len,
		"Connection: close\r\n");
	
	
	return OK;
}


struct Options* get_host_ptr(HeaderStruct* headers){
	struct Options* host_ptr = HostsParametres;
	
	if(headers->httpVersion >= 11)
		while(host_ptr && strncasecmp(
		host_ptr->ServerName, headers->host.data, 
	MAX(headers->host.count,strlen(host_ptr->ServerName))
		))
			host_ptr = host_ptr->next;
	
	if( !host_ptr )
		host_ptr = HostsParametres; //Par default, premier site;

	return host_ptr;
}

