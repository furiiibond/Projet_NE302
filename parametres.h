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

// /* ############## DEBUG FLAGS ############## */
// /* #### */#define DEBUG				/* #### */
// /* #### */#define DEBUG_MEMVIEW		/* #### */
// /* #### */#define DEBUG_IO_CONSTRUIRE/* #### */
// /* ######################################### */

#endif