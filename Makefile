MYNAME=LAPIERRE_Jean-Camille_HUGO_JEAN

BIN = exec
PREFIX=http
CC = gcc
FLAGS = -Wall -Wextra -Werror -g
C_FILES = $(wildcard *.c)
OBJ_FILES = $(C_FILES:.c=.o)

all: $(OBJ_FILES)
	$(CC) $(FLAGS) -o $(BIN) $^ 

%.o: %.c
	$(CC) $(FLAGS) -c $^

clean:
	rm -f *.o $(BIN)
	
tar: clean
	dir=$$(basename $$PWD) && echo "compressing $(dir)" && cd .. && \
	tar cvfz "$(PREFIX)-$(MYNAME).tgz" \
	--transform="s,^$$dir,$(PREFIX)-$(MYNAME)," \
	--exclude="$(IGNORE)" "$$dir" --verbose --show-transformed-names
