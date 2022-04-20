#include <stdlib.h>
#include <string.h>

#include "parametres.h"
#include "option_parser.h"

struct Options* HostsParametres;


int FillHostsParametres(void){
	
	HostsParametres = malloc(sizeof(struct Options));
	strcpy(HostsParametres->ServerName, "Sitecool.com");
	strcpy(HostsParametres->DocumentRoot, "/mnt/c/Users/J/OneDrive/3A/Cours_S2/Projet_NE302/Projet_NE302/www");
	
	HostsParametres->next = malloc(sizeof(struct Options));
	strcpy(HostsParametres->ServerName, "NumeroDos.tv");
	strcpy(HostsParametres->DocumentRoot, "/home/userir/2");
	
	HostsParametres->next->next = NULL;
	
	return 0;
}