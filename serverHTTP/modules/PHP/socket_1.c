#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "socket_1.h"
#include "fastcgi.h"

// =========================================================================================================== //
void writeSocket(int fd,FCGI_Header *h,unsigned int len)
{
	int w;

	h->contentLength=htons(h->contentLength);
	h->requestId=htons(h->requestId);
	h->reserved=0;


	while (len) {
	// try to write
		do {
			w = write(fd, h, len);
		} while (w == -1 && errno == EINTR);
	len-=w;
	}
}

// =========================================================================================================== //
void writeLen(int len, char **p) {
	if (len > 0x7F ) {
		*((*p)++)=(len>>24)&0x7F;
		*((*p)++)=(len>>16)&0xFF;
		*((*p)++)=(len>>8)&0xFF;
		*((*p)++)=(len)&0xFF;
	} else *((*p)++)=(len)&0x7F;
}

// =========================================================================================================== //
int addNameValuePair(FCGI_Header *h,char *name,char *value)
{
	char *p;
	unsigned int nameLen=0,valueLen=0;

	if (name) nameLen=strlen(name);
	if (value) valueLen=strlen(value);

	if ((nameLen > FASTCGIMAXNVPAIR) || (valueLen > FASTCGIMAXNVPAIR) ) return -1;
	if ((h->contentLength+((nameLen>0x7F)?4:1)+((valueLen>0x7F)?4:1)) > FASTCGILENGTH ) return -1;

	p=(h->contentData)+h->contentLength;
	writeLen(nameLen,&p);
	writeLen(valueLen,&p);
	strncpy(p,name,nameLen);
	p+=nameLen;
	if (value) strncpy(p,value,valueLen);
	h->contentLength+=nameLen+((nameLen>0x7F)?4:1);
	h->contentLength+=valueLen+((valueLen>0x7F)?4:1);
	return 0;
}
// =========================================================================================================== //

void sendGetValue(int fd)
{
FCGI_Header h;

	h.version=FCGI_VERSION_1;
	h.type=FCGI_GET_VALUES;
	h.requestId=FCGI_NULL_REQUEST_ID;
	h.contentLength=0;
	h.paddingLength=0;
	addNameValuePair(&h,FCGI_MAX_CONNS,NULL);
	addNameValuePair(&h,FCGI_MAX_REQS,NULL);
	addNameValuePair(&h,FCGI_MPXS_CONNS,NULL);
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}
// =========================================================================================================== //
void sendBeginRequest(int fd,unsigned short requestId,unsigned short role,unsigned char flags)
{
FCGI_Header h={0};
FCGI_BeginRequestBody *begin;

	h.version=FCGI_VERSION_1;
	h.type=FCGI_BEGIN_REQUEST;
	h.requestId=requestId;
	h.contentLength=sizeof(FCGI_BeginRequestBody);
	h.paddingLength=0;
	begin=(FCGI_BeginRequestBody *)&(h.contentData);
	begin->role=htons(role);
	begin->flags=flags;
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}
// =========================================================================================================== //
void sendAbortRequest(int fd,unsigned short requestId)
{
FCGI_Header h;

	h.version=FCGI_VERSION_1;
	h.type=FCGI_ABORT_REQUEST;
	h.requestId=requestId;
	h.contentLength=0;
	h.paddingLength=0;
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}
#define sendStdin(fd,id,stdin,len) sendWebData(fd,FCGI_STDIN,id,stdin,len)
#define sendData(fd,id,data,len) sendWebData(fd,FCGI_DATA,id,data,len)
#define sendParam(fd,id,data,len) sendWebData(fd,FCGI_PARAMS,id,data,len)
//============================================================================================================ //

void sendWebData(int fd,unsigned char type,unsigned short requestId,char *data,unsigned int len)
{
FCGI_Header h;

	if (len > FASTCGILENGTH) return ;

	h.version=FCGI_VERSION_1;
	h.type=type;
	h.requestId=requestId;
	h.contentLength=len;
	h.paddingLength=0;
	memcpy(h.contentData,data,len);
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
}

// =========================================================================================================== //
static int createSocket(int port)
{
	int fd;
	struct sockaddr_in serv_addr;
	//int enable = 1; //Commenté par nous pour enlever le warning

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creation failed\n");
		return (-1);
	}

	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	inet_aton("127.0.0.1",&serv_addr.sin_addr);
	serv_addr.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect failed\n");
		return (-1);
	}

	return fd;
}


void init_param(FCGI_Header *header_param, struct Options* site_param, HeaderStruct* headers, Fichier* file);

// =========================================================================================================== //
int executePHP(struct Options* site_param, HeaderStruct* headers, Fichier* file, HTML_Rep* reponse, Header_List* PHP_data)
{
	// Construction du paquet FCGI_PARAMS
	FCGI_Header header_param;
	init_param(&header_param, site_param, headers, file);


// =========== Envoie requête PHP =>
	int fd;
	fd=createSocket(9000);

	sendBeginRequest(fd,10,FCGI_RESPONDER,FCGI_KEEP_CONN);

	writeSocket(fd, &header_param, FCGI_HEADER_SIZE + header_param.contentLength + header_param.paddingLength);
	sendParam(fd,10,NULL,0);

	if(headers->method == POST)
		sendStdin(fd,10,headers->msg_body.data,headers->msg_body.count);
	sendStdin(fd,10,NULL,0);


// =========== Ecoute réponse PHP =>
	FCGI_Header h;
	h.type=0;

	Header_List *bak = PHP_data;
	Header_List *ptr;
	char* tab;

	while(h.type != FCGI_END_REQUEST){

			//Lit headers
			read(fd,&h,FCGI_HEADER_SIZE);

			//Conversition Little Endian
			h.requestId = htons(h.requestId);
			h.contentLength = htons(h.contentLength);

			int n=h.contentLength+h.paddingLength;
			int m=0;
			int k;

			/* Si c'est un paquet STDOUT on ajoute le Contenu
			à la liste chainée PHP_data */
			if(h.type == FCGI_STDOUT){

				ptr = malloc (sizeof(Header_List));
				tab = malloc (sizeof(char)*FASTCGILENGTH);

				while(m<n){
					k= read(fd,tab+m,n-m);
					if(k==-1) {printf("break"); break;}
					m+=k;
					printf("%d ",m);
				}
				puts("");
				printf("]contentData:%.*s\n",10,tab);

				ptr->header.data = tab;
				ptr->header.count = h.contentLength;

				bak->next=ptr;
				bak=ptr;
			}
			// Sinon on ignore le contenu
			else{
				while(m<n){
					k= read(fd,h.contentData+m,n-m);
					if(k==-1) {printf("break"); break;}
					m+=k;
				}
				//h.contentData[n]='\0';
				//printf("DEBUG:%s",h.contentData);
			}

	}
	ptr->next=NULL;

// Ferme la Connexion
	close(fd);


// =========== Compte les charactères pour Content-Length =>
	int count = 0;
	int count_flag = 0;
	ptr=PHP_data->next;
	while(ptr){
		//printf("]"SV_Fmt"\n",SV_Arg(ptr->header));
		//Compte le Content-length
		if (count_flag == 0){
			for(size_t i=0; i<ptr->header.count; i++){
			if(count_flag == 0) {
			// Quand on arrive au Double CRLF (début du message body)
				if(!strncmp(ptr->header.data+i,"\r\n\r\n",4)){
					i+=3; count_flag=1;
				}
			}else
				count++;
		}}
		else
			count+=ptr->header.count;

		ptr=ptr->next;
	}
	//printf("Content-length:%d\n",count);


// =========== Construction de la réponse =>
	reponse->len = snprintf(reponse->content, HEADER_LEN_MAX, SERV_VERSION(headers->httpVersion) );
	reponse->len += snprintf(reponse->content+reponse->len, HEADER_LEN_MAX,
		" 200 OK\r\nContent-length:%d\r\n",count);
	//keep-alive
	if(headers->connection.keepAlive)
		reponse->len += snprintf(reponse->content+reponse->len,
			HEADER_LEN_MAX-reponse->len,
			"Connection: keep-alive\r\nKeep-Alive: timeout=5, max=2\r\n");
	else
		reponse->len += snprintf(reponse->content+reponse->len,
		HEADER_LEN_MAX-reponse->len,
		"Connection: close\r\n");

file->to_send=0;

return OK;
}


void init_param(FCGI_Header *header_param,struct Options* site_param, HeaderStruct* headers, Fichier* file){
	header_param->version=FCGI_VERSION_1;
	header_param->type=FCGI_PARAMS;
	header_param->requestId=10;
	header_param->contentLength=0;
	header_param->paddingLength=0;
	//copy headers->host String_View into Null Terminated String char[] buffer
	char tmp[FASTCGIMAXNVPAIR];
	char tmp2[FASTCGIMAXNVPAIR];
	char pwd[FASTCGIMAXNVPAIR];
	getcwd(pwd, FASTCGIMAXNVPAIR);

	strncpy(tmp,headers->host.data,headers->host.count);
	tmp[headers->host.count]='\0';
	addNameValuePair(header_param,"HTTP_HOST", tmp);
	sprintf(tmp,"%d",SERVERPORT);
	addNameValuePair(header_param,"SERVER_PORT", tmp);
	addNameValuePair(header_param,"REQUEST_SCHEME", "http");
	addNameValuePair(header_param,"SERVER_ADMIN", "root@localhost");

	if(site_param->DocumentRoot[0] == '/'){
		strcpy(tmp,site_param->DocumentRoot);
		addNameValuePair(header_param,"DOCUMENT_ROOT", site_param->DocumentRoot);
	}
	else{
		addNameValuePair(header_param,"CONTEXT_DOCUMENT_ROOT", site_param->DocumentRoot);
		addNameValuePair(header_param,"CONTEXT_PREFIX", pwd);

		strcpy(tmp,pwd);
		strcat(tmp,"/");
		strcat(tmp,file->path);
	}


	char scriptPrefix[] = "proxy:fcgi://127.0.0.1:9000/";   // Valeur par défaut
	strcpy(tmp2,scriptPrefix);
	strcat(tmp2,tmp);
	addNameValuePair(header_param,"SCRIPT_FILENAME", tmp2);  //is it the right path ?

	addNameValuePair(header_param,"GATEWAY_INTERFACE", "CGI/1.1");
	addNameValuePair(header_param,"SERVER_PROTOCOL", "HTTP/1.1");


	switch(headers->method){
		case GET:
			strcpy(tmp2,"GET");
		break;
		case POST:
				strncpy(tmp2,headers->referer.data,headers->referer.count); tmp2[headers->referer.count]='\0';
			addNameValuePair(header_param,"HTTP_REFERER", tmp2);
				strncpy(tmp2,headers->contentType.data,headers->contentType.count); tmp2[headers->contentType.count]='\0';
			addNameValuePair(header_param,"CONTENT_TYPE", tmp2);
				strncpy(tmp2,headers->contentLength.data,headers->contentLength.count); tmp2[headers->contentType.count]='\0';
			addNameValuePair(header_param,"CONTENT_LENGTH", tmp2);

			strcpy(tmp2,"POST");
		break;
		case HEAD:
			strcpy(tmp2,"HEAD");
		break;
	}
	addNameValuePair(header_param,"REQUEST_METHOD", tmp2);

	if(headers->query.count){
		strncpy(tmp2,headers->query.data,headers->query.count);
		tmp2[headers->query.count]='\0';
	}else
		tmp2[0]=0;
	addNameValuePair(header_param,"QUERY_STRING", tmp2);
	addNameValuePair(header_param,"REQUEST_URI", file->path+file->middle);
	addNameValuePair(header_param,"SCRIPT_NAME", file->path+file->last_slash); // TODO : check this
	
}
