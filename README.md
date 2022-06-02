# README

**** Instructions de compilation:
Options du Makefile
- export: affiche la ligne à copier coller pour indiquer
la localisation des bibliothèques partagées
- main: compile le serveur (avec libnotreparser)
et copie l'executable dans le dossier courant
- sharedobject: recompile la bibliothèque partagée dans libnotreparser
- clean: supprime le binaire et les fichiers objets dans les sous-dossiers

======================================================================

**** Fichiers configurables:
Développeurs:
- parametres.h
	COLOR = active ou non la couleur dans la sortie terminal
	HTTP_RULES = couple (nom du fichier abnf pour les requêtes HTTP,
						 le premier tag )
	PATH_LEN_MAX = longueur maximale pour le chemin d'un fichier
	HEADER_LEN_MAX = longueur maximum pour les headers de la réponse HTTP
	SERV_CONFIG = nom du fichier de configuration serveur
	CONF_RULES = couple (nom du fichier abnf pour la config serveur,
						 le premier tag )

Utilisateurs:
Fichier de configuration
- server.conf
	La syntaxe et la suivante:
	Tout ce qui suit un '#' est considéré comme commentaire
	Une entrée est composée d'un Hostname, suivie
	d'accollade contenant diffénts champs
	Les champs sont séparés par un retour chariot
	et un saut de ligne (CRLF)
	Un seul champ nécessaire:
		Root-dir: le repertoire racine du site.
		(syntaxe: chemin (relatif ou absolu) entre guillemets,
		ne doit pas se terminer par un /)
	Si le hostname n'est pas reconnu, le serveur défaut sur
	la première entrée.
Fichiers abnf
*Si vous ne savez pas ce que vous faites il est recommandé de ne pas toucher à ceux fournis*
	- fullrfc.abnf
		La syntaxe abnf pour parser les requêtes HTTP.
	- config.abnf
		La syntaxe abnf pour parser le fichier de configuration.


======================================================================

**** Fonctionnalités
- Prise en charge des méthodes GET et HEAD.
- Headers réponse:
	Content-length => taille du message body
	Content-type => type du message body
					utilisation de libmagic puis extension
					si ça ne marche pas
	Connection => close si pas de keep-Alive
	Allow => methode supportées (pour l'erreur 405)
- Headers réponse:
	Host => nom du site auquel le client veut accéder
	Connection => on conserve la connection si keep-Alive
- version HTTP:
	On supporte la version 1.0 et 1.1
- Percent Encoding:
	Traduction d'une url pourcent encodée vers un chemin de type UNIX
- Multi Site:
	Si HTTP/1.1, support du multi site en fonction du champ
	Host et du fichier config
- Gestion des Erreurs:
	400 Bad Request
	403 Forbidden
	404 Not found
	405 Method Not Allowed
	500 Internal Server Error
	501 Not implemented


---- Type de fichier testé ----
javascript
css
plain-text
html
mp4
icon


## Utilisation:

Lancer ./server

**** Server will listen on 8080 TCP port.
**** Server will wait for one request and exit

## Auteurs :

Mathis RANC, Jean ROUGE, Jean-Camille LAPIERRE, Hugo GARDE

## Parser :

Le fichier parametres.h de configuration du programme (la grammaire lue, affichage de l’arbre, etc..)

## Server :

Serveur HTTP dans ./serverHTTP
Effectuer un make clean puis la commande : export LD_LIBRARY_PATH=LD_LIBRARY_PATH:../libnotreparser/:../librequest-0.5/
