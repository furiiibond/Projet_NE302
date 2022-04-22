#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>	// open
#include <sys/stat.h>	// open
#include <fcntl.h>		// open
#include <unistd.h>		// close
 #include <sys/mman.h>	// mmap

#include "parametres.h"
#include "option_parser.h"
#include "api.h"

struct Options* HostsParametres;


int FillHostsParametres(void){
	
	// HostsParametres = malloc(sizeof(struct Options));
	// strcpy(HostsParametres->ServerName, "Sitecool.com");
	// strcpy(HostsParametres->DocumentRoot, "/mnt/c/Users/J/OneDrive/3A/Cours_S2/Projet_NE302/Projet_NE302/www");
	
	// HostsParametres->next = malloc(sizeof(struct Options));
	// strncpy(HostsParametres->next->ServerName, "NumeroDos.tv",HOST_LEN_MAX);
	// strncpy(HostsParametres->next->DocumentRoot, "/home/userir/2",PATH_LEN_MAX);
	
	// HostsParametres->next->next = NULL;
	
	
	load_gramm_rule(CONF_RULES);// charge la grammaire HTTP
	
	int fd;
	char * ptr;
    if ((fd= open(SERV_CONFIG, O_RDWR)) == -1) {
        perror("Fichier de configuration serveur innexistant");
        exit(1);
    }
    struct stat st;
    if (fstat(fd, &st) == -1){
        exit(1);
	}
    if ((ptr = mmap(NULL, st.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0)) == NULL){
        exit(1);
	}
    
	if ( parseur(ptr, st.st_size) == FALSE){
		perror("Fichier de configuration non valide");
		exit(1);
	}
	
	_Token *r,*r2,*tok,*Entry_list;
	Entry_list = searchTree( NULL, "Entry");
	
	struct Options* hostparlist = NULL;
	
	tok = Entry_list;
	
	if(!tok){ // Ne devrait pas arriver
		perror("Fichier de configuration non valide");
		exit(1);
	}
	
	while(tok != NULL){
		if (hostparlist){
			hostparlist->next = malloc(sizeof(struct Options));
			hostparlist = hostparlist->next;
		}else{
			hostparlist = malloc(sizeof(struct Options));
			HostsParametres = hostparlist;
		}
		char *s; int l;
		r = searchTree( tok->node, "Hostname");
		s = getElementValue(r->node, &l);
		memcpy(hostparlist->ServerName, s, l);
		hostparlist->ServerName[l]='\0';
		
		printf("Filling %s infos\n",hostparlist->ServerName);
		
		purgeElement(&r);
		r = searchTree( tok->node, "DocRoot");
		r2 = searchTree( r->node, "Path");
		s = getElementValue(r2->node, &l);
		memcpy(hostparlist->DocumentRoot, s, l);
		hostparlist->DocumentRoot[l]='\0';
	
		purgeElement(&r);
		purgeElement(&r2);
		
		tok = tok->next;
    }

	purgeElement(&Entry_list);
	purgeTree(NULL);
    close(fd);
	close_gramm_rule();
	
	printf(GRN"DONE PARSING OPTIONS\n"NC);
	
	return 0;
}