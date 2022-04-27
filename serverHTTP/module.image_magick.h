#include <magic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parametres.h"

struct MIME_Line {
	char exension[10];
	char mime_type[40];
};

/*Struct contenant les couples (extension, MIME type) non reconnus par libmagic*/
#define MIME_TAB_LEN 3
struct MIME_Line MIME_tab[MIME_TAB_LEN] = {
	{"js","application/javascript"},
	{"css","text/css"},
	{"csv","text/csv"}
};



/*Cas ou le type n'est pas reconnu par libmagic
  Ecrit le type d'un fichier dans f->type en fonction de son extension*/
void get_mime_from_file_extension(Fichier* f){
	char* p = f->path;
	char* extension;
	while (*p != '\0') if (*p++=='.') extension = p;
	int i=0;
	while (i<MIME_TAB_LEN){
		if(!strcmp(extension,MIME_tab[i].exension)){
			strcpy(f->type, MIME_tab[i].mime_type);
			i = MIME_TAB_LEN;
		}
		i++;
	}
}

/*trouve le MIME type d'un fichier et l'ecrit dans f->type*/
int get_mime_type(Fichier* f){
	const char* tmp;
	magic_t mag;
	/*Create magic cookie pointer*/
	mag = magic_open(MAGIC_MIME_TYPE);	//MAGIC_MIME_TYPE will allow to return a MIME type string
	if (mag == NULL){
		perror("magic open");
		return -1; //Error 500
	}
	if (magic_load(mag, NULL) != 0) {	//Load magic database
		printf("error loading magic database: %s\n", magic_error(mag));
        magic_close(mag);
        return -1; //Error 500
    }
	/*Get MIME type*/
	tmp = magic_file(mag, f->path);	//tmp is needed because magic_file returns a const char*
	//assert(strlen(tmp)<=TYPE_LEN_MAX );
	if (strlen(tmp)<=TYPE_LEN_MAX){
		strcpy(f->type,tmp);
	}
	else{
		return -1; //Error 500
	}
	/*Close magic database and deallocates ressources used ("free" tmp)*/
	magic_close(mag);
	
	//Cas ou libmagic n'est pas suffisant
	if (!strcmp(f->type,"text/plain"))
		get_mime_from_file_extension(f);
	
	return 0;
}