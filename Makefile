CC = gcc
CFLAGS = -Iinclude -Wall -Werror -g
SRC = src/main.c src/CEthreads.c
OBJ = $(SRC:.c=.o)
EXEC = clone_example

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
