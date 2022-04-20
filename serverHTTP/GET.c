#include "GET.h"

int traiter_GET(_Token *root){
	
	r  = searchTree(root, "request-line");
	r2 = searchTree(r->node, "method");
	tok=r2;
	int l;
	char *s;
	
	return 0;
}