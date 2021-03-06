#include <stdio.h>	// printf
#include <stdlib.h>	// malloc
#include <string.h>	// strlen
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "request_handler.h"
//inclusion des autres modules
#include "../ErrorHandler/error_handler.h"
#include "../Headers/header_parser.h"
#include "../Semantique/semantique.h"
#include "../Methodes/methode.h"
#include "../PHP/socket_1.h"

//parser api
#include "api.h"

struct Options* get_host_ptr(HeaderStruct* headers);
int traiter_URI(Fichier* file, HeaderStruct* headers, struct Options* host_ptr);

int RequestHandler(message *requete, HeaderStruct* headers, HTML_Rep* reponse, Fichier* file, Header_List* PHP_data){
	int status_code;
	int method;
	file->to_send=0;
	if ( parseur(requete->buf,requete->len) == FALSE ) {
		printf(RED "Requete non parsée\n" NC);
		/* ERROR BAD REQUEST */
		status_code = ERR_400;
	} else {
		//		Execution normale
		printf(GRN "Requete parsée\n" NC);
		struct Options* host_ptr;

		traiter_Header(headers);
		status_code = verifSemantique(headers);

		method = headers->method;

		/* ----------------------------------------- */
		if (status_code == OK){

			host_ptr = get_host_ptr(headers);

			status_code = traiter_URI(file,headers,host_ptr);
		}


		/* ----------------------------------------- */
		if (status_code == OK){
			if(!strcmp(file->path+file->length-4, ".php")){
				printf(BLU"Request PHP script execution"NC"\n");
				status_code = executePHP(host_ptr, headers, file, reponse, PHP_data);
			}
			else
			switch(method){
				case GET:
				case POST:
					file->to_send = 1;
					/* fall through */
				case HEAD:
					status_code = traiter_GET(headers, reponse, file);
				break;
				case PUT:
				case DELETE:
				case CONNECT:
				case OPTIONS:
				case TRACE:
					status_code = ERR_405; //Erreur 405 méthode non supporté
				break;
				default:
					status_code = ERR_501; //Erreur 501 méthode non reconnue
			}
		}
	}

	if(status_code < 0){
		file->to_send = 0;// Out of place mais plus facile
		ErrorHandler(reponse, headers, /*file,*/ status_code);
		method=status_code;
	}

	/* Retourne la méthode si tout c'est bien passé
		le code d'erreur sinon */
	return method;
}


/** ------------------------------------------------------------- */


/* ------------ Annexe ------------ */ // <-- Nécessite son propre fichier surement

int cat_n_with_percent_encoding(char* path, const char* data,int count);
int verif_path_sanity(char *path,int len);
int get_default_page(char* path, int len);


int traiter_URI(Fichier* file, HeaderStruct* headers, struct Options* host_ptr){
	//Concaténation du path du dossier avec le absolute-path du fichier
	strcpy(file->path,host_ptr->DocumentRoot);
	file->middle = strlen(host_ptr->DocumentRoot);

	int len = cat_n_with_percent_encoding(file->path, headers->absolutePath.data, headers->absolutePath.count);
	file->path[len] = '\0';//Ajout de la sentinelle

	/* Sanitize Path */
	int root_path = strlen(host_ptr->DocumentRoot);
	if(verif_path_sanity(file->path+root_path,len-root_path)!=OK)
		return ERR_403; // 403 Forbidden

	char* last_char = (file->path +len-1);
	if (*last_char == '/')
		len = get_default_page(file->path, len);

	file->path[len]='\0';
	file->length=len;
	printf("full path: %s\n",file->path);
	file->last_slash = strrchr(file->path, '/') - file->path;


	return OK;
}




/* Gestion du multi-site
	renvoie un pointeur vers la structure d'option
	du site correspondant au champ Host.
	Renvoie le site par défaut si non-trouvé
*/
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



/* Vérifie si le path du origin-form ne traverse pas
	des endroits non-accessible
	(ex: tout dossier avant le root-directory)
*/
int verif_path_sanity(char *path,int len){
	String_View sv = {len,path};
	if(*sv.data=='/') {sv.data++; sv.count--;}

	// If below root: 403 Forbidden
	// Compter les dossiers traversé:
	//		dossier normal > +=1
	//		.			   > +=0
	//		-			   > -=1
	// Si négatif à un moment -> ERR_403
	int depth=0;
	String_View line;

	do {
	line = sv_chop_by_delim(&sv, '/');    //line ends before '/', rule start after '/'
	if (!strncmp(line.data,".",line.count))
		depth+=0;
	else if (!strncmp(line.data,"..",line.count))
		depth-=1;
	else
		depth+=1;
	} while(sv.count > 0 && depth >= 0);

	return depth<0 ? -1 : OK;
}


/* Fonctionnement identique à strncat()
	Mais avec le traitement de l'encodage par pourcent en plus
   Renvoie la longueur de la nouvelle chaine de charactères
*/
int cat_n_with_percent_encoding(char* path, const char* data,int count){
	int i=0,j=0,nb;
	while(path[i]!='\0')i++;

	while(i<PATH_LEN_MAX && j<count ){

		if(data[j]!='%'){
			path[i]=data[j];
		}else
		{
			nb=42;
			sscanf(&data[j+1],"%X",&nb);
			path[i]=nb;
			j+=2;
		}
		i++;
		j++;
	}
	return i;
}



//Retour
int get_default_page(char* path,int len){
	int fd;
	char ht_path[PATH_LEN_MAX];
	strcpy(ht_path,path);
	strncpy(ht_path+len,".htaccess",PATH_LEN_MAX-len);

	//vérifie qu'on peut ouvrir le fichier
	if ((fd= open(ht_path, O_RDWR))>=0){
		struct stat st;
		if (fstat(fd, &st) != -1){
			char * addr;
			if ((addr = mmap(NULL, st.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0))){
				//Look for DirectoryIndex
				String_View line,rule,ht_file={st.st_size,addr};

				int cmp=1;
				do {
				line = sv_chop_by_delim(&ht_file, '\n');    //line ends before '\n', rule start after '\n'
				rule = sv_chop_by_delim(&line, ' ');    //rule ends before ' ', line starts after ' ' (and ends before '\n')
				cmp = strncmp("DirectoryIndex",rule.data,rule.count);
				} while( line.count> 0 && cmp);    //While we haven't found the string

				if(cmp == 0){ //On a donc trouvé "DirectoryIndex"
					// Iterate over every entries while we didn't found a valid file
					do {
						rule = sv_chop_by_delim(&line, ' ');
						strncpy(path+len, rule.data, PATH_LEN_MAX-rule.count );
						path[len+rule.count]='\0';
						cmp = open(path, O_RDWR);// test access
						//printf("file:%.*s open:%d\n",(int)rule.count,rule.data,cmp);
					}while( cmp <0 && line.count>0);

					if(cmp>=0){//If file is accessible
						close(fd);
						close(cmp);
						printf("full path:[%s]\n",path);
						return len+rule.count; //because we don't want to activate the fallback mechanism
					}
				}
			}
		}
		close(fd);
	}

	// If anything fails, fallback to index.html
	strncpy(path+len, "index.html", PATH_LEN_MAX-len );
	return len+11;
}
