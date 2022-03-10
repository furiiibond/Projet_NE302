#include "parser.h"
#include "annexe.h"

// Global var, used to point the file to parse
char *mem;
char *no_go_zone;

/* ------------- ETAPE 1 ------------- */
int matchedCommand(char **ptr, char *end, node **node_ptr) {
	int res = TRUE;
		#ifdef DEBUG
		printf("Matched a letter !\n");
		#endif
	char * debut = (*ptr);
	while (*ptr < end && !isspace(*++(*ptr))) ;

	char command[50];
	int longueur = ((*ptr) - debut);
	memcpy(command, debut, longueur);
	command[longueur] = '\0';
		#ifdef DEBUG
			printf(BLUE "cmd_> %s\n"NC" ",command);
		#endif
	char* backup = mem;

/** ####### APPEL RECURSIF ##################*/
/** #####*/	res = construire(command,*node_ptr); /**####*/
/** #########################################*/
	if (!res)
        mem = backup;

    else{ //Si la construction est un succes on met bien à jour le papa et on crée un nouveau frère pour node_ptr
        strcpy((*node_ptr)->tag,command);
        (*node_ptr)->value = backup;
        (*node_ptr)->len_value = mem-backup;
        (*node_ptr) = addNodeAsBrother("", mem, 0, *node_ptr);
    }
		#ifdef DEBUG
			printf("res %s: ", command); truth(res);
		#endif
	return res;
}

int loopAlgoCalls(int min, int max, char *debut, char *fin, node **noeud) {
	int res = TRUE;
	char *backup = mem;
	char *backup2 = mem; // if we validate less time than min
	int i = 0;
	while (i < max && res) {
		#ifdef DEBUG_MEMVIEW
		printf(CYN"tmp:%c\n"NC, *mem);
		#endif
		#ifdef DEBUG_IO_CONSTRUIRE
		printf(RED"Call>_algo0("NC);
        fflush(stdout);
		write(STDOUT_FILENO, debut, fin - debut);
		printf(RED")\n"NC);
        #endif

		res = algo0(debut, fin - debut, *noeud);
        if (res) {
            while ((*noeud)->brother)
                (*noeud) = (*noeud)->brother;//get back to the current empty brother( the last)
        } 
		else
            reset_first_child_queue(*noeud);

        if (res)
            backup = mem;
		i += res;
	}
    mem = backup;
	res = (i >= min) && (i <= max);
	if (!res)
        mem = backup2;
	#ifdef DEBUG
		printf("\texit loop with i=%d\n", i);
		printf("res: "); truth(res);
	#endif
	return res;
}
/*
int Matched_digit(char** ptr, char *end){
}

int Matched_star(char** ptr, char *end){
}

int Matched_crochet(){
}
*/

void reset_first_child_queue(node* noeud) {
    noeud->tag[0]='\0';
    noeud->len_value = 0;
    if (noeud->brother) {
        purgeTree(noeud->brother);
        noeud->brother = NULL;
    }
	if (noeud->child) {
        purgeTree(noeud->child);
        noeud->child = NULL;
    }
		
}

int algo0(char *str, int len, node * first_Child) { // if error return 1 else 0
	char *ptr, *end;
    node * node_ptr = first_Child;
	int res = TRUE;
	ptr = str;
	end = str + len;
	#ifdef DEBUG
		printf(MAGENTA"Entree dans algo:[");
		fflush(stdout);
		write(STDOUT_FILENO, str, len);
		printf("]=[%s]\n"NC,str);
	#endif

// FIX UTILE A VERIFIER
char * sauvegarde_pointeur_memoire = mem;
// ATTENTION AU COMPTE RENDU DE DEMAIN
// Conclusion ce fix fonctionne et il rend même obsolète
// d'autre outils de gestion du pointeur mem.
// Il a d'ailleur permis de valider les test 8 et 3 qui ne marchaient pas
// On virera les trucs superflu plus tard. 



	while (ptr < end && *ptr != '\0' && mem < no_go_zone) {
		#ifdef DEBUG
			printf("Current scan: [%c]\n", *ptr);
		#endif

/*----*/if (isalpha(*ptr)) {/* ETAPE 1 ------------- */

            res = matchedCommand(&ptr, end, &node_ptr);
		}
		else
/*----*/if (isdigit(*ptr) || *ptr=='*') {/* ETAPE 3  */
			int x, y;
			y = MAX_ITER;
			x = MIN_ITER;

			if (isdigit(*ptr)) {
				#ifdef DEBUG
				printf("Matched a digit\n");
				#endif
				sscanf(ptr, "%d*%d", &x, &y);
				if (*(ptr + 1) != '*')
					y = x;
			} else {
				#ifdef DEBUG
				printf("Matched a '*'\n");
				#endif
				sscanf(ptr, "* %d", &y);
			}
			while (!isspace(*++ptr)){;}
			while (isspace(*++ptr)){;}
			#ifdef DEBUG
				printf(BLUE"\tnext:[%c]\n"NC, *ptr);
				printf("\tx:%d / y:%d\n", x, y);
			#endif

			char* debut_chaine = ptr;

			if (*ptr == '(') {
				//Etape n°33
				debut_chaine++; //if debut_chaine -> '(' on l'incr pour qu'il pointe sur le vrai debut de la chaine
				ptr++; // de même pour ptr sinon il pointe sur '('
				goto_next(&ptr, end, ')');
			}
            else {
				//Etape n°32
				while (! isspace(*++ptr)) ;
			}

            res = loopAlgoCalls(x, y, debut_chaine, ptr, &node_ptr);

			if (*ptr == ')')
                ptr++;
		}
		else
/*----*/if (*ptr == '%') { /* ETAPE 4 ----------------- */
			#ifdef DEBUG
			printf("Matched '%%' !\n");
			#endif
			ptr ++;
			assert(*ptr == 'x');
			#ifdef DEBUG
			puts("\tx is there, good.");
			#endif

			if (*(ptr + 3) == '.') {
				/*Si: %xHH.HH.HH...*/
				#ifdef DEBUG
				puts("\tdetected pattern: HH.HH...");
				#endif
				int nb;
				char *mem_bak = mem;
				while(!isspace(*ptr) && res && mem < no_go_zone) {
					sscanf(ptr + 1, "%x", &nb);
					#ifdef DEBUG_MEMVIEW
						printf("\t\twhl ptr:%c\n",*(ptr));
						printf("\t\tmem: %c %x | ptr: %c %x\n", *mem, *mem, nb, nb);
					#endif
					res = (*mem == nb);
					mem += res;
                    ptr += 3;
				}
				if (!res) {
                    mem = mem_bak;
                }
            }
            else {
				/*Si: %xHH-HH ou %xHH */
				#ifdef DEBUG
				printf("\tdetected pattern: HH-HH\n");
				#endif
				u_int8_t h1, h2;
				sscanf(ptr + 1,"%hhx-%hhx", &h1, &h2);

				if (*(ptr + 3) == '-')
                    ptr += 6;
				else{
                    ptr += 3;
                    h2 = h1;
                }
				#ifdef DEBUG_MEMVIEW
					printf("\t\th1:%x h2:%x | mem:%x %c\n", h1, h2, *mem, *mem);
				#endif
				res = ((*mem & MASK_OCTET) >= h1) && ((*mem & MASK_OCTET) <= h2);
				mem += res;
			}
			#ifdef DEBUG
				printf("res: "); truth(res);
			#endif
		}
		else
/*----*/if (*ptr == '"') { /* ETAPE 5 ----------------- */
			#ifdef DEBUG
				printf("Matched '\"'\n");
			#endif
			ptr++;
			int longueur = distance_from(ptr, end, '"');
			if (mem + longueur >= no_go_zone)
				res = FALSE;
			else
				res = nocase_memcomp(ptr, mem, longueur);
			#ifdef DEBUG_MEMVIEW
                printf("\t[");
                fflush(stdout);
                write(STDOUT_FILENO, ptr, longueur);
                printf("]\n\t[");
                fflush(stdout);
                write(STDOUT_FILENO, mem,longueur);
                puts("]");
            #endif

            if (res) {
                strcpy(node_ptr->tag, "case_insensitive_string");
                node_ptr->value = mem;
                node_ptr->len_value = longueur;
                mem += longueur;
                node_ptr = addNodeAsBrother("", mem, 0, node_ptr);
            }
			ptr += longueur + 1;

			#ifdef DEBUG
				printf("res: "); truth(res);
			#endif
		}
		else
/*----*/if (*ptr == '(') {/* ETAPE 9 ----------------- */
			#ifdef DEBUG
				printf("Matched '('\n");
			#endif
			ptr++;
			char *debut = ptr;
			goto_next(&ptr, end, ')');

			res = loopAlgoCalls(1, 1, debut, ptr, &node_ptr);

			#ifdef DEBUG
				printf("Out of parenthesis; current:'%c'\n", *ptr);
			#endif
            ptr++;
		} else
/*----*/if (*ptr == '[') {/* ETAPE 10 ---------------- */
			char *debut = ++ptr;

			goto_next(&ptr, end, ']');

            res = loopAlgoCalls(0, 1, debut, ptr, &node_ptr);

			ptr++;
		}
		else if (*ptr == ')') {
            #ifdef DEBUG
                printf("ptr-20:%s\n", ptr - 20);
                printf(YELLOW"str:(%s)\n"NC, str);
            #endif
			perror("Parsing error on parenthesis");
			exit(1);
		}

/* ---- ETAPE 2 ---------------------------------- */
		{
			if (*ptr=='/') {
				if (res) {
					//WHILE EXIT (return res)
					ptr = end - 1;
				} else {
					res = TRUE;
					mem = sauvegarde_pointeur_memoire;
                    node_ptr=first_Child;
                    reset_first_child_queue(node_ptr);
				}
			} else {
				if (!res) {
					//Step n23 Goto '/'
					goto_next(&ptr, end, '/');
					if(ptr < end && *ptr == '/')//peut-être que le if n'est même pas nécessaire
                        ptr--;
				}
			}
		}
		ptr++;
	}
	if (mem > no_go_zone) {
		perror("mem pointer out of range");
		res = FALSE;
	}
	#ifdef DEBUG
		printf("Return res:"); truth(res);
	#endif

	return res;
}

/* A déplacer ou vous voulez (peut-etre tree.c)*/
int deleteEmptyBrothers(node* noeud) {
    if (noeud == NULL)
        return 1; // safe fail
    if (*(noeud->tag) == '\0') {
        free(noeud);
        return 1;
    }
    else {
        if (deleteEmptyBrothers(noeud->brother))
            noeud->brother = NULL;
        return 0; // So previous know it's not empty
    }
}


int construire(char *module, node* parent) {
    int valid = 0;
    char *str = content(module);
	#ifdef DEBUG_IO_CONSTRUIRE
		printf(YELLOW"CONSTRUIRE %s\n"NC, module);
		printf(YELLOW"str:|%s|\n"NC, str);
	#endif

    char *debut_value = mem;


    //node *Current_node = addNodeAsChild(module, debut_value, 0, parent);
    node *first_child = addNodeAsChild("", debut_value, 0, parent);

    /* Procedure d'appel principale */
    valid = algo0(str, strlen(str), first_child);

    if (valid) {
        if (deleteEmptyBrothers(first_child)) {
            parent->child = NULL;
        }
    } else {
        purgeTree(first_child);
        parent->child = NULL;
    }



	#ifdef DEBUG_IO_CONSTRUIRE
		if (valid)
            printf(GREEN"Nice"NC" (exiting:%s)\n", module);
		else
            printf(RED"Not nice"NC" (exiting:%s)\n", module);
		// write(STDOUT_FILENO,debut_value,mem-debut_value); printf("[EOL]\n");
		write(STDOUT_FILENO, mem, 20);
        printf("[EOL]\n");
	#endif

	free(str);

	return valid; // return 1 si valide
}
