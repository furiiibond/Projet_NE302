#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>	// open
#include <sys/stat.h>	// open
#include <fcntl.h>		// open
#include <unistd.h>		// close
 #include <sys/mman.h>	// mmap

// for librequest
#include "request.h"

// for parser

// this will declare internal type used by the parser
#include "httpparser.h"
#include "api.h"

//
#include "parametres.h"
#include "option_parser.h"
#include "request_handler.h"

int send_file(unsigned int clientId, Fichier file){
	
	if (access( file.path, F_OK )) return -1;
	
	char * ptr;
    int len, fd;
	if (( fd= open(file.path, O_RDWR)) == -1) {
		perror("open");
		exit(1);
	}
	struct stat st;
	if (fstat(fd, &st) == -1)
		exit(1);
	if ((ptr = mmap(NULL, st.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0)) == NULL)
		exit(1);
	len = st.st_size;
		
	writeDirectClient(clientId,ptr,len);
	
	close(fd);
	return 0;
}


int main(/*int argc, char *argv[]*/)
{
	message *requete;
	
	
	FillHostsParametres(); //Setup des options (see option_parser.h)
	load_gramm_rule(HTTP_RULES);// charge la grammaire HTTP
	
	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
		if ((requete=getRequest(8080)) == NULL ) return -1;

		// Affichage de debug
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId);
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande\n%.*s\n\n",requete->len,requete->buf);
		
		
		
		HTML_Rep reponse;
		Fichier file;
		
		RequestHandler(requete, &reponse, &file);
		
		//Reponse HTML
		writeDirectClient(requete->clientId, reponse.content, reponse.len);
		
		//Message Body (le fichier à transmettre)
		send_file( requete->clientId, file);
		
		
		
		
		
		
		
		
		
		endWriteDirectClient(requete->clientId);
		requestShutdownSocket(requete->clientId);
	// on ne se sert plus de requete a partir de maintenant, on peut donc liberer...
	freeRequest(requete);
	}
	printf("exit?\n");
	close_gramm_rule();
	return (1);
}
