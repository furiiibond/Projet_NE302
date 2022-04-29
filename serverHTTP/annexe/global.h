#ifndef GLOBALH
#define GLOBALH


#include "../parametres.h"

#include "sv.h"


//Prof
#define ERROR "HTTP/1.0 400 SUCKA\r\nContent-Type: text/plain\r\n\r\n\ Error 400\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n Hey Bro why did you send me this:\r\n"

//CONSTANTES
#define TRUE 1
#define FALSE 0

typedef struct {
	char path[PATH_LEN_MAX];
	char type[TYPE_LEN_MAX];
	long length;
} Fichier;

typedef struct {
	char content[HEADER_LEN_MAX];
	int len;
} HTML_Rep;

struct conect {
	int keepAlive;
	int close;
};

typedef struct {
  int httpVersion;
  int method;
  String_View host;
  struct conect connection;
  String_View absolutePath;
} HeaderStruct;



// Pour l'option-parser
struct Options {
	char ServerName[HOST_LEN_MAX];
	char DocumentRoot[PATH_LEN_MAX];
	
	
	struct Options* next;
};
extern struct Options* HostsParametres;
/*
	L'intéret est de pouvoir parser le fichier de config serveur, puis créer une liste
	chainée où chaque maillon contiendra les informations d'un site en particulier.
	Ces infos sont utiles pour traiter les requetes (ex:GET)
	que ce soit en version 1.0 (DocumentRoot), ou 1.1 (ServerName).
*/




/* Nombre */
#define GET		1
#define HEAD	2
#define POST	3
#define PUT		4
#define DELETE	5
#define CONNECT	6
#define OPTIONS	7
#define TRACE	8

/* Constantes for Request Handler*/
#define OK 0
#define ERR_400 -1
#define ERR_404 -2
#define ERR_405 -3
#define ERR_500 -4
#define ERR_505 -5
// #define ERR_HOST_HEADER_MISSING
// #define ERR_NON_EXISTING_HOST






#endif