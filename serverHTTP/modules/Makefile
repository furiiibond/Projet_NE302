LIBPARS = ../../libnotreparser
LIBREQ = ../../librequest-0.5


MODULES = OptionsParser RequestHandler Methodes ErrorHandler Headers Semantique
OBJ_PATH=objs/

FLAGS=-Wall -Wextra -g -I $(LIBPARS) -I $(LIBREQ)/api

objs: dir
	for moduleName in $(MODULES) ; do \
		gcc $(FLAGS) -c $$moduleName/*.c  -o $(OBJ_PATH)$$moduleName.o ; \
	done
	@echo Done

dir:
	@mkdir -p $(OBJ_PATH)

clean:
	rm -rf $(OBJ_PATH)


.PHONY: OptionsParser RequestHandler Methodes ErrorHandler Semantique dir objs
