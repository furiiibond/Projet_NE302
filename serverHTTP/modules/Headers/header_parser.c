#include <stdio.h>
#include "header_parser.h"

int traiter_Header(_Token *root, HeaderStruct* headers){
	_Token *req,*t,*ptr;
	int l;
	char *s;
	
	// TODO: penser à initialiser	
	
	// Request Target
	req = searchTree(root, "request-line");
	t = searchTree(req->node, "absolute-path");
	headers->absolutePath.data = getElementValue(t->node, (int *) &(headers->absolutePath.count));
	purgeElement(&t);
	// HTTP-version
	t = searchTree(req->node, "HTTP-version");
	s = getElementValue(t->node, &l);
	char tmp[l+1];
	strncpy(tmp,s,l);
	tmp[l]='\0';
	int a,b;
	sscanf(tmp,"HHTP/%d.%d",&a,&b);
	headers->httpVersion=10*a+b;
	purgeElement(&t);
	// Method
	t = searchTree(req->node, "method");
	s = getElementValue(t->node, &l);
	headers->method = 0;
	// pas beau mais nécessaire (peut-être cacher ça dans une fonction)
	if(!strncmp(s,"GET",3)){ //TODO fix avec un max
		printf("GET\n");
		headers->method = GET;
	}else if(!strncmp(s,"HEAD",4)){
		printf("HEAD\n");
		headers->method = HEAD;
	}else if(!strncmp(s,"POST",4)){
		printf("POST\n");
		headers->method = POST;
	}else if(!strncmp(s,"PUT",3)){
		printf("PUT\n");
		headers->method = PUT;
	}else if(!strncmp(s,"DELETE",6)){
		printf("DELETE\n");
		headers->method = DELETE;
	}else if(!strncmp(s,"CONNECT",7)){
		printf("CONNECT\n");
		headers->method = CONNECT;
	}else if(!strncmp(s,"OPTIONS",7)){
		printf("OPTIONS\n");
		headers->method = OPTIONS;
	}else if(!strncmp(s,"TRACE",5)){
		printf("TRACE\n");
		headers->method = TRACE;
	}
	purgeElement(&t);
	purgeElement(&req);
	
	//Host-header
	req = searchTree(root, "Host-header");
	t = searchTree(req->node, "host");
	headers->host.data = getElementValue(t->node, (int *) &(headers->host.count));
	purgeElement(&req);
	purgeElement(&t);
	
	//Connection-header:
	req = searchTree(root, "Connection-header");
	t = searchTree(req->node, "connection-option");
	ptr=t;
	headers->connection.keepAlive=0;
	headers->connection.close=0;
	while (ptr){
		s = getElementValue(ptr->node, &l);
		if (!strncmp(s,"keep-alive",10))
			headers->connection.keepAlive=1;
		else if(!strncmp(s,"close",5))
			headers->connection.close=1;
		else 
			printf(RED"Unsuported connection-opiton: "NC"%.*s\n",l,s);
		ptr =ptr->next;
	}
	purgeElement(&req);
	purgeElement(&t);
	if(headers->connection.keepAlive && headers->connection.close)
		return ERR_400;
		
	return OK;
}