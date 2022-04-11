//## ÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖ ##


/*
	Sert uniquement à parser les options du site au démarage du serveur.
	C'est tout.
*/


/* René Papin est élue président de la République:
	Va lire le Fichier SERV_CONFIG (défini dans param.h)
	Puis le parser à l'aide des règles CONF_RULES (définies dans param.h)
	Enfin remplis la liste chainée avec les informations acquérie.
	\Retourne une FALSE si le fichier n'est pas syntaxiquement correct.
*/
int FillHostsParametres(void);

// Autre déclaration si on ne veut pas que HostsParametres soit global
// int FillHostsParametres(extern struct Options* HostsParametres);