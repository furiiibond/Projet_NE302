#ifndef GLOBALH
#define GLOBALH

#include "../parametres.h"
#include "sv.h" // Librairy StringView

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

/** DYNAMIC ALLOCATION ---
	NOUVEAU Type pour la réponse
	Liste chainée de Header (String_View)
	(Il faut malloc de l'espace puis le free)*/
typedef struct hl {
	String_View header;
	struct hl *next;
} Header_List;
/** ---------------------- */

/*
	Structure contennant les headers supportés par notre serveur.
	Permet d'avoir un accès plus facile à ces données que de chercher
	dans la requète parsée à chaque fois.
*/
struct connect {
	int keepAlive;
	int close;
};
typedef struct {
  int httpVersion;
  int method;
  String_View host;
  struct connect connection;
  String_View absolutePath;
  String_View accept[10];
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

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


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
#define ERR_403 -2
#define ERR_404 -3
#define ERR_405 -4
#define ERR_414 -5
#define ERR_500 -6
#define ERR_501 -7
#define ERR_505 -8




#endif