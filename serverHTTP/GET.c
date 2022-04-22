#include "GET.h"
#include <string.h>
#include <stdio.h>

int traiter_GET(_Token *root, Fichier* file){
	_Token *r,*tok; int ret=0;
	
	r  = searchTree(root, "absolute-path");
	tok=r;
	
	
	int len1 = strlen(HostsParametres->DocumentRoot);
	int len2; char* abs_path;
	
	strncpy(file->path, HostsParametres->DocumentRoot, PATH_LEN_MAX);
	
	abs_path = getElementValue(tok->node, &len2);
	
	if(len1 + len2 < PATH_LEN_MAX){
		
		strncpy(file->path + len1, abs_path, len2);
		
		char last_char = *(file->path +len1 +len2 -1);
		
		printf("[last char]=%c\n",last_char);
		
		if (last_char == '/'){
			strncpy(file->path+len1+len2, "index.html", PATH_LEN_MAX-len1-len2);
		}
		
		
	} else {
		file->path[0]='\0';
		ret = -1;
		printf(RED"file path exceed PATH_LEN_MAX\n"NC);
	}
	
	
	purgeElement(&r);

    return ret=-1;
}