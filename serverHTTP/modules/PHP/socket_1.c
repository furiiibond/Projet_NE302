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
	//h->paddingLength=htons(h->paddingLength);


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
	h.requestId=htons(FCGI_NULL_REQUEST_ID);
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
FCGI_Header h;
FCGI_BeginRequestBody *begin;

	h.version=FCGI_VERSION_1;
	h.type=FCGI_BEGIN_REQUEST;
	h.requestId=htons(requestId);
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
	h.type=htons(FCGI_ABORT_REQUEST);
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
	h.requestId=htons(requestId);
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







// =========================================================================================================== //
int executePHP(struct Options* site_param, HeaderStruct* headers, Fichier* file, HTML_Rep* reponse, Header_List* PHP_data)
{
	FCGI_Header header_param;
	header_param.version=FCGI_VERSION_1;
	header_param.type=FCGI_PARAMS;
	header_param.requestId=htons(10);
	//convert headers->host from String_Value to char*
	char tmp[FASTCGIMAXNVPAIR];
	char pwd[FASTCGIMAXNVPAIR];
	getcwd(pwd, FASTCGIMAXNVPAIR);

	strncpy(tmp,headers->host.data,headers->host.count);
	tmp[headers->host.count]='\0';
	addNameValuePair(&header_param,"HTTP_HOST", tmp);
	addNameValuePair(&header_param,"SERVER_SOFTWARE", "Apache/2.4.53 (Debian)");
	addNameValuePair(&header_param,"SERVER_NAME", "localhost");
	addNameValuePair(&header_param,"SERVER_ADDR", "localhost");
	addNameValuePair(&header_param,"SERVER_PORT", "8080");
	addNameValuePair(&header_param,"REMOTE_ADDR", "localhost");
	addNameValuePair(&header_param,"DOCUMENT_ROOT", site_param->DocumentRoot);
	addNameValuePair(&header_param,"REQUEST_SCHEME", "http");
	addNameValuePair(&header_param,"CONTEXT_DOCUMENT_ROOT", site_param->DocumentRoot);
	strcpy(tmp,pwd);
	if(site_param->DocumentRoot[0] != '\0')strcat(tmp,"/");
	strcat(tmp,site_param->DocumentRoot);
	addNameValuePair(&header_param,"CONTEXT_PREFIX", tmp);
	addNameValuePair(&header_param,"SERVER_ADMIN", "root@localhost");
	char scriptPrefix[] = "proxy:fcgi://127.0.0.1:9000/";   // TODO : change this
	//concatenate the scriptFileName with the DocumentRoot and the file->path
	strcpy(tmp,scriptPrefix);
	strcat(tmp,pwd);
	if(site_param->DocumentRoot[0] != '\0')strcat(tmp,"/");
	strcat(tmp,site_param->DocumentRoot);
	addNameValuePair(&header_param,"SCRIPT_FILENAME", tmp);  //is it the right path ?
	//addNameValuePair(&header_param,"REMOTE_PORT", "56842");  //TODO an other one Dj Khaled
	addNameValuePair(&header_param,"GATEWAY_INTERFACE", "CGI/1.1");
	addNameValuePair(&header_param,"SERVER_PROTOCOL", "HTTP/1.1");
	addNameValuePair(&header_param,"REQUEST_METHOD", "GET");
	//addNameValuePair(&header_param,"QUERY_STRING", "");
	strncpy(tmp,headers->absolutePath.data,headers->absolutePath.count);
	tmp[headers->absolutePath.count]='\0';
	addNameValuePair(&header_param,"REQUEST_URI", tmp);
	//addNameValuePair(&header_param,"SCRIPT_NAME", absolutePath); // TODO : check this

	int fd;
	fd=createSocket(9000);

	sendBeginRequest(fd,10,FCGI_RESPONDER,FCGI_KEEP_CONN);
	writeSocket(fd, &header_param, FCGI_HEADER_SIZE + header_param.contentLength + header_param.paddingLength);
	sendParam(fd,10,NULL,0);
	sendStdin(fd,10,NULL,0);


	/*
	*	Header of
	*/
	FCGI_Header h;
	h.type=0;

	while(h.type != FCGI_END_REQUEST){

			//Lit headers
			read(fd,&h,FCGI_HEADER_SIZE);
			//Conversition Little Endian
			h.requestId = htons(h.requestId);
			h.contentLength = htons(h.contentLength);
			printf("version:%d\n",h.version);
			printf("type:%d\n",h.type);
			printf("requestID:%d\n",h.requestId);
			printf("contentLength:%d\n",h.contentLength);
			printf("paddingLength:%d\n",h.paddingLength);
			int n=h.contentLength+h.paddingLength;
			int m=0;
			int k;
			while(m<n){
				k= read(fd,&(h.contentData)+m,n-m);
				if(k==-1) {printf("break"); break;}
				m+=k;
				printf("%d ",m);
			}
			puts("");
			printf("contentData:%.*s\n",10,h.contentData);
	}

	close(fd);


return OK;
}


/**
ExecutePHP(struct Options* site_param, Fichier* file, HTML_Rep* reponse, Header_List* liste)
{
	int fd;
	fd=createSocket(9000);

char param1[]=
"\x11\x01\x70\x72\x6f\x78\x79\x2d\x6e\x6f\x6b\x65\x65\x70\x61\x6c" \
"\x69\x76\x65\x31\x09\x09\x48\x54\x54\x50\x5f\x48\x4f\x53\x54\x31" \
"\x32\x37\x2e\x30\x2e\x30\x2e\x31\x0f\x44\x48\x54\x54\x50\x5f\x55" \
"\x53\x45\x52\x5f\x41\x47\x45\x4e\x54\x4d\x6f\x7a\x69\x6c\x6c\x61" \
"\x2f\x35\x2e\x30\x20\x28\x58\x31\x31\x3b\x20\x4c\x69\x6e\x75\x78" \
"\x20\x78\x38\x36\x5f\x36\x34\x3b\x20\x72\x76\x3a\x37\x38\x2e\x30" \
"\x29\x20\x47\x65\x63\x6b\x6f\x2f\x32\x30\x31\x30\x30\x31\x30\x31" \
"\x20\x46\x69\x72\x65\x66\x6f\x78\x2f\x37\x38\x2e\x30\x0b\x4a\x48" \
"\x54\x54\x50\x5f\x41\x43\x43\x45\x50\x54\x74\x65\x78\x74\x2f\x68" \
"\x74\x6d\x6c\x2c\x61\x70\x70\x6c\x69\x63\x61\x74\x69\x6f\x6e\x2f" \
"\x78\x68\x74\x6d\x6c\x2b\x78\x6d\x6c\x2c\x61\x70\x70\x6c\x69\x63" \
"\x61\x74\x69\x6f\x6e\x2f\x78\x6d\x6c\x3b\x71\x3d\x30\x2e\x39\x2c" \
"\x69\x6d\x61\x67\x65\x2f\x77\x65\x62\x70\x2c\x2a\x2f\x2a\x3b\x71" \
"\x3d\x30\x2e\x38\x14\x0e\x48\x54\x54\x50\x5f\x41\x43\x43\x45\x50" \
"\x54\x5f\x4c\x41\x4e\x47\x55\x41\x47\x45\x65\x6e\x2d\x55\x53\x2c" \
"\x65\x6e\x3b\x71\x3d\x30\x2e\x35\x14\x0d\x48\x54\x54\x50\x5f\x41" \
"\x43\x43\x45\x50\x54\x5f\x45\x4e\x43\x4f\x44\x49\x4e\x47\x67\x7a" \
"\x69\x70\x2c\x20\x64\x65\x66\x6c\x61\x74\x65\x0f\x0a\x48\x54\x54" \
"\x50\x5f\x43\x4f\x4e\x4e\x45\x43\x54\x49\x4f\x4e\x6b\x65\x65\x70" \
"\x2d\x61\x6c\x69\x76\x65\x1e\x01\x48\x54\x54\x50\x5f\x55\x50\x47" \
"\x52\x41\x44\x45\x5f\x49\x4e\x53\x45\x43\x55\x52\x45\x5f\x52\x45" \
"\x51\x55\x45\x53\x54\x53\x31\x12\x09\x48\x54\x54\x50\x5f\x43\x41" \
"\x43\x48\x45\x5f\x43\x4f\x4e\x54\x52\x4f\x4c\x6d\x61\x78\x2d\x61" \
"\x67\x65\x3d\x30\x04\x46\x50\x41\x54\x48\x2f\x75\x73\x72\x2f\x6c" \
"\x6f\x63\x61\x6c\x2f\x73\x62\x69\x6e\x3a\x2f\x75\x73\x72\x2f\x6c" \
"\x6f\x63\x61\x6c\x2f\x62\x69\x6e\x3a\x2f\x75\x73\x72\x2f\x73\x62" \
"\x69\x6e\x3a\x2f\x75\x73\x72\x2f\x62\x69\x6e\x3a\x2f\x73\x62\x69" \
"\x6e\x3a\x2f\x62\x69\x6e\x3a\x2f\x73\x6e\x61\x70\x2f\x62\x69\x6e" \
"\x10\x46\x53\x45\x52\x56\x45\x52\x5f\x53\x49\x47\x4e\x41\x54\x55" \
"\x52\x45\x3c\x61\x64\x64\x72\x65\x73\x73\x3e\x41\x70\x61\x63\x68" \
"\x65\x2f\x32\x2e\x34\x2e\x35\x33\x20\x28\x44\x65\x62\x69\x61\x6e" \
"\x29\x20\x53\x65\x72\x76\x65\x72\x20\x61\x74\x20\x31\x32\x37\x2e" \
"\x30\x2e\x30\x2e\x31\x20\x50\x6f\x72\x74\x20\x38\x30\x3c\x2f\x61" \
"\x64\x64\x72\x65\x73\x73\x3e\x0a\x0f\x16\x53\x45\x52\x56\x45\x52" \
"\x5f\x53\x4f\x46\x54\x57\x41\x52\x45\x41\x70\x61\x63\x68\x65\x2f" \
"\x32\x2e\x34\x2e\x35\x33\x20\x28\x44\x65\x62\x69\x61\x6e\x29\x0b" \
"\x09\x53\x45\x52\x56\x45\x52\x5f\x4e\x41\x4d\x45\x31\x32\x37\x2e" \
"\x30\x2e\x30\x2e\x31\x0b\x09\x53\x45\x52\x56\x45\x52\x5f\x41\x44" \
"\x44\x52\x31\x32\x37\x2e\x30\x2e\x30\x2e\x31\x0b\x02\x53\x45\x52" \
"\x56\x45\x52\x5f\x50\x4f\x52\x54\x38\x30\x0b\x09\x52\x45\x4d\x4f" \
"\x54\x45\x5f\x41\x44\x44\x52\x31\x32\x37\x2e\x30\x2e\x30\x2e\x31" \
"\x0d\x0d\x44\x4f\x43\x55\x4d\x45\x4e\x54\x5f\x52\x4f\x4f\x54\x2f" \
"\x76\x61\x72\x2f\x77\x77\x77\x2f\x68\x74\x6d\x6c\x0e\x04\x52\x45" \
"\x51\x55\x45\x53\x54\x5f\x53\x43\x48\x45\x4d\x45\x68\x74\x74\x70" \
"\x0e\x00\x43\x4f\x4e\x54\x45\x58\x54\x5f\x50\x52\x45\x46\x49\x58" \
"\x15\x0d\x43\x4f\x4e\x54\x45\x58\x54\x5f\x44\x4f\x43\x55\x4d\x45" \
"\x4e\x54\x5f\x52\x4f\x4f\x54\x2f\x76\x61\x72\x2f\x77\x77\x77\x2f" \
"\x68\x74\x6d\x6c\x0c\x13\x53\x45\x52\x56\x45\x52\x5f\x41\x44\x4d" \
"\x49\x4e\x77\x65\x62\x6d\x61\x73\x74\x65\x72\x40\x6c\x6f\x63\x61" \
"\x6c\x68\x6f\x73\x74\x0f\x32\x53\x43\x52\x49\x50\x54\x5f\x46\x49" \
"\x4c\x45\x4e\x41\x4d\x45\x70\x72\x6f\x78\x79\x3a\x66\x63\x67\x69" \
"\x3a\x2f\x2f\x31\x32\x37\x2e\x30\x2e\x30\x2e\x31\x3a\x39\x30\x30" \
"\x30\x2f\x2f\x76\x61\x72\x2f\x77\x77\x77\x2f\x68\x74\x6d\x6c\x2f" \
"\x69\x6e\x66\x6f\x2e\x70\x68\x70\x0b\x05\x52\x45\x4d\x4f\x54\x45" \
"\x5f\x50\x4f\x52\x54\x35\x36\x38\x34\x32\x11\x07\x47\x41\x54\x45" \
"\x57\x41\x59\x5f\x49\x4e\x54\x45\x52\x46\x41\x43\x45\x43\x47\x49" \
"\x2f\x31\x2e\x31\x0f\x08\x53\x45\x52\x56\x45\x52\x5f\x50\x52\x4f" \
"\x54\x4f\x43\x4f\x4c\x48\x54\x54\x50\x2f\x31\x2e\x31\x0e\x03\x52" \
"\x45\x51\x55\x45\x53\x54\x5f\x4d\x45\x54\x48\x4f\x44\x47\x45\x54" \
"\x0c\x00\x51\x55\x45\x52\x59\x5f\x53\x54\x52\x49\x4e\x47\x0b\x09" \
"\x52\x45\x51\x55\x45\x53\x54\x5f\x55\x52\x49\x2f\x69\x6e\x66\x6f" \
"\x2e\x70\x68\x70\x0b\x09\x53\x43\x52\x49\x50\x54\x5f\x4e\x41\x4d" \
"\x45\x2f\x69\x6e\x66\x6f\x2e\x70\x68\x70";
int len=1002;

	sendBeginRequest(fd,10,FCGI_RESPONDER,FCGI_KEEP_CONN);
	sendParam(fd,10,param1,len);
	sendParam(fd,10,NULL,0);
	sendStdin(fd,10,NULL,0);


		//sendBeginRequest(fd,10,FCGI_RESPONDER,FCGI_KEEP_CONN);
		//sendStdin(fd,10,argv[1],strlen(argv[1]));
		//sendData(fd,10,argv[1],strlen(argv[1]));
		//sendStdin(fd,10,argv[1],0);


	FCGI_Header h;
	h.type=0;


while(h.type != FCGI_END_REQUEST){

		//Lit headers
		read(fd,&h,FCGI_HEADER_SIZE);
		//Conversition Little Endian
		h.requestId = htons(h.requestId);
		h.contentLength = htons(h.contentLength);
		printf("version:%d\n",h.version);
		printf("type:%d\n",h.type);
		printf("requestID:%d\n",h.requestId);
		printf("contentLength:%d\n",h.contentLength);
		printf("paddingLength:%d\n",h.paddingLength);
		int n=h.contentLength+h.paddingLength;
		int m=0;
		int k;
		while(m<n){
			k= read(fd,&(h.contentData)+m,n-m);
			if(k==-1) {printf("break"); break;}
			m+=k;
			printf("%d ",m);
		}
		puts("");
		printf("contentData:%.*s\n",10,h.contentData);
}

close(fd);



}*/
