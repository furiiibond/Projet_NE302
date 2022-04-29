#ifndef COLOR

/*
Color 0:desactivated / 1:activated
*/
#define COLOR 1

#if COLOR
	#define RED    "\033[0;31m" //rouge
	#define BLU   "\033[0;34m"  //bleu
	#define GRN  "\033[0;32m"   //vert
	#define YEL "\033[0;33m"    //jaune
	#define MAG "\e[0;35m"      //magenta
	#define CYN "\e[0;36m"      //cyan
	#define NC     "\033[0m"    //no color
#else
	#define RED    ""
	#define BLU   ""
	#define GRN  ""
	#define YEL ""
	#define MAG ""
	#define CYN ""
	#define NC     ""
#endif

#endif