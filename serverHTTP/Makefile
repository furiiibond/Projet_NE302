
# En fonction de l'emplacement des fichiers so faire
# export LD_LIBRARY_PATH=.
# ou
# export LD_LIBRARY_PATH=../libnotreparser/:../librequest-0.5/ <--bypass symbolic links

LIBPARS = ../libnotreparser
LIBREQ = ../librequest-0.5
SERVER = .

EXEC = server

all: export notre

export:
	@echo "Copier coller la ligne ci-dessous"
	export LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(LIBPARS):$(LIBREQ)

prof:
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -o server server.c -lparser -lrequest

notre:
	gcc -I $(LIBPARS) -I $(LIBREQ)/api  -L $(LIBPARS) -L $(LIBREQ) -o $(EXEC) $(SERVER)/server.c -lnotreparser -lrequest 

clean:
	rm -f server

.PHONY: export all notre clean
