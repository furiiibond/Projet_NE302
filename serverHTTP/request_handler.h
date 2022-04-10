//## ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ##

/*
	Est utile comme interprète principal entre le serveur et
		les implémentations de méthodes comme GET ou HEAD.
*/


// IMPORTANT Décider à quel endroit se fait le parsing de la requète:
//		(server.c ou request_handler.c)
// +++++++++ Décider si le RequestHandler à le droit de répondre directement au client
// 		ou si c'est le travail du serveur (à ce moment là le ReqH renvoirait une zone mem. )
/*
	Réponse de Jean:
	Idéalement je pense qu'on peut avoir notre Handler et notre Serveur en parallèle
	et dès que le Handler aurait un quelque chose à envoyer, il le ferait passer au Serveur 
	à l'aide d'un pipe par exemple (il faudrait éviter les recopies de données).
	Autre idée, le Handler peut juste retourner avec un code spécial, qui ordonnerait au serveur
	de répondre à la requète avec le fichier dont le chemin aurait été placé dans une zone spécial
	(comme ça la recopie serait limitée au nom du fichier)
*/

int RequestHandler(message *requete);