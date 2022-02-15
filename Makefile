MYNAME=LAPIERRE_Jean-Camille_HUGO_JEAN
IGNORE = old_generate.c.old
## Do not change below please
BIN = test
PREFIX=MIPS3
CC = gcc
FLAGS = -Wall
C_FILES = $(wildcard *.c)
OBJ_FILES = $(C_FILES:.c=.o)

all: $(OBJ_FILES)
	$(CC) $(FLAGS) -o $(BIN) $^ -lm

%.o: %.c
	$(CC) $(FLAGS) -c $^

clean:
	rm -f *.o *~ $(BIN) *.hex *.s *.pdf


# A useful command to deliver your project while keeping me sane
tar: clean
	dir=$$(basename $$PWD) && echo "compressing $(dir)" && cd .. && \
	tar cvfz "$(PREFIX)-$(MYNAME).tgz" \
	--transform="s,^$$dir,$(PREFIX)-$(MYNAME)," \
	--exclude="$(IGNORE)" "$$dir" --verbose --show-transformed-names
