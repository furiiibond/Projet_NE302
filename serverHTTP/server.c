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
	// // this will declare internal type used by the parser
	// #include "annexe/httpparser.h"
#include "api.h"

//
#include "parametres.h"
#include "modules/OptionsParser/option_parser.h"
#include "modules/RequestHandler/request_handler.h"


//TODO: Fix for large files ( >1.4G )
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
		
	printf("write message body\n");
	writeDirectClient(clientId,ptr,len);
	
	close(fd);
	return 0;
}


int main(/*int argc, char *argv[]*/)
{
	message *requete;
	
	
	FillHostsParametres(); //Setup des options (see option_parser.h)
	load_gramm_rule(HTTP_RULES);// charge la grammaire HTTP
	
	printf(CYN"#------------	Server Ready	------------#\n"NC);
	
	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
		if ((requete=getRequest(8080)) == NULL ) return -1;

		// Affichage de debug
		printf(RED"#########################################"NC"\nDemande recue depuis le client %d\n",requete->clientId);
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf(YEL"Contenu de la demande"NC"\n%.*s\n",requete->len,requete->buf);
		
		
		
		HTML_Rep reponse;
		Fichier file;
		
		int method = RequestHandler(requete, &reponse, &file);
		
		//Reponse HTML
		writeDirectClient(requete->clientId, reponse.content, reponse.len);
		
		printf(YEL"Contenu de la reponse"NC"\n%.*s\n",reponse.len-4,reponse.content);
		
		//Action à faire en fonction de la methode
		switch(method){
			case GET:
				//Message Body (le fichier à transmettre)
				send_file(requete->clientId, file);
				printf("["BLU"Fichier"NC":"MAG"%s"NC"] %s\n\n",file.type,file.path);
				break;
		}
		
		
		
		
		
		
		
		
		endWriteDirectClient(requete->clientId);
		requestShutdownSocket(requete->clientId);
	// on ne se sert plus de requete a partir de maintenant, on peut donc liberer...
	freeRequest(requete);
	}
	printf("exit?\n");
	close_gramm_rule();
	return (1);
}
