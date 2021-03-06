#ifndef PARAMETRESH
#define PARAMETRESH

/* Color 0:desactivated / 1:activated */
#define COLOR 1
#include "annexe/color.h"


/* Les règles de grammaires par défault*/
#define HTTP_RULES "fullrfc.abnf","HTTP-message"

// Structure fichier
#define PATH_LEN_MAX 8000
#define TYPE_LEN_MAX 64

// Structure HTML_Rep
#define HEADER_LEN_MAX 1024

/**
 \Répartiton des paramètres:
	-La page par défault est définie on a per directory basis in the .htaccess file
		otherwise it's index.html [Pas Fait encore]
	-Les paramètres du site sont définis globalement dans $(SERV_CONFIG_PATH)/$(SERV_CONFIG)
		If options are missing there's no default, should raise an error [Fait]
*/
//#define SERV_CONFIG_PATH "."
#define SERV_CONFIG "server.conf"
#define CONF_RULES "config.abnf","Config"

// Version HTTP pour les réponses
#define SERV_VERSION(v)  (v >= 11) ? "HTTP/1.1" : "HTTP/1.0"

// Pour option_parser
#define HOST_LEN_MAX 64
#define PHPHANDLER "phpHandler"

// Module image Magic
#define DEFAULT_TYPE "text/plain"


#define SERVERPORT 8080

//#####~Docs~#######
//
// apache2: 		https://www.liquidweb.com/kb/configure-apache-virtual-hosts-ubuntu-18-04/
// .htaccess: 		https://httpd.apache.org/docs/2.4/howto/htaccess.html
// default page:	https://stackoverflow.com/questions/19322345/how-do-i-change-the-default-index-page-in-apache
//
// percent-encoding to unix style paths:
// https://brettterpstra.com/2015/11/16/shell-tricks-convert-file-urls-to-unix-paths/

#endif
