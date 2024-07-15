# compliler
CC := gcc

# compiler flags
CFLAGS := -Wall -Werror -Wextra

# files to include
FILES := src/main.c src/playMusic.c src/linkedList.c src/renameSong.c

# executable name
NAME := AudioRenamer


all: clean build run

build:
	$(CC) $(FILES) -o $(NAME)

debug_build:
	$(CC) -g $(CFLAGS) $(FILES) -o $(NAME)

debug: clean debug_build
	gdb $(NAME) -tui

run:
	./$(NAME)

clean:
	rm -f $(NAME)
