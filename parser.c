#include "parser.h"
#include "annexe.h"

// Global var, used to point the file to parse
char *mem;
char *no_go_zone;

/* ------------- ETAPE 1 ------------- */
/* Cette fonction est appellé quand algo0() rencontre un charactère alphanumérique 
	(ce qui correspond à un tag de la grammaire abnf)
	Le but de cette fonction est donc d'appller la fonction construire sur ce tag.
*/
int matchedCommand(char **ptr, char *end, node **node_ptr) {
	// Initialisation de res la variable qui contiendra le résultat à retourner
	int res = TRUE;//pas nécessaire d'initialiser à TRUE
	
	//debut est un pointeur vers la première lettre du tag (le début du mot, d'où le nom)
	char * debut = (*ptr);
	//pour avoir la fin du mot il suffit d'avancer le pointeur *ptr jusqu'au prochain espace
	//(dans la grammaire tous les éléments sont séparés par des espace)
	while (*ptr < end && !isspace(*++(*ptr))) ;

	// On déclare un tableau de char de taille fixe pour contenir le tag qu'on veut
	// passer en paramètre à la fonction construire().
	char command[TAG_LEN];
	//On peut calculer la longueur du tag facilement car *ptr pointe sur le char n+1 du mot
	//et debut sur le char 1 donc *ptr - debut = x+n+1 - x+1 = n.
	int longueur = ((*ptr) - debut);
	//On copie donc le tag dans le buffer command.
	memcpy(command, debut, longueur);
	//Et on ajoute un \0 à la fin de la chaine car toutes les strings doivent finir par un character nul (EOL ?)
	command[longueur] = '\0';
	
	// Ici on sauvegarde le pointeur vers le current char du fichier à parser
	// car si jamais construire n'est pas validé, il risque d'avoir modifié ce pointeur
	// et c'est à la fonction appellante de le restorer.
	char* backup = mem;

// Le coeur de l'algorithme, on appelle construire avec en parametre le tag à évaluer
// ainsi qu'un pointeur vers le noeud de l'arbre ou nous nous situons actuellement
// (Exemple: si on évalue le tag start-line, on passe à construire un pointeur vers 
//  le noeud avec l'étiquette start-line et construire() s'occupera de créer ses fils
//  ou ne fera rien si le résultat est faux.
/** ############# ~~ APPEL RECURSIF ~~ ##################*/
/** #####*/	res = construire(command,*node_ptr); /**#####*/
/** #####################################################*/
	
	if (!res)
	//Si le resultat est faux, on restore le pointeur vers la mémoire qui à été sauvegardé précédement
        mem = backup;
    else{ 
	//Si la construction est un succes on met bien à jour le papa 
        strcpy((*node_ptr)->tag,command);
		//On utilise la sauvegarde "backup" pour définir le début du contenu 
		//Et on calcule la longueur de la même manière que précédemment
        (*node_ptr)->value = backup;
        (*node_ptr)->len_value = mem-backup;
	//et on crée un nouveau frère "vide" pour node_ptr
        (*node_ptr) = addNodeAsBrother("", mem, 0, *node_ptr);
    }
	
	//On retourne le résultat renvoyé par construire()
	return res;
}


/* ------------- LOOP ------------- */
/* Cette fonction permet de faciliter les répétitions d'appels à algo0()
	Elle est principalement utilisée pour les chiffres / * / ( ) / [ ]
*/
int loopAlgoCalls(int min, int max, char *debut, char *fin, node **noeud) {
	// Initialisation de res la variable qui contiendra le résultat à retourner
	int res = TRUE;
	
	//On va faire deux backup mémoire,
	//		Un pour restorer dès que res sera faux
	//		Un pour restorer si on a validé res moins de fois que min
	char *backup = mem;
	char *backup2 = mem; // if we validate less time than min
	
	int i = 0;
	while (i < max && res) {
				
		// On backup le pointeur vers le noeud vide actuel.
		// Si on ne valide pas un appel à algo0(), on restore ce pointeur et on supprime tout
		// memory leak fix 1 (more to come) EDIT:CA N'A RIEN FIXE DU TOUT EN FAIT, INUTILE
/*/!\*/	node* bakcup_first_child = *noeud;
		
		/** ############# ~~ APPEL ITERATIF ? ~~ ##################*/
		/** ###*/ res = algo0(debut, fin - debut, *noeud);  /** ###*/ 
		/** #######################################################*/
        
		
		if (res) {
		//On déplace le pointeur de noeud vers le dernier frère vide
		//On est onligé de le faire nous-même car algo0 n'est pas conçu pour modifier un pointeur d'arbre
		//On lui passe uniquement (ce qu'elle considérera comme) le premier frère en paramètre.
		//Ce frère existera toujours, peu importe le résultat.
            while ((*noeud)->brother)
                (*noeud) = (*noeud)->brother;//get to the current empty brother( the last)
        } 
		else{
			//Si le resultat est faux, on veux supprimer tous les frère/fils qui auraient été créé
/* /!\/!\ */*noeud =  bakcup_first_child;//noeud ne peut pas être modifié par algo donc cette ligne est inutile
            //on transforme donc le noeud pointé par *noeud en élément vide et on supprime toute son arborescence
			reset_first_child_queue(*noeud);
		}
		
		//Si le résultat est vrai on fait un nouveau backup pour que le dernier faux qui soit renvoyé
		//puisse rollback à l'avant dernière itération sans perdre tout le progrès
        if (res)
            backup = mem;
		//On incrémente le nombre de loop validée si res est vrai
		i += res;
	}
	//On restore le backup
	//		Si on est sortit du while car res est faux il est nécessaire de restorer le pointeur pour que 
	//		le reste du parsing puisse se dérouler correctement
	//		Si on est sorti car on à atteint le nombre max d'ittération alors le pointeur mem
	//		n'a pas été modifié entre temps et cette ligne n'a aucun effet.
    mem = backup;
	//On vérifie si le nombre de validation est bien compris entre min et max (inclus) 
	res = (i >= min) && (i <= max);
	//Si le résultat final est faux, on restore le tout premier backup de la mémoire pour annuler toutes
	//les modifiactions faites à mem
	if (!res)
        mem = backup2;
	
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


/* Cette fonction transforme l'element pointé par noeud en élément vide
	puis purge l'arbre à partir de cet élément
*/
void reset_first_child_queue(node* noeud) {
	//rend l'élément "vide"
    noeud->tag[0]='\0';
    noeud->len_value = 0;
	/*Ce serait bien si on pouvait appeler purgeTree() sur NULL*/
	//Purge a partir du brother et child s'ils existent
    if (noeud->brother) {
        purgeTree(noeud->brother);
        noeud->brother = NULL;
    }
	if (noeud->child) {
        purgeTree(noeud->child);
        noeud->child = NULL;
    }
		
}


/** Si il y a bien une fonction à bien expliquer, c'est celle-là.
	L'idée est d'avoir un pointeur qui parcour la chaine de charactère
	correspondant à la ligne de la grammaire abnf que nous somme en train d'analyser.
	On prend ensuite des décisions en fonction des charactères lu au fur et à mesure.
**/
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

/** IDEE: Refaire les dépendances
		Ex: Algo0-> TRUE = renvoie un arbre valide
					FALSE = renvoie un élément vide
		construire->TRUE = arbre valide descendant de parent
					FALSE = arbre vide parent->child = NULL
**/

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
