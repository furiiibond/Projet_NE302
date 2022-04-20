#ifndef PARAMETRES00
#define PARAMETRES00

//Prof
#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\nHey Bro why did you send me this:\r\n"





/* Les règles de grammaires par défault*/
#define HTTP_RULES "fullrfc.abnf"
#define FIRST_TAG "HTTP-message"

#define CONF_RULES "config.abnf"


typedef struct {
	char path[256];
	char type[20];
} Fichier;

typedef struct {
	char* content;
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





//A faire: structure contenant les différentes options par site (pour liste chainée)
// IMPORTANT choisir si on utilise le style apache2 ou le notre
//exemple tiré de 000-default.conf (apache2)
struct Options {
	char ServerName[50];
	char DocumentRoot[256];
	
	
	struct Options* next;
};
extern struct Options* HostsParametres;
/*
	L'intéret sera de pouvoir parser le fichier de config serveur, puis créer une liste
	chainée où chaque maillon contiendra les informations d'un site en particulier.
	Ces infos seront utiles pour traiter les requetes (ex:GET)
	que ce soit en version 1.0 (DocumentRoot), ou 1.1 (ServerName).
*/




//#####~Docs~#######
//
// apache2: 		https://www.liquidweb.com/kb/configure-apache-virtual-hosts-ubuntu-18-04/
// .htaccess: 		https://httpd.apache.org/docs/2.4/howto/htaccess.html
// default page:	https://stackoverflow.com/questions/19322345/how-do-i-change-the-default-index-page-in-apache
//
//
//
//

#endif