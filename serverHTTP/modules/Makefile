LIBPARS = ../../libnotreparser
LIBREQ = ../../librequest-0.5
modules = OptionsParser RequestHandler GET ErrorHandler


OBJ_PATH=objs/

FLAGS=-Wall -Wextra -g -I $(LIBPARS) -I $(LIBREQ)/api

objs: dir OptionsParser RequestHandler GET ErrorHandler
	for moduleName in $(modules) ; do \
		gcc $(FLAGS) -c $$moduleName/*.c  -o $(OBJ_PATH)$$moduleName.o ; \
	done
	@echo Done

dir:
	@mkdir -p $(OBJ_PATH)

clean:
	rm -rf $(OBJ_PATH)


.PHONY: OptionsParser RequestHandler GET ErrorHandler dir objs
