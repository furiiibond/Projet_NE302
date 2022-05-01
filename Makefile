MAKEFLAGS += --no-print-directory

NAMES=LAPIERRE_GARDE_ROUGE_RANC
GROUPE=Groupe7

LIBPARS = libnotreparser
LIBREQ = librequest-0.5
SERVER = serverHTTP
IGNORE = EXEC

EXEC = server

default: export main

export:
	@echo "Copier coller la ligne ci-dessous"
	export LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(LIBPARS):$(LIBREQ)

main:
	cd $(SERVER) && \
	make notre   && \
	cp server .. && \
	make clean

sharedobject:
	cd $(LIBPARS) && make sharedobject

clean:
	rm -f server
	cd $(SERVER) && make clean

tar:
	tar -cvzf PROJET_NE302-$(GROUPE)-$(NAMES).tar.gz * --exclude="$(IGNORE)"