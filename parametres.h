#ifndef PARAMETRES
#define PARAMETRES
/* Fichier dans lequel on définiera tous les paramètres pour moins de confusion */

//ABNF Gramm file
#define ABNF_RULES "allrfc.abnf" //Fichier contenant les règles de grammaire
#define FIRST_TAG "HTTP-message" //Premier Tag pour l'appel récursif

//Maximum rule line size
#define CONTENT_LEN 1000

//Maximum Tag(/commande) size (le premier mot d'une ligne de la grammaire abnf)
#define TAG_LEN 50

// nb of char displayed by line in display_tree()
#define DISPLAY_MAX 30

/* ############## DEBUG FLAGS ############## */
/* #### */	#define DEBUG // define every DEBUG Flag
/* #### ----DEBUG Algo0 cases  ---*/
/* #### */	#define DEBUG_MATCHED_COMMAND //Debug la fonction matched command
/* #### */	#define DEBUG_MATCHED_LOOP // Debug la fonction Boucle Algo (et aussi la partie is digit, is *, is (, is [ ?????????????????)
/* #### */	#define DEBUG_MATCHED_PERCENT // Debug la fonction Michel_pourcent
/* #### */	#define DEBUG_MATCHED_STRING // Debug la fonction Cocktel_Molotov
/* #### */	#define DEBUG_ETAPE2 // Debug la fin de la boucle (etape2)
/* #### ----DEBUG call and return of main functions ---*/
/* #### */	#define DEBUG_CONSTRUIRE_IN //Display the string passed to construire, along with its matched abnf rule
/* #### */	#define DEBUG_CONSTRUIRE_OUT // Display res when construire return
/* #### */	#define DEBUG_ALGO_IN //Display the string passed to algo0
/* #### */	#define DEBUG_ALGO_OUT // Display res when algo0 return, along with the string with which algo0 was called
/* #### ----DEBUG call and return of main functions ---*/
/* #### */	#define DEBUG_TREE_GROWTH //Display Tree at every step in the process ("step" yet to be define)
/* #### */	//(step>)can be split in DEBUG_TREE_CONSTRUIRE / DEBUG_TREE_ALGO / DEBUG_TREE_LOOP
/* #### */	#define DEBUG_TREE_MALLOC //Every time there's a malloc for a tree part, there's a printf
/* #### */	#define DEBUG_TREE_FREE //  Every time there's a free	for a tree part, there's a printf
/* ######################################### */

#endif