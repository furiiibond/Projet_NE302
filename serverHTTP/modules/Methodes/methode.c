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
int cat_n_with_percent_encoding(char* path, const char* data,int count);
int verif_path_sanity(char *path,int len);

/*
	Fais appel aux autres modules pour valider la requête, semantiquement
*/
int traiter_GET( HeaderStruct* headers, HTML_Rep* reponse, Fichier* file){
	struct Options* host_ptr;
	host_ptr = get_host_ptr(headers);
	int len;
	
	//Concaténation du path du dossier avec le absolute-path du fichier
	strcpy(file->path,host_ptr->DocumentRoot);
	len = cat_n_with_percent_encoding(file->path, headers->absolutePath.data, headers->absolutePath.count);
	if(len >= PATH_LEN_MAX){
		return ERR_400; //Chemin trop long
	}
	file->path[len] = '\0';//Ajout de la sentinelle
	
	/* Sanitize Path */
	int root_path = strlen(host_ptr->DocumentRoot);	
	if(verif_path_sanity(file->path+root_path,len-root_path)!=OK){
		return ERR_403; // 403 Forbidden
	}
	
	char last_char = *(file->path +len-1);
	if (last_char == '/'){
		//get_default_page(file->path) //A implémenter
		strncpy(file->path+len,
				"index.html",
				PATH_LEN_MAX-len );
	}
	

	int fichier;
	//vérifie qu'on peut ouvrir le fichier
	if ((fichier = open(file->path, O_RDWR)) == -1) {
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
		SERV_VERSION(headers->httpVersion) " 200 OK\r\nContent-type:%s\r\nContent-length:%ld\r\n",
			file->type, file->length);
	
	if(!headers->connection.keepAlive)
		reponse->len += snprintf(reponse->content+reponse->len,
		HEADER_LEN_MAX-reponse->len,
		"Connection: close\r\n");
	
	
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


/*
//Retour
void get_default_page(char* path,int len){
	
	int fd;
	char ht_path[PATH_LEN_MAX]
	strcpy(ht_path,path);
	strncpy(ht_path+len,".htaccess",PATH_LEN_MAX-len);
	
	//vérifie qu'on peut ouvrir le fichier
	if ((fd= open(ht_path, O_RDWR)) == -1) {
		strncpy(path+len, "index.html", PATH_LEN_MAX-len );
	}
	else{
		struct stat st;
		if (fstat(fichier, &st) != -1){
			char * addr
			if ((addr = mmap(NULL, st.st_size, PROT_WRITE, MAP_PRIVATE, fi, 0)) == NULL){
				//Look for DirectoryIndex
				String_View line,rule,ht_file={addr,st.st_size};
				
				
				do { 
				line = sv_chop_by_delim(&ht_file, '\n');    //line ends before '\n', rule start after '\n'
				rule = sv_chop_by_delim(&line, ' ');    //rule ends before ' ', line starts after ' ' (and ends before '\n')
				cmp = strncmp("DirectoryIndex",rule.data,rule.count);
				} while(rule.count > 0 && cmp);    //While we haven't found the string
		
			}
		}else{
		strncpy(path+len, "index.html", PATH_LEN_MAX-len );
		}
		close(fd);
	}
	
}
*/
