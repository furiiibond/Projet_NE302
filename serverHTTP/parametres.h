

/* Les règles de grammaires par défault*/
#define HTTP_RULES "fullrfc.abnf"

#define CONF_RULES "config.abnf"






/**
 \Répartiton des paramètres:
	-La page par défault est définie on a per directory basis in the .htaccess file
		otherwise it's index.html
	-Les paramètres du site sont définis globalement dans /$(A_DEFINIR)/Server.conf
		If options are missing there's no default, should raise an error
*/
#define SERV_CONFIG "Hosts.conf"





//A faire: structure contenant les différentes options par site (pour liste chainée)
// IMPORTANT choisir si on utilise le style apache2 ou le notre
//exemple tiré de 000-default.conf (apache2)
struct Options {
	char ServerName[50];
	char DocumentRoot[256];
	
	
	struct Options* next;
}
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
