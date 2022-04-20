
#include <string.h>

#include "parametres.h"

struct Options* HostsParametres;


int FillHostsParametres(void){
	
	HostsParametres = malloc(sizeof(struct Options));
	strcpy(HostsParametres.ServerName, "Sitecool.com");
	strcpy(HostsParametres.DocumentRoot, "/var/www");
	
	HostsParametres.next = malloc(sizeof(struct Options));
	strcpy(HostsParametres.ServerName, "NumeroDos.tv");
	strcpy(HostsParametres.DocumentRoot, "/home/userir/2");
	
	HostsParametres.next.next = null;
	
	return 0;
}