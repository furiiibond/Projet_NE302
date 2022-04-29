#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>	// open
#include <sys/stat.h>	// open
#include <fcntl.h>		// open
#include <unistd.h>		// close
 #include <sys/mman.h>	// mmap

#include "../../global.h"
#include "option_parser.h"

// -L notreparser
#include "api.h"

struct Options* HostsParametres;


int FillHostsParametres(void){	
	
	load_gramm_rule(CONF_RULES);// charge la grammaire HTTP
	
	
	/* Ouverture du fichier de config $(SERV_CONFIG) */
	int fd; char * ptr;
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
    
	// Parsing
	if ( parseur(ptr, st.st_size) == FALSE){
		perror("Fichier de configuration non valide");
		exit(1);
	}
	
	/** A faire:
			vérifier si le fichier à été parsé entièrement,
			sinon mettre un warning ("Possible Entry Miss")
	{
		_Token tok; int l;
		tok = searchTree( NULL, "Config");
		getElementValue(tok->node, &l);
		if (l < st.st_size)
			printf(MAG"Warning: Possible Entry Miss"NC" - Check your "SERV_CONFIG" file\n");
	
	*/
	
	_Token *r,*r2,*tok,*Entry_list;
	Entry_list = searchTree( NULL, "Entry"); // Le 1er arg est NULL donc utilise parse tree root
	struct Options* hostparlist = NULL;
	tok = Entry_list;
	
	if(!tok){ // Ne devrait pas arriver
		perror("Fichier de configuration non valide");
		exit(1);
	}
	
	while(tok != NULL){
		if (hostparlist){ /* Cas des autres boucles, insertion en queue */
			hostparlist->next = malloc(sizeof(struct Options));
			hostparlist = hostparlist->next;
		}else{ /* Cas de la première boucle, met à jour le pointeur global */
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