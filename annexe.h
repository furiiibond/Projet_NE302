#ifndef ANNEXE_H
#define ANNEXE_H

#ifndef COLOR
	#include "color.h"
#endif
//Lib nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "parametres.h"

// String_view librairy
#include "./sv.h"


//Init ABNF_rule_mmap
void init_gramm_rule(void);


void cacheAddUser(String_View sv);
void cacheDelUser(String_View sv);


//Print TRUE or FALSE
void truth(int b);

//Return the line matching $st in ABNF_RULES
String_View getRule(String_View sv);

String_View searchNewRule(String_View string);

// Compare two pointers to char of length $l
int nocase_memcomp(const char *s, const char *d, int l);

//Return the distance from $st to the first char $a while not going past $end
int distance_from(const char *st, const char *end, char a);

//Move pointer $ptr to the next matching char $a while avoiding () [] and "" and also not going past $end
void goto_next(const char **ptr,const  char *end, char a);

//Fix tous les petits problèmes
/* /!\ TRèS INSTABLE /!\ */
void FIX_FINAL_NE_PAS_TOUCHER(void);

#endif