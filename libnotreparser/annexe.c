#define SV_IMPLEMENTATION
#include "annexe.h"

// TODO: Sort what's unused.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

int ABNF_FileDescriptor;
String_View Abnf_FileMap; //should be const
char first_tag[TAG_LEN];

typedef struct {
	unsigned long key;
	String_View rule;
	char flags;//4 High Bits are number of uses, 4 low bit are number of owner of that line
} Ligne;

#define CACHE_LINE_NB 20
Ligne Cache[20] = {0};


/* Credit: djb2 by Dan Bernstein
   http://www.cse.yorku.ca/~oz/hash.html  */
unsigned long hash(String_View sv)
{
	
    unsigned long hash = 5381;
    int c;
	size_t i=0;
    while (i<sv.count && (c = sv.data[i++]))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void load_gramm_rule(char *gram_file, char *rule_init){
	if(strlen(rule_init)>TAG_LEN){
		perror("First tag too long");
		exit(1);
	}
	strcpy(first_tag,rule_init);
	
	if ((ABNF_FileDescriptor = open(gram_file, O_RDWR)) == -1) {
		perror("Can't open gram_file");
		exit(1);
	}
	struct stat st;
	if (fstat(ABNF_FileDescriptor, &st) == -1)
		exit(1);
	if ((Abnf_FileMap.data = mmap(NULL, st.st_size, PROT_WRITE, MAP_PRIVATE, ABNF_FileDescriptor, 0)) == NULL)
		exit(1);
	Abnf_FileMap.count = st.st_size;
}

void close_gramm_rule(void){
	close(ABNF_FileDescriptor);
}

int findCacheLine(unsigned long key){
	int i_find=-1;
	for(int i=0; i<CACHE_LINE_NB && i_find<0; i++){
		if ( Cache[i].key == key )
			i_find=i;
	}
	return i_find;
}

//Pas ouf
void cacheAddUser(String_View sv){
	unsigned long key = hash(sv);
	int i_find = findCacheLine(key);
	if(i_find<0)
		perror("error cacheAddUser: Impossible");
	else{
		Cache[i_find].flags += 0x1;//+1 sur la partie basse
		//Il faudrait check les overflows pour pas que ça mange sur la partie haute
		//Mais flemme, on va dire que ca n'arrivera pas
	}
}

//Par contre celle-là utile
void cacheDelUser(String_View sv){
	unsigned long key = hash(sv);
	int i_find = findCacheLine(key);
	if(i_find<0)
		perror("error cacheDelUser: Impossible");
	else{
		Cache[i_find].flags -= 0x1;//-1 sur la partie basse (check overflow ...)
	}
}



String_View cacheGetLine(unsigned long key){
	String_View result;
	int i_find = findCacheLine(key);
	if(i_find<0){
		result.data = NULL;
	}else{
		result.data = Cache[i_find].rule.data;
		result.count = Cache[i_find].rule.count;
		//Increment number of uses
		Cache[i_find].flags += 0x10;//+1 sur la partie haute
		//	Penser à faire pour éviter un appel à cacheAddUser
		//Cache[i_find].flags += 0x1;
	}
	return result;
}

int cacheAddLine(String_View sv){	
	int i_find=-1;
	for(int i=0; i<CACHE_LINE_NB && i_find<0; i++){
		//We want a cached line without owners
		if ( (Cache[i].flags&0xF) == 0 )
			i_find=i;
	}
	Cache[i_find].key = hash(sv);
	Cache[i_find].rule.data = sv.data;
	Cache[i_find].rule.count = sv.count;
	Cache[i_find].flags = 0x10;// Means 1 use and 0 owner.
	//	Penser à faire pour éviter un appel à cacheAddUser
	//Cache[i_find].flags += 0x1;
	
	return 0;
}

//Lis le cache ou Dans le fichier
String_View getRule(String_View sv){
	String_View result = {0};
	//Calcul de la cle
	unsigned long key = hash(sv);
	//Verif dans le Cache
	result = cacheGetLine(key);
	//Si trouvé, on renvoie la ligne
	//Sinon
	//Nouvelle ligne
	if (result.data == NULL) {
		//nouveau sv
		result = searchNewRule(sv);
		//on l'ajoute dans le Cache
		cacheAddLine(result);
	}
	
	return result;
}

/* Fonction: searchNewRule
	arg: string, la méthode dont on cherche la règle
	"Danielo est prêt pour le combat"
*/
String_View searchNewRule(String_View string){
    String_View line;
    String_View method;
    /*Copy Abnf_FileMap in rule*/
    String_View rule;
    rule.data = Abnf_FileMap.data;
    rule.count = Abnf_FileMap.count;
    
    int cmp;    //result of memcmp
    
    do { 
        line = sv_chop_by_delim(&rule, '\n');    //line ends before '\n', rule start after '\n'
        method = sv_chop_by_delim(&line, '=');    //method ends before '=', line starts after '=' (and ends before '\n')
        cmp = memcmp(string.data,method.data,string.count);
    } while(rule.count > 0 && cmp);    //While we haven't found the string
    
    if (cmp){    //if we haven't found the string
        fprintf(stderr,"failed to find method \""SV_Fmt"\"\n",SV_Arg(string));
        exit(1);
    }
    else{
        return line;
    }
}

void truth(int b) {
    if (b)
        printf(GRN"TRUE\n"NC);
    else
        printf(RED"FALSE\n"NC);
}


/** Compare deux chaines deux charactères de taille l */
int nocase_memcomp(const char *s, const char *d, int l) {
    int i = 0;
    while (i < l && (s[i] == d[i] || (isalpha(s[i]) && isalpha(d[i]) && s[i]%32 == d[i]%32)))
        i++;
	#ifdef DEBUG_MEMVIEW
	printf(RED"___DEBUG___\n");
	write(STDOUT_FILENO, s, l); puts("");
	write(STDOUT_FILENO, d, l);
	truth(i==l);
	printf("___FIN___\n"NC);
	#endif
    return (i == l);
}

#include <unistd.h>
void FIX_FINAL_NE_PAS_TOUCHER(void){
	sleep(0);
}

int distance_from(const char *st, const char *end, char a) {
    // printf("\t\t\t\t\t>%s\n",st);
    int res = 0;
    while (st < end && *st != a && *st != '\0') {
		res++;
		st++;
	}
    return res;
}

void goto_next(const char **ptr, const char *end, char a) {
    int parenthesis = 0;
    int bracket = 0;
    while (*ptr < end && ( **ptr != a || parenthesis || bracket)) {
        if (**ptr=='"') 
			*ptr += distance_from((*ptr)+1,end,'"') + 2;
        parenthesis += (**ptr=='(');
        bracket += (**ptr=='[');
        parenthesis -= (**ptr==')');
        bracket -= (**ptr==']');
        (*ptr)++;
    }
}
