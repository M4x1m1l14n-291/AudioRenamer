# compliler
CC := gcc

# compiler flags
CFLAGS := -Wall -Werror -Wextra

# files to include
FILES := src/main.c src/playMusic.c src/linkedList.c src/editSong.c

# executable name
NAME := AudioRenamer


all: clean build run

build:
	$(CC) -o $(NAME) $(FILES)

debug_build:
	$(CC) -o $(NAME) -g $(CFLAGS) $(FILES)

debug: clean debug_build
	gdb $(NAME) -tui

run:
	./$(NAME)

clean:
	rm -f $(NAME)
