#include <magic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parametres.h"

void get_file_type(Fichier f){
	const char* tmp;
	magic_t mag;
	/*Create magic cookie pointer*/
	mag = magic_open(MAGIC_MIME_TYPE);	//MAGIC_MIME_TYPE will allow to return a MIME type string
	if (mag == NULL){
		perror("magic open");
		strcpy(type,DEFAULT_TYPE);
		return;
	}
	if (magic_load(mag, NULL) != 0) {	//Load magic database
		printf("error loading magic database: %s\n", magic_error(mag));
        magic_close(mag);
		strcpy(type,DEFAULT_TYPE);
        return;
    }
	/*Get MIME type*/
	tmp = magic_file(mag, f.path);	//tmp is needed because magic_file returns a const char*
	assert(strlen(tmp)<=TYPE_LEN_MAX );
	strcpy(f.type,tmp);
	/*Close magic database and deallocates ressources used ("free" tmp)*/
	magic_close(mag);
}