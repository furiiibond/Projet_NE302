#include <stdio.h>
#include "header_parser.h"

//Prototopo
void meth_trouve(HeaderStruct* headers,char* s, int l);

/*
	Cette fonction remplis la structure header avec les valeurs
	lues dans l'arbre de la requète parsée
*/
int traiter_Header(_Token *root, HeaderStruct* headers){
	_Token *req,*t,*ptr;
	int l;
	char *s;

	// Initialisation des headers optionels:
	memset(headers, 0, sizeof(HeaderStruct));
	/*
	headers->host.count = 0;
	headers->connection.keepAlive=0;
	headers->connection.close=0;
	headers->query.data=NULL;
	headers->msg_body.data=NULL;
	headers->msg_body.count=0;
	headers->referer.data=NULL;
	headers->referer.count=0;
	headers->contentType.data=NULL;
	headers->contentType.count=0;
	headers->contentLength.data=NULL;
	headers->contentLength.count=0;*/

	// Request Target [Obligatoire]
	req = searchTree(root, "request-line");
	t = searchTree(req->node, "absolute-path");
	headers->absolutePath.data = getElementValue(t->node, &l);
	headers->absolutePath.count = l;
	purgeElement(&t);

	// Query [Optionel]
	t = searchTree(req->node, "query");
	if(t){
		headers->query.data = getElementValue(t->node, &l);
		headers->query.count = l;
	}
	purgeElement(&t);


	// HTTP-version [Obligatoire]
	t = searchTree(req->node, "HTTP-version");
	s = getElementValue(t->node, &l);
	char tmp[l+1];
	strncpy(tmp,s,l);
	tmp[l]='\0';
	int a,b;
	sscanf(tmp,"HTTP/%1d.%1d",&a,&b);
	headers->httpVersion=10*a+b;
	purgeElement(&t);

	// Method [Obligatoire]
	t = searchTree(req->node, "method");
	s = getElementValue(t->node, &l);
	meth_trouve(headers, s, l);
	purgeElement(&t);
	purgeElement(&req);

	//Host-header [Optionnel]
	req = searchTree(root, "Host-header");
	if (req){
		t = searchTree(req->node, "host");
		headers->host.data = getElementValue(t->node, &l);
		headers->host.count = l;
		purgeElement(&t);
	}
	purgeElement(&req);

	//Connection-header [Optionnel]
	req = searchTree(root, "Connection-header");
	if (req){
		t = searchTree(req->node, "connection-option");
		ptr=t;
		while (ptr){
			s = getElementValue(ptr->node, &l);
			if (!strncasecmp(s,"keep-alive",10))
				headers->connection.keepAlive=1;
			else if(!strncasecmp(s,"close",5))
				headers->connection.close=1;
			else
				printf(RED"Unsuported connection-opiton: "NC"%.*s\n",l,s);
			ptr =ptr->next;
		}
		purgeElement(&t);
	}
	purgeElement(&req);

	//Accept [Optionnel]
	req = searchTree(root, "Accept-header");
	if (req){
		t = searchTree(req->node, " media-range");
		ptr=t;
		int i=0;
		while (ptr && i<10){
			headers->accept[i].data = getElementValue(ptr->node, (int *) &(headers->accept[i].count));
			ptr =ptr->next;
			i++;
		}
		purgeElement(&t);
	}
	purgeElement(&req);

	// Message Body [Optionel]
	req = searchTree(root, "message-body");
	if (req){
		headers->msg_body.data = getElementValue(req->node, &l);
		headers->msg_body.count = l-1;
	}
	purgeElement(&req);

	// Content Type [Optionel]
	req = searchTree(root, "Content-Type");
	if (req){
		headers->contentType.data = getElementValue(req->node, &l);
		headers->contentType.count = l;
	}
	purgeElement(&req);

	// Content Length [Optionel]
	req = searchTree(root, "Content-Length");
	if (req){
		headers->contentLength.data = getElementValue(req->node, &l);
		headers->contentLength.count = l;
		//headers->msg_body.count = sscanf(); //Juste au cas où
	}
	purgeElement(&req);

	// Referer [Optionel]
	req = searchTree(root, "Referer");
	if (req){
		headers->referer.data = getElementValue(req->node, &l);
		headers->referer.count = l;
	}
	purgeElement(&req);



	/* DEBBUG */
	printf("httpVersion = %d.%d\n",headers->httpVersion/10,headers->httpVersion%10);
	printf("Host:" SV_Fmt "\n", SV_Arg(headers->host));
	return OK;
}

int max(int x, int y){
    return (x > y ? x : y);
}

/* Trouve la meth
ode */
// Attention Dr White
void meth_trouve(HeaderStruct* headers,char* s, int l){
	headers->method = 0;
	// Utilisation du max pour match précisément
	// le nom de la méthode (pas G ni GETTA)
	if(!strncmp(s,"GET",max(3,l))){
		printf("GET\n");
		headers->method = GET;
	}else if(!strncmp(s,"HEAD",max(4,l))){
		printf("HEAD\n");
		headers->method = HEAD;
	}else if(!strncmp(s,"POST",max(4,l))){
		printf("POST\n");
		headers->method = POST;
	}else if(!strncmp(s,"PUT",max(3,l))){
		printf("PUT\n");
		headers->method = PUT;
	}else if(!strncmp(s,"DELETE",max(6,l))){
		printf("DELETE\n");
		headers->method = DELETE;
	}else if(!strncmp(s,"CONNECT",max(7,l))){
		printf("CONNECT\n");
		headers->method = CONNECT;
	}else if(!strncmp(s,"OPTIONS",max(7,l))){
		printf("OPTIONS\n");
		headers->method = OPTIONS;
	}else if(!strncmp(s,"TRACE",max(5,l))){
		printf("TRACE\n");
		headers->method = TRACE;
	}
}
