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
#include "api.h"

#define SV_IMPLEMENTATION

//
#include "annexe/global.h"
#include "modules/OptionsParser/option_parser.h"
#include "modules/RequestHandler/request_handler.h"


int send_file(unsigned int clientId, Fichier file);
int writeHeaders(unsigned int clientId, Header_List reponseHL);// DYNAMIC ALLOCATION
int writePHP(unsigned int clientId, Header_List *PHP_data);

/**
 * MAIN !!
 */
int main(/*int argc, char *argv[]*/)
{
	message *requete;
	HTML_Rep reponse;
	Fichier file;
	HeaderStruct headers;
  Header_List PHP_data;

	FillHostsParametres(); //Setup des options (see option_parser.h)
	load_gramm_rule(HTTP_RULES);// charge la grammaire HTTP


	/** DYNAMIC ALLOCATION
	Declaration d'une Header List
	 - Header initial */
	char init[100]="HTTP/1.1 ........................";
	Header_List reponseHL={
	    { 9, init },
		NULL
	};
	reponseHL.header.data[7]='5';
	/** ----------------------- */

	printf(CYN"#------------	Server Ready	------------#\n"NC);

	while ( 1 ) {
		// on attend la reception d'une requete HTTP, requete pointera vers une ressource allouée par librequest.
		if ((requete=getRequest(8080)) == NULL ) return -1;

		// Affichage de debug
		printf(RED"\n#########################################"GRN"\nDemande recue depuis le client %d\n"NC,requete->clientId);
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf(YEL"Contenu de la demande"NC"\n%.*s\n",requete->len,requete->buf);


		reponseHL.next = NULL;// DYNAMIC ALLOCATION

		int method = RequestHandler(requete, &headers, &reponse, &reponseHL, &file, &PHP_data);

		//HEADER response
		writeDirectClient(requete->clientId, reponse.content, reponse.len);
		printf(YEL"Contenu de la reponse"NC"\n%.*s\n",reponse.len,reponse.content);

		//writeHeaders(requete->clientId, reponseHL);// DYNAMIC ALLOCATION


		//Action à faire en fonction de la methode
		if(file.to_send){
				//Message Body (le fichier à transmettre)
				send_file(requete->clientId, file);
				printf("["BLU"Fichier"NC":"MAG"%s"NC"] %s\n\n",file.type,file.path);
		}
		/**if(champ_php->to_send){
			send php
			printf
			free ??
		}*/
    writePHP( requete->clientId, &PHP_data);

		endWriteDirectClient(requete->clientId);


		if (!headers.connection.keepAlive){
			printf(YEL"Shutding down connection\n"NC);
			requestShutdownSocket(requete->clientId);
		}

		printf("Free Request ___\n");
		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer...
		freeRequest(requete);
	}
	printf("exit\n");
	close_gramm_rule();
	return (1);
}

/** DYNAMIC ALLOCATION */
int writeHeaders(unsigned int clientId, Header_List reponseHL){
	//writeDirectClient(clientId, reponseHL.header.data, reponseHL.header.count);
	printf(MAG "DEBUG: %.*s", (int) reponseHL.header.count, reponseHL.header.data);
	Header_List *ptr;
	ptr = reponseHL.next;
	Header_List *ptr2;
	while(ptr){
		//writeDirectClient(clientId, ptr->header.data, ptr->header.count);
		printf("%.*s", (int) ptr->header.count, ptr->header.data);
		ptr2 = ptr->next;
		free(ptr->header.data);
		free(ptr);
		ptr = ptr2;
	}
	printf(NC "\n++++\n");
  //Termine la partie header avec un deuxième CRLF
  //writeDirectClient(requete->clientId, "\r\n", 2);
	return 0;
}

//TODO: Fix for large files ( >1.4G )
int send_file(unsigned int clientId, Fichier file){

	// if (access( file.path, F_OK )) return -1; // On a déjà checké normalement
	// Les if ne devraient pas être utile vu qu'on déjà vérifié tout.
	char * ptr;
    int len, fd;
	if (( fd= open(file.path, O_RDWR)) == -1) {
		perror("open: to_send alors que n'existe pas ??");
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

int writePHP(unsigned int clientId, Header_List *PHP_data){

  if(PHP_data->next==NULL) return -1;

	Header_List *ptr;
	ptr = PHP_data->next;
	Header_List *ptr2;
	while(ptr){
		writeDirectClient(clientId, ptr->header.data, ptr->header.count);
		printf(SV_Fmt, SV_Arg(ptr->header));
		ptr2 = ptr->next;
		free(ptr->header.data);
		free(ptr);
		ptr = ptr2;
	}
	printf(NC "\n++++\n");
  PHP_data->next=NULL;
	return 0;
}
