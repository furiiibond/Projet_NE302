#ifndef PARAMETRES
#define PARAMETRES
/* Fichier dans lequel on définiera tous les paramètres pour moins de confusion */

//ABNF Gramm file
#define ABNF_RULES "fullrfc.abnf" //Fichier contenant les règles de grammaire
#define FIRST_TAG "HTTP-message" //Premier Tag pour l'appel récursif

//Maximum rule line size
#define CONTENT_LEN 1000

//Maximum Tag(/commande) size (le premier mot d'une ligne de la grammaire abnf)
#define TAG_LEN 50

// nb of char displayed by line in display_tree()
#define DISPLAY_MAX 30
//Profondeur maximale de la fonction display_tree()
#define MAX_DEPTH 9

// If set to 1, will display the tree produced
#define DISPLAY_TREE 0

//
#define PRINT_VALID 0

/* ############## DEBUG FLAGS ############## */
/* #### */	#define DEBUG 0// define every DEBUG Flag
/* #### ----DEBUG Algo0 cases  ---*/
/* #### */	#define DEBUG_MATCHED_COMMAND 0//Debug la fonction matched command
/* #### */	#define DEBUG_MATCHED_LOOP 0// Debug la fonction Boucle Algo (et aussi la partie is digit, is *, is (, is [ ?????????????????)
/* #### */	#define DEBUG_MATCHED_PERCENT 0// Debug la fonction Michel_pourcent
/* #### */	#define DEBUG_MATCHED_STRING 0// Debug la fonction Cocktel_Molotov
/* #### */	#define DEBUG_ETAPE2 0// Debug la fin de la boucle (etape2)
/* #### ----DEBUG call and return of main functions ---*/
/* #### */	#define DEBUG_CONSTRUIRE_IN 0//Display the string passed to construire, along with its matched abnf rule
/* #### */	#define DEBUG_CONSTRUIRE_OUT 0// Display res when construire return
/* #### */	#define DEBUG_ALGO_IN 0//Display the string passed to algo0
/* #### */	#define DEBUG_ALGO_OUT 0// Display res when algo0 return, along with the string with which algo0 was called
/* #### ----DEBUG call and return of main functions ---*/
/* #### */	#define DEBUG_TREE_GROWTH 0//Display Tree at every step in the process ("step" yet to be define)
/* #### */	//(step>)can be split in DEBUG_TREE_CONSTRUIRE / DEBUG_TREE_ALGO / DEBUG_TREE_LOOP
/* #### */	#define DEBUG_TREE_MALLOC 0//Every time there's a malloc for a tree part, there's a printf
/* #### */	#define DEBUG_TREE_FREE 0//  Every time there's a free	for a tree part, there's a printf
/* ######################################### */

#endif
