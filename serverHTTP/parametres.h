#ifndef PARAMETRESH
#define PARAMETRESH

#include "color.h"

//Prof
#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\nHey Bro why did you send me this:\r\n"

//CONSTANTES
#define TRUE 1
#define FALSE 0



/* Les règles de grammaires par défault*/
#define HTTP_RULES "fullrfc.abnf","HTTP-message"


#define PATH_LEN_MAX 256
#define TYPE_LEN_MAX 40
typedef struct {
	char path[PATH_LEN_MAX];
	int length;
	char type[TYPE_LEN_MAX];
} Fichier;

#define HEADER_LEN_MAX 1000
typedef struct {
	char content[HEADER_LEN_MAX];
	int len;
} HTML_Rep;



/**
 \Répartiton des paramètres:
	-La page par défault est définie on a per directory basis in the .htaccess file
		otherwise it's index.html
	-Les paramètres du site sont définis globalement dans $(SERV_CONFIG_PATH)/$(SERV_CONFIG)
		If options are missing there's no default, should raise an error
*/
//#define SERV_CONFIG_PATH "."
#define SERV_CONFIG "server.conf"
#define CONF_RULES "config.abnf","Config"




//A faire: structure contenant les différentes options par site (pour liste chainée)
// IMPORTANT choisir si on utilise le style apache2 ou le notre
//exemple tiré de 000-default.conf (apache2)
#define HOST_LEN_MAX 50
struct Options {
	char ServerName[HOST_LEN_MAX];
	char DocumentRoot[PATH_LEN_MAX];
	
	
	struct Options* next;
};
extern struct Options* HostsParametres;
/*
	L'intéret sera de pouvoir parser le fichier de config serveur, puis créer une liste
	chainée où chaque maillon contiendra les informations d'un site en particulier.
	Ces infos seront utiles pour traiter les requetes (ex:GET)
	que ce soit en version 1.0 (DocumentRoot), ou 1.1 (ServerName).
*/




// Module image Magic
#define DEFAULT_TYPE "text/plain"


//#####~Docs~#######
//
// apache2: 		https://www.liquidweb.com/kb/configure-apache-virtual-hosts-ubuntu-18-04/
// .htaccess: 		https://httpd.apache.org/docs/2.4/howto/htaccess.html
// default page:	https://stackoverflow.com/questions/19322345/how-do-i-change-the-default-index-page-in-apache
//
//
//
//



/* Nombre */
#define GET		1
#define HEAD	2
#define POST	3
#define PUT		4
#define DELETE	5
#define CONNECT	6
#define OPTIONS	7
#define TRACE	8





#endif