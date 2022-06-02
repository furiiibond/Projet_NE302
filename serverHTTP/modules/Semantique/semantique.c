#include "semantique.h"
#include <stdio.h>

int verifSemantique(HeaderStruct* headers){
	//Si on a le header Connection a keep-Alive et close
	if(headers->connection.keepAlive && headers->connection.close)
		return ERR_400;

	//Si HTTP-version est sup à 1.1, il faut un champ Host.
	if(headers->httpVersion >= 11 && headers->host.count == 0)
		return ERR_400;

	// Si l'absolute path est plus grand que la valeur maximale supporté par le serveur
	if(headers->absolutePath.count>=PATH_LEN_MAX)
		return ERR_414;


	//  	Suite à la soutenance on rajoute cette vérification
	//   It's that easy ;)
	if(headers->contentLength.data){
		int a=0;
		sscanf(headers->contentLength.data,"%d",&a);
		if( (int) headers->msg_body.count != a)
			return ERR_400;
	}


	return OK;
}
