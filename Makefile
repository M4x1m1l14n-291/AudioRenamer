# compliler
CC := gcc

# compiler flags
CFLAGS := -g -Wall -Werror -Wextra

# files to include
FILES := main.c playMusic.c linkedList.c

# executable name
NAME := AudioRenamer


all: clean build run

build:
	$(CC) $(FILES) -o $(NAME)

debug:
	$(CC) $(CFLAGS) $(FILES) -o $(NAME)

run:
	./$(NAME)

clean:
	rm $(NAME)
