LIBPARS = ../../libnotreparser
LIBREQ = ../../librequest-0.5


MODULES = OptionsParser RequestHandler GET ErrorHandler Headers
OBJ_PATH=objs/

FLAGS=-Wall -Wextra -g -I $(LIBPARS) -I $(LIBREQ)/api

objs: dir OptionsParser RequestHandler GET ErrorHandler
	for moduleName in $(MODULES) ; do \
		gcc $(FLAGS) -c $$moduleName/*.c  -o $(OBJ_PATH)$$moduleName.o ; \
	done
	@echo Done

dir:
	@mkdir -p $(OBJ_PATH)

clean:
	rm -rf $(OBJ_PATH)


.PHONY: OptionsParser RequestHandler GET ErrorHandler dir objs
