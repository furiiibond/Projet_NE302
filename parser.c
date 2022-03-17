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

int Boucle1(int x, int y, char **ptr, char *end, node** node_ptr){
	
	//on déplace le pointeur après les chiffres/étoile
	while (!isspace(*++*ptr)){;}
	//on mange les espaces, pour mettre le pointeur sur
	//le premier char du morceau de règles à looper
	while (isspace(*++*ptr)){;}
	
	//On sauvegarde le début de la chaine 
	char* debut_chaine = *ptr;

	if (**ptr == '(') {
		//Etape n°33
		debut_chaine++; //if debut_chaine -> '(' on l'incr pour qu'il pointe sur le vrai debut de la chaine
		(*ptr)++; // de même pour ptr sinon il pointe sur '('
		//La fonction got_next() emmene le pointeur directement à la fin de la parenthese
		goto_next(ptr, end, ')');
	}
	else {
		//Etape n°32
		//Comme il n'y a pas de parentheses, la boucle s'applique uniquement au mot qui suit
		while (! isspace(*++*ptr)) ;
	}

	//appel à la fonction de boucle qui renverra la validité du morceaux de règle en fonction de x et y
	// (elle remplie aussi l'arbre)
	int res = loopAlgoCalls(x, y, debut_chaine, *ptr, node_ptr);

	// on est pas forcément obligé de faire un if ici
	// je pense que peut importe le cas l'incrémenter ne devrait pas être néfaste
	if (**ptr == ')')
		(*ptr)++;
	
	return res;
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
	

// FIX UTILE A VERIFIER
char * sauvegarde_pointeur_memoire = mem;
// ATTENTION AU COMPTE RENDU DE DEMAIN
// Conclusion ce fix fonctionne et il rend même obsolète
// d'autre outils de gestion du pointeur mem.
// Il a d'ailleur permis de valider les test 8 et 3 qui ne marchaient pas
// On virera les trucs superflu plus tard. 


/*	Cette boucle while premet de parcourir le contenu de la règle abnf passé en parametre (str)
	on déplace juste un pointeur ptr le long de cette chaine en exécutant les action correspondantes
	à chaque cas rencontrés.
	Les conditions d'arrêt sont,soit: on est arrivé à la fin de str ('\0' ou ptr >=end)
								soit: le pointeur memoire est dans une zone illégale (panic) */
	while (ptr < end && *ptr != '\0' && mem < no_go_zone) {
		/* Ici on "switch" *ptr et selon le "case",
			on effectue les actions correspondantes */
		/* Les numéros d'étape correspondes à la
		numérotation du flowchart fournis en annexe */
/*----*/if (isalpha(*ptr)) {/* ETAPE 1 ---------------- */
			//On rencontré une lettre, il s'agit donc
			//d'un appel à une nouvelle règle de la grammaire abnf
            res = matchedCommand(&ptr, end, &node_ptr);
		}
		else
/*----------------- ETAPE 3 --------------------------- */
			//Si on match un chiffre ou une * c'est qu'on va faire un boucle
			//On veut donc mettre a jour notre min et max nombre d'iteration (x et y)
			//Puis déterminer le morceau de règles à looper (boucler ?)
			//Avant d'appeller la fonction loopAlgoCalls()
		if (isdigit(*ptr) ) {/* Etape 30a */
			int x, y;
			y = MAX_ITER;
			
			/* Etape 31a */
			//Si c'est un chiffre on regarde s'il y a une étoile
			sscanf(ptr, "%d*%d", &x, &y);
			
			//Le code ci_dessous ne fonctionne que si le premier nombre est un chiffre (1 char)
			if (*(ptr + 1) != '*') //Si le char après le digit n'est pas une * (c'est le cas "N blabla")
				y = x;// On validera la boucle exactement N fois 

			//fonction cool
			res = Boucle1(x,y,&ptr,end,&node_ptr);

		} else if (*ptr=='*') {/* Etape 30b */
			
			int x, y;
			y = MAX_ITER;
			x = MIN_ITER;
			
			/* Etape 31b */
			//Si c'est une étoile on regarde si il y a un nombre après
			sscanf(ptr, "* %d", &y);//Il faudrait pas enlever l'espace ici (entre * et %d) ?
			
			//fonction cool
			res = Boucle1(x,y,&ptr,end,&node_ptr);
		
		
		}
		else
/*----*/if (*ptr == '%') { /* ETAPE 4 ----------------- */
			//On pointe sur un %, il faut valider (ou pas)
			//Le(s) code(s) ascii qui suivent le x
			ptr ++;
			//on verrifie qu'il y a bien un x (toujours le cas normalement)
			assert(*ptr == 'x');
			
			// A ce moment ptr pointe sur le 'x', donc ptr+3 pointe sur
			//le char juste après le code hexa (soit un '.', soit un '-', soit un ' ')
			if (*(ptr + 3) == '.') {
				/*Si il y a un point on est dans le cas:
					%xHH.HH.HH...
				*/
				
				uint8_t nb;
				char *mem_bak = mem;// sauvegarde du pointeur memoire (pas utile ?)
				//Tant qu'il rest des HH à scanner, que res est vrai, et que le pointeur mem est correct
				while(!isspace(*ptr) && res && mem < no_go_zone) {
					//on scan la valeur ascii
					sscanf(ptr + 1, "%hhx", &nb);
					//et si mem pointe un char qui valide le code nd
					res = (*mem == nb);
					// alors on incrémente mem
					mem += res;
					//on avance ptr de 3 pour aller lire le prochain (on saute ".HH")
                    ptr += 3;
				}
				
				if (!res) {//Si res est faux on restore mem (pas utile ?)
                    mem = mem_bak;
                }
            }
            else {
				/*Sinon on est dans le cas:
					%xHH-HH     ou	   %xHH
				*/
				
				uint8_t h1, h2;
				//on remplit h1 et h2 
				sscanf(ptr + 1,"%hhx-%hhx", &h1, &h2);
				
				//si il y a un tiret après HH
				if (*(ptr + 3) == '-')
                    ptr += 6;//On avance de 6 (on saute "xHH-HH")
				else{
                    ptr += 3;//Sinon on avance de 3 (on saute "xHH")
                    h2 = h1;// c'est pour le test juste après
                }
				
				// on check si *mem est compris entre h1 et h2
				res = ((*mem & MASK_OCTET) >= h1) && ((*mem & MASK_OCTET) <= h2);
				mem += res;//on incrémente mem adéquatement (si res == 1)
			}
			
		}
		else
/*----*/if (*ptr == '"') { /* ETAPE 5 ----------------- */
			// On match des guillemets, on veut donc comparer une chaine de charactères
			
			
			ptr++;//valeur à l'addresse de ptr égale ptr plus 1 point virgule
			
			//on cherche la longueur de la chaine (jusqu'au prochain guillemet)
			int longueur = distance_from(ptr, end, '"');
			
			//si mem + taille de la chaine sort de la zone mémoire attribué à la requête à parser
			if (mem + longueur >= no_go_zone)
				res = FALSE;//ON EXIT EN URGENCE
			else
			//sïnon on compare (case insensitive) la chaine avec la zone de taille longueur pointé par mem
				res = nocase_memcomp(ptr, mem, longueur);
			
			//si resultat est vrai
            if (res) {
				//dans la feuille de l'arbre correspondante, on copie le tag ("case_insensitnve striong")
                strcpy(node_ptr->tag, "case_insensitive_string");
                node_ptr->value = mem;
                node_ptr->len_value = longueur;
                mem += longueur;
				//on crée un nouveau frère vide
                node_ptr = addNodeAsBrother("", mem, 0, node_ptr);
            }
			//on incrémente ptr
			ptr += longueur + 1;

		}
		else
/*----*/if (*ptr == '(') {/*  ETAPE 9 ----------------- */
			//c'est une parenthese, on veut valider une et unique fois
			// son contenu
			ptr++;
			char *debut = ptr;
			goto_next(&ptr, end, ')');

			res = loopAlgoCalls(1, 1, debut, ptr, &node_ptr);

            ptr++;
		}
		else
/*----*/if (*ptr == '[') {/* ETAPE 10 ---------------- */
			//c'est des crochets
			//on veut valider ce qu'il y a entre crochet 0 ou 1 fois
			char *debut = ++ptr;
			goto_next(&ptr, end, ']');
			//on loop entre 0 et 1 fois
            res = loopAlgoCalls(0, 1, debut, ptr, &node_ptr);
			//res = TRUE
			ptr++;
		}
		else if (*ptr == ')') {
            //ERREUR
			//MAIS C'EST PAS POSSIBLE !!
			perror("Parsing error on parenthesis");
			/*  Mais si c'est possible avec la carte kiwi
				Pour les enfants de moins de 16 ans
				Et ceux qui l'accompagnent jusqu'à
				4 PERsonnes , payent tous moitié prix.
				MOITIé PRIX !? C'est PAS POSSIBle !
				Mais si c'est possible avec la carte kiwi
				Pour les enfants de moins de 16 ans
				Et ceux qui l'accompagnent jusqu'à
				4 PERsonnes , payent tous moitié prix.
				MOITIé PRIX !? C'est PAS POSSIBle !
				Mais si c'est possible avec la carte kiwi
				Pour les enfants de moins de 16 ans
				Et ceux qui l'accompagnent jusqu'à
				4 PERsonnes , payent tous moitié prix.
				MOITIé PRIX !? C'est PAS POSSIBle !
				Mais si c'est possible avec la carte kiwi
				Pour les enfants de moins de 16 ans
				Et ceux qui l'accompagnent jusqu'à
				4 PERsonnes , payent tous moitié prix.
				MOITIé PRIX !? C'est PAS POSSIBle !
				Mais si c'est possible avec la carte kiwi
				Pour les enfants de moins de 16 ans
				Et ceux qui l'accompagnent jusqu'à
				4 PERsonnes , payent tous moitié prix.
				MOITIé PRIX !? C'est PAS POSSIBle !
				Mais si c'est possible avec la carte kiwi
				Pour les enfants de moins de 16 ans
				Et ceux qui l'accompagnent jusqu'à
				4 PERsonnes , payent tous moitié prix.
				MOITIé PRIX !? C'est PAS POSSIBle !
				Mais si c'est possible avec la carte kiwi
				Pour les enfants de moins de 16 ans
				Et ceux qui l'accompagnent jusqu'à
				4 PERsonnes , payent tous moitié prix.
				MOITIé PRIX !? C'est PAS POSSIBle !
				Mais si c'est possible avec la carte kiwi
				Pour les enfants de moins de 16 ans
				Et ceux qui l'accompagnent jusqu'à
				4 PERsonnes , payent tous moitié prix.
				MOITIé PRIX !? C'est PAS POSSIBle !
				Mais si c'est possible avec la carte kiwi
				Pour les enfants de moins de 16 ans
				Et ceux qui l'accompagnent jusqu'à
				4 PERsonnes , payent tous moitié prix.
				MOITIé PRIX !? C'est PAS POSSIBle !
				*/
			exit(1);
		}

/* ---- ETAPE 2 ---------------------------------- */
/*Acol*/{/*ade inutile /!\ */
			if (*ptr=='/') {
				if (res) {
					//WHILE EXIT (return res)
					//En fait on met ptr = fin comme ça on sort directement du while
					ptr = end - 1;
				} else {
					res = TRUE;
					//on restore mem
					mem = sauvegarde_pointeur_memoire;
                    node_ptr=first_Child;
					//On encule tous les frères/enfants créés.
                    reset_first_child_queue(node_ptr);
				}
			} else {//Pas de slash (c'est un ET) donc on continue
				if (!res) {//par contre si le résultat est faux
					//Step n23 Goto '/'
					//on va au prochain /
					goto_next(&ptr, end, '/');
					if(ptr < end && *ptr == '/')//peut-être que le if n'est même pas nécessaire
                        ptr--;/* On décrémente ptr parcequ'il va être incrémenté juste après
								 et du coup à la prochaine boucle on exécutera bien le code
								 correspondant au "OU" ('/') */
				}
			}
/*Acol*/}/*ade inutile /!\ */
		
		ptr++;
	}//FIN DU WHILE()
	
	if (mem > no_go_zone) {
		perror("mem pointer out of range");
		res = FALSE;
	}
	

	return res;
}

/* A déplacer ou vous voulez (peut-etre tree.c)*/
int deleteEmptyBrothers(node* noeud) {
    if (noeud == NULL)
        return 1; // failsafe (si il n'y a pas de fr`re vide, ce qui ne devrait jamais arriver)
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
	

    char *debut_value = mem;


    //node *Current_node = addNodeAsChild(module, debut_value, 0, parent);
    node *first_child = addNodeAsChild("", debut_value, 0, parent);

    /* Procedure d'appel principale */
    valid = algo0(str, strlen(str), first_child);


/** IDEE: Refaire les dépendances [DONE]
		Ex: Algo0-> TRUE = renvoie un arbre valide (avec un élem vide)  <-c'est fait
					FALSE = renvoie un élément vide						<-idem
		construire->TRUE = arbre valide descendant de parent  	<-c'est fait
					FALSE = arbre vide parent->child = NULL		<-idem
**/

    if (valid) {
        if (deleteEmptyBrothers(first_child)) {
            parent->child = NULL;
        }
    } else {
        purgeTree(first_child);
        parent->child = NULL;
    }



	free(str);

	return valid; // return 1 si valide
}
