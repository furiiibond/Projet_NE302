httpparser: main.o libparser.so 
	gcc $(CCFLAGS) -o $@ $< -L. -lparser 

main.o: main.c 
	gcc $(CCFLAGS) -c $< -o $@

.PRECIOUS: %.c %.h

all: httpparser

clean : 
	rm -rf httpparser
	rm -rf *.o 


