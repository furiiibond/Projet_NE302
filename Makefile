

LIBPARS = libnotreparser
LIBREQ = librequest-0.5
SERVER = serverHTTP

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

clean:
	rm -f server
	cd $(SERVER) && make clean

#gcc -I $(LIBPARS) -I $(LIBREQ)/api  -L $(LIBPARS) -L $(LIBREQ) -o $(EXEC) $(SERVER)/server.c -lnotreparser -lrequest 