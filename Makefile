MAKEFLAGS += --no-print-directory

NAMES=LAPIERRE_GARDE_ROUGE_RANC
GROUPE=Groupe7

LIBPARS = libnotreparser
LIBREQ = librequest-0.5
SERVER = serverHTTP
IGNORE = EXEC

EXEC = server

default: export main

# Affiche la ligne à exporter
export:
	@echo "Copier coller la ligne ci-dessous"
	export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIBPARS):$(LIBREQ)

# Configure les hostname de notre configuration par défaut
hosts:
	sudo sh -c 'echo "127.0.0.1	sitecool.com" >> /etc/hosts'
	sudo sh -c 'echo "127.0.0.1	numerodos.tv" >> /etc/hosts'

# Compile le serveur
main:
	cd $(SERVER) && \
	make notre   && \
	cp server .. && \
	make clean

# Recompile la libnotreparser
sharedobject:
	cd $(LIBPARS) && make sharedobject

# Clean l'executable
clean:
	rm -f server
	cd $(SERVER) && make clean

# Créée une distribution tar
tar:
	tar -cvzf PROJET_NE302-$(GROUPE)-$(NAMES).tar.gz * --exclude="$(IGNORE)"
