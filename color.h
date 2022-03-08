/*
Color 0:activated / 1:desactivated
*/

#define COLOR 1

#if COLOR
	#define RED    "\033[0;31m"
	#define BLUE   "\033[0;34m"
	#define GREEN  "\033[0;32m"
	#define YELLOW "\033[0;33m"
	#define MAGENTA "\e[0;35m"
	#define CYN "\e[0;36m"
	#define NC     "\033[0m"
#else
	#define RED    ""
	#define BLUE   ""
	#define GREEN  ""
	#define YELLOW ""
	#define MAGENTA ""
	#define CYN ""
	#define NC     ""
#endif

