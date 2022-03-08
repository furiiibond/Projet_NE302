#include "annexe.h"


void Truth(int b){
    if (b)
        printf(GREEN"TRUE\n"NC);
    else
        printf(RED"FALSE\n"NC);
}


char * content(char* st){
    int len = strlen(st);
    FILE* file_abnf = fopen(ABNF_RULES, "r");
    if (file_abnf == NULL) {
        perror("Probleme ouverture gram.abnf");
        exit(1);
    }

    char curr_char = 0;
    int i = 0;
    char* c = (char*) malloc(1000*sizeof(char));    //Remplacer 1000 par une constante MAX
    for (int i=0; i<1000; i++) c[i]='\0';
    while(i<len && curr_char != EOF){
        i = 0;
        curr_char = getc(file_abnf);
        if (curr_char != st[i]){
            while (curr_char != '\n' && curr_char != EOF){
                curr_char = getc(file_abnf);
            }
        }
        while(curr_char == st[i] && i < len && curr_char != EOF){    //tant que le début de la ligne correspond à str
            curr_char = getc(file_abnf);
            i++;
        }
        if (i==len){
            curr_char = getc(file_abnf);
            if (curr_char != '=') i = 0;
        }
    }

    if (curr_char != EOF) {
        getc(file_abnf);
        curr_char = getc(file_abnf);    //espace
        i = 0;
        while (curr_char != '\n' && curr_char != EOF){    //Copie dans c jusqu'au '\n'
            c[i] = curr_char;
            i++;
            curr_char = getc(file_abnf);
        }
        c[i]='\0';
    }
    else{
        perror("ERR");
        printf("Pas trouvé :%s\n", st);
    }
    fclose(file_abnf);
    return c;
}

/** Compare deux chaines deux charactères de taille l */
int nocase_memcomp(char* s, char* d, int l){
    int i = 0;
    while(i<l && (s[i]==d[i] || (isalpha(s[i]) && isalpha(d[i]) && s[i]%32==d[i]%32))){
        i++;
    }
    return (i==l);
}

int distance_from(char * st,char * end, char a){
    // printf("\t\t\t\t\t>%s\n",st);
    int res=0;
    while(st<end && *st != a && *st != '\0') {res++;st++;}
    return res;
}

void goto_next(char ** ptr, char * end, char a){
    int parenthesis=0;
    int crochet=0;
    while(*ptr < end && ( **ptr != a || parenthesis || crochet)) {
        if(**ptr=='"') *ptr += distance_from((*ptr)+1,end,'"')+2;
        parenthesis+=(**ptr=='(');
        crochet+=(**ptr=='[');
        parenthesis-=(**ptr==')');
        crochet-=(**ptr==']');
        (*ptr)++;
    }
}
