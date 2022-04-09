
all : librequest.so sock 

sock : librequest.so main.o 
	gcc -g -o sock main.o -L. -lrequest

%.o: %.c 
	gcc -fPIC -g -I../../abnf -I./api -c $< 

main.o: main.c
	gcc -fPIC -g -I./api -c $< 

librequest.so : socket.o 
	ld -shared -Bsymbolic -Bsymbolic-function -soname librequest.so -o librequest.so ./parser/librequest.o ./socket.o 
#	gcc -Wall -shared -I./abnf -I ./api -o librequest.so socket.o ./parser/librequest.o 

clean : 
	rm -rf rfc*.c 
	rm -rf rfc*.h 
	rm -rf httpparser
	rm -rf *.o 
	rm -rf *.so 

