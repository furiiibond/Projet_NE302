
# En fonction de l'emplacement des fichiers so faire
# export LD_LIBRARY_PATH=.
# ou
# export LD_LIBRARY_PATH=../libnotreparser/:../librequest-0.5/ <--bypass symbolic links

export: notre
	@echo "Copier coller la ligne ci-dessous"
	export LD_LIBRARY_PATH=LD_LIBRARY_PATH:../libnotreparser/:../librequest-0.5/

all:
	gcc -I ../libparser -I ../librequest-0.5/api  -L ../libparser -L ../librequest-0.5 -o server server.c -lparser -lrequest

notre:
	gcc -I ../libnotreparser -I ../librequest-0.5/api  -L ../libnotreparser -L ../librequest-0.5 -o server server.c -lnotreparser -lrequest 

clean:
	rm -f server

.PHONY: export all notre clean
