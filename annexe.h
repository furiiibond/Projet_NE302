#ifndef COLOR
	#include "color.h"
#endif
//Lib nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//ABNF Gramm file
#define ABNF_RULES "allrfc.abnf"

//Print TRUE or FALSE
void Truth(int b);

//Return the line matching $st in ABNF_RULES
char * content(char* st);

/** Compare deux chaines deux charactères de taille l */
int nocase_memcomp(char* s, char* d, int l);

//Return the distance from $st to the first character $a while not going past $end
int distance_from(char * st,char * end, char a);

//Move pointer $ptr to the next matching char $a while avoiding () [] and "" and also not going past $end
void goto_next(char ** ptr, char * end, char a);
