#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "GET.h"
#include "module.image_magick.h"

int traiter_GET(_Token *root, Fichier* file){
	struct Options* host_prm = HostsParametres;
	_Token *req = searchTree(root, "request-line");
	_Token *t = searchTree(req->node, "absolute-path");
	
	//_Token *v = searchTree(req->node, "HTTP-version");
	/*Check de la version*/
	/*
	int l;
	char *host;
	if (!strncmp(getElementValue(v->node,&l),"HTTP/1.1",8)) {	//HTTP Version 1.1, Host header needed
		_Token *hst = searchTree(root, "Host");
		_Token *h = hst;
		if (h == NULL){
			purgeElement(&req);
			purgeElement(&t);
			purgeElement(&v);
			purgeElement(&h);
			return ERR_HOST_HEADER_MISSING;
		}
		
		host = getElementValue(h->node, &l);
		while (host_prm != NULL && strncmp(host_prm->ServerName, host, l)){	//Parcours les Hosts jusqu'à trouver celui correspondant
			host_prm = host_prm->next;
		}
		if (host_prm == NULL){
			purgeElement(&req);
			purgeElement(&t);
			purgeElement(&v);
			purgeElement(&h);
			return ERR_NON_EXISTING_HOST;
		}
		purgeElement(&h);
	}
	if (!strncmp(getElementValue(v->node,&l),"HTTP/1.0",8)){	//HTTP Version 1.0
		host_prm = HostsParametres;	//Host by default
	}
	else{
		purgeElement(&req);
		purgeElement(&t);
		purgeElement(&v);
		return ERR_WRONG_VERSION;
	}*/
	
	
	int len_path;
	char* path = getElementValue(t->node, &len_path);
	purgeElement(&req);
	purgeElement(&t);
	//purgeElement(&v);
	int root_path = strlen(host_prm->DocumentRoot);
	int total_path = len_path + root_path;
	
	if(total_path > PATH_LEN_MAX){
		
	}
	
	//Concaténation du path du dossier avec le absolute-path du fichier
	strcpy(file->path,host_prm->DocumentRoot);
	memcpy(file->path + root_path, path, len_path);	
	file->path[total_path] = '\0';//Ajout de la sentinelle
	
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
		return ERR_PATH_UNREACHABLE;
    }
	
	//Met le MIME type du fichier dans file->type
	get_mime_type(file);
    
	struct stat st;
    if (fstat(fichier, &st) == -1){
		close(fichier);
		return ERR_FSTAT;
	}
	
	file->length = st.st_size;	//Longueur du fichier
	
	close(fichier);
	return OK;
}
