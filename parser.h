#ifndef PARSER_H
#define PARSER_H

// make it work with windows
#if defined(_WIN32)
    #include <stdint.h>
    typedef uint8_t u_int8_t;
#endif

#ifndef COLOR
#include "color.h"
#endif

#include "tree.h"
// Lib nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
// Lib annexes (not used yet ?)
#include <sys/types.h>
#include <unistd.h>
// Constants
#define TRUE 1
#define FALSE 0
#define MAX_ITER 0xFFFF
#define MIN_ITER 0

#define MASK_OCTET 0xFF
// #define DEBUG
// #define DEBUG_MEMVIEW

#define DEBUG_IO_CONSTRUIRE
// Global var, used to point the file to parse
char *mem;
char *no_go_zone;

// main fonction that needs to be called from parser
int construire(char *module, node *parent);

int algo0(char *str, int len, node *first_Child);

void reset_first_child_queue(node* noeud);

#endif
