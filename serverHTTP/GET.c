#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "GET.h"
#include "module.image_magick.h"

int traiter_GET(_Token *root, Fichier* file){
	char host[HOST_LEN_MAX];
	struct Options* host_prm = HostsParametres;
	_Token *req = searchTree(root, "request-line");
	_Token *t = searchTree(req->node, "absolute-path");
	//_Token *v = searchTree(req->node, "HTTP-version");
	/*Check de la version*/
	/*
	int l;
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
		strcpy(host, host_prm->ServerName);	//Host by default
	}
	else{
		purgeElement(&req);
		purgeElement(&t);
		purgeElement(&v);
		return ERR_WRONG_VERSION;
	}*/
	
	strcpy(host, host_prm->ServerName);	//A enlever quand on voudra checker la version
	
	int len_path;
	char* path = getElementValue(t->node, &len_path);
	if (!strncmp(path,"/",len_path)){	//Cas par défaut, on renvoit la page index.html
		strcpy(path,"/index.html");
		len_path = strlen(path);
	}
	
	strcpy(file->path,host_prm->DocumentRoot);
	memcpy(file->path + strlen(host_prm->DocumentRoot), path, len_path);	//Concaténation du path du dossier avec le absolute-path du fichier
	file->path[strlen(host_prm->DocumentRoot) + len_path] = '\0';	//Ajout de la sentinelle

	int fichier;
	if ((fichier= open(file->path, O_RDWR)) == -1) {	//vérifie qu'on peut ouvrir le fichier
        purgeElement(&req);
		purgeElement(&t);
		//purgeElement(&v);
		close(fichier);
		return ERR_PATH_UNREACHABLE;
    }
	get_file_type(file);	//Met le MIME type du fichier dans file->type
    struct stat st;
    if (fstat(fichier, &st) == -1){
        purgeElement(&req);
		purgeElement(&t);
		//purgeElement(&v);
		close(fichier);
		return ERR_FSTAT;
	}
	
	file->length = st.st_size;	//Longueur du fichier
	purgeElement(&req);
	purgeElement(&t);
	//purgeElement(&v);
	close(fichier);
	return OK;
}
