#include "semantique.h"


int verifSemantique(HeaderStruct* headers){
	//Si on a le header Connection a keep-Alive et close
	if(headers->connection.keepAlive && headers->connection.close)
		return ERR_400;
	
	//Si HTTP-version est sup à 1.1, il faut un champ Host.
	if(headers->httpVersion >= 11 && headers->host.count == 0)
		return ERR_400;
	
	return OK;
}