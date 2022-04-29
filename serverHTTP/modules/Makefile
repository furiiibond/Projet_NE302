LIBPARS = ../../libnotreparser
LIBREQ = ../../librequest-0.5

OBJ_PATH=objs/

FLAGS=-Wall -Wextra -g -I $(LIBPARS) -I $(LIBREQ)/api

objs: dir OptionsParser RequestHandler GET ErrorHandler
	@echo Done
	

dir:
	@mkdir -p $(OBJ_PATH)


OptionsParser:
	gcc $(FLAGS) -c $@/*.c  -o $(OBJ_PATH)$@.o

RequestHandler:
	gcc $(FLAGS) -c $@/*.c  -o $(OBJ_PATH)$@.o 

GET:
	gcc $(FLAGS) -c $@/*.c  -o $(OBJ_PATH)$@.o	
	
ErrorHandler:
	gcc $(FLAGS) -c $@/*.c  -o $(OBJ_PATH)$@.o


clean:
	rm -rf $(OBJ_PATH)


.PHONY: OptionsParser RequestHandler GET ErrorHandler dir objs
