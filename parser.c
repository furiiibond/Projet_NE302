#include "parser.h"
#include "annexe.h"

/* ------------- ETAPE 1 ------------- */
int Matched_command(char **ptr) { //, char *end) {
	int Res = TRUE;
		#ifdef DEBUG
		printf("Matched a letter !\n");
		#endif
	char * debut = (*ptr);
	while(!isspace(*++(*ptr))) ;

	char command[50];
	int longueur = ((*ptr) - debut);
	memcpy(command, debut, longueur);
	command[longueur] = '\0';
		#ifdef DEBUG
			printf(BLUE "cmd_> %s\n"NC" ",command);
		#endif
	char* backup = mem;
/** ####### APPEL RECURSIF ##################*/
/** #####*/	Res = construire(command); /**####*/
/** #########################################*/
	if (!Res)
        mem = backup;
		#ifdef DEBUG
			printf("Res %s: ", command); Truth(Res);
		#endif
	return Res;
}

int Loop_algo_calls(int min, int max, char *debut, char *fin) {
	int Res = TRUE;
	char *backup = mem;
	char *backup2 = mem; // if we validate less time than min
	int i = 0;
	while (i < max && Res) {
		#ifdef DEBUG_MEMVIEW
		printf(CYN"tmp:%c\n"NC, *mem);
		#endif
		#ifdef DEBUG_IO_CONSTRUIRE
		printf(RED"Call>_algo0("NC);
        fflush(stdout);
		write(STDOUT_FILENO, debut, fin - debut);
		printf(RED")\n"NC);
		#endif
		Res = algo0(debut,fin - debut);
		if (Res)
            backup = mem;
		i += Res;
	}
    mem = backup;
	Res = (i >= min) && (i <= max);
	if (!Res)
        mem = backup2;
	#ifdef DEBUG
		printf("\texit loop with i=%d\n", i);
		printf("Res: "); Truth(Res);
	#endif
	return Res;
}
/*
int Matched_digit(char** ptr, char *end){
}

int Matched_star(char** ptr, char *end){
}

int Matched_crochet(){
}
*/

int algo0(char *str, int len) { // if error return 1 else 0
	char *ptr, *end;
	int Res = TRUE;
	ptr = str;
	end = str + len;
	#ifdef DEBUG
		printf(MAGENTA"Entree dans algo:[");
		fflush(stdout);
		write(STDOUT_FILENO, str, len);
		puts("]"NC);
	#endif

	while (ptr < end && *ptr != '\0' && mem < no_go_zone) {
		#ifdef DEBUG
			printf("Current scan: [%c]\n", *ptr);
		#endif

/*----*/if (isalpha(*ptr)) {/* ETAPE 1 ------------- */
			Res = Matched_command(&ptr);// end);
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
				sscanf(ptr,"%d*%d", &x, &y);
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
				while (!isspace(*++ptr)) ;
			}

			Loop_algo_calls(x, y, debut_chaine, ptr);

			if (*ptr == ')')
                ptr++;
		}
		else
/*----*/if (*ptr == '%') { /* ETAPE 4 ----------------- */
			#ifdef DEBUG
			printf("Matched '%%' !\n");
			#endif
			ptr++;
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
				while(!isspace(*ptr) && Res && mem < no_go_zone) {
					sscanf(ptr + 1, "%x", &nb);
					#ifdef DEBUG_MEMVIEW
						printf("\t\twhl ptr:%c\n",*(ptr));
						printf("\t\tmem: %c %x | ptr: %c %x\n", *mem, *mem, nb, nb);
					#endif
					Res = (*mem == nb);
					mem += Res;
                    ptr += 3;
				}
				if (!Res) {
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
				Res = ((*mem & MASK_OCTET) >= h1) && ((*mem & MASK_OCTET) <= h2);
				mem += Res;
			}
			#ifdef DEBUG
				printf("Res: "); Truth(Res);
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
				Res = FALSE;
			else
				Res = nocase_memcomp(ptr, mem, longueur);
			#ifdef DEBUG_MEMVIEW
                printf("\t[");
                fflush(stdout);
                write(STDOUT_FILENO, ptr, longueur);
                printf("]\n\t[");
                fflush(stdout);
                write(STDOUT_FILENO, mem,longueur);
                puts("]");
			#endif
			mem += longueur * Res;
			ptr += longueur + 1;
			#ifdef DEBUG
				printf("Res: "); Truth(Res);
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
			// printf("stop at:%c,next:%c\n", *ptr, *(ptr+1));
			Res = algo0(debut,ptr - debut);
			#ifdef DEBUG
				printf("Out of parenthesis; current:'%c'\n", *ptr);
			#endif
            ptr++;
		} else
/*----*/if (*ptr == '[') {/* ETAPE 10 ---------------- */
			char *debut_chaine = ++ptr;

			goto_next(&ptr, end, ']');

            int x = 0;
			int y = 1;

			Loop_algo_calls(x, y, debut_chaine, ptr);

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
				if (Res) {
					//WHILE EXIT (return Res)
					ptr = end - 1;
				} else {
					Res = TRUE;
				}
			} else {
				if (!Res) {
					//Step n23 Goto '/'
					goto_next(&ptr, end, '/');
					if(ptr < end && *ptr == '/')
                        Res = TRUE;
				}
			}
		}
		ptr++;
	}
	if (mem > no_go_zone) {
		perror("mem pointer out of range");
		Res = FALSE;
	}
	#ifdef DEBUG
		printf("Return Res:"); Truth(Res);
	#endif

	return Res;
}

int construire(char *module) {
	int Valid = 0;
	char *str = content(module);
	#ifdef DEBUG_IO_CONSTRUIRE
		printf(YELLOW"CONSTRUIRE %s\n"NC, module);
		printf(YELLOW"str:|%s|\n"NC, str);
	#endif
	//char* debut_value = mem;
	/* Procédure d'appel principale */
    Valid = algo0(str,strlen(str));

	#ifdef DEBUG_IO_CONSTRUIRE
		if (Valid)
            printf(GREEN"Nice"NC" (exiting:%s)\n", module);
		else
            printf(RED"Not nice"NC" (exiting:%s)\n", module);
		// write(STDOUT_FILENO,debut_value,mem-debut_value); printf("[EOL]\n");
		write(STDOUT_FILENO, mem, 20);
        printf("[EOL]\n");
	#endif

	free(str);

	return Valid; // return 1 si valide
}
