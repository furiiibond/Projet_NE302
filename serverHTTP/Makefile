
LIBPARS = ../libnotreparser
LIBREQ = ../librequest-0.5
SERVER = .

SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
MODULES=modules

OBJ_PATH=objs/
MOD_OBJ=$(MODULES)/objs/
#objs/

CC=gcc
FLAGS=-Wall -Wextra -g -I $(LIBPARS) -I $(LIBREQ)/api

EXEC = server

# Par defaut
all: export notre

# compile et exécute
run: notre
	@clear
	./$(EXEC)

# Affiche la ligne à exporter (pour les bibliothèque)
export:
	@echo "Copier coller la ligne ci-dessous"
	export LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(LIBPARS):$(LIBREQ)
	
#-----------------------------------------------------------------------------------------#
## Compilation de notre projet
notre: dir $(OBJS) modules link


# Creating directories
dir:
	@mkdir -p $(OBJ_PATH)


# Liaison des .o pour créer l'executable
link:
	$(CC) $(FLAGS) -o $(EXEC) $(OBJ_PATH)*.o $(MOD_OBJ)*.o  -L $(LIBPARS) -L $(LIBREQ) -lnotreparser -lrequest  -lmagic


# All objects rule
%.o: %.c
	$(CC) $(FLAGS) -c $^ -o $(OBJ_PATH)$@

# compilation des modules (en fichiers objets)
modules:
	@echo compilation des modules
	cd $(MODULES) && make objs

#-----------------------------------------------------------------------------------------#

# Cleaning the files
clean:
	@echo cleaning objects and exec
	rm -rf $(OBJ_PATH) $(EXEC)
	cd modules && make clean



.PHONY: export all notre clean modules

