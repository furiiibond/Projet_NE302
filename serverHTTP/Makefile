
# En fonction de l'emplacement des fichiers so faire
# export LD_LIBRARY_PATH=.
# ou
# export LD_LIBRARY_PATH=../libnotreparser/:../librequest-0.5/ <--bypass symbolic links

LIBPARS = ../libnotreparser
LIBREQ = ../librequest-0.5
SERVER = .

SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)

OBJ_PATH=objs/
#objs/

CC=gcc
FLAGS=-Wall -Wextra -g -I $(LIBPARS) -I $(LIBREQ)/api

EXEC = server

all: export notre

run: notre
	clear
	./server

export:
	@echo "Copier coller la ligne ci-dessous"
	export LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(LIBPARS):$(LIBREQ)

# Creating directories
dir:
	@mkdir -p $(OBJ_PATH)

prof:
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -o server server.c -lparser -lrequest

notre: debug dir $(OBJS) link

link:
	$(CC) $(FLAGS) -o $(EXEC) $(OBJ_PATH)*.o -L $(LIBPARS) -L $(LIBREQ) -lnotreparser -lrequest  -lmagic

#server.o: server.c
#	gcc -I $(LIBPARS) -I $(LIBREQ)/api -o server.o -c $(SERVER)/server.c

debug:
	@echo TEST DEBUG OBJ FILES
	@echo "$(SRC) | $(OBJS)"
	@echo OK

# All objects rule
%.o: %.c
	$(CC) $(FLAGS) -c $^ -o $(OBJ_PATH)$@

# Cleaning the files
clean:
	@echo cleaning objects and exec
	rm -rf $(OBJ_PATH)*.o $(EXEC)
	rmdir --ignore-fail-on-non-empty $(OBJ_PATH)

.PHONY: export all notre clean
