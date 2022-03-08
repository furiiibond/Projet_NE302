#ifndef ABNF_H
#define ABNF_H

#include <string.h>

int httpMessage(char* req, int len);
int debut(char* req, int len);
int body(char* req, int len);
int mot(char* req, int len);
int nombre(char* req, int len);
int separateur(char* req, int len);
int ponct(char* req, int len);
int fin(char *req, int len);

#endif