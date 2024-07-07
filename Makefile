# compliler
CC := gcc

# compiler flags
CFLAGS := -Wall -Werror -Wextra

# files to include
FILES := src/main.c src/playMusic.c src/linkedList.c

# executable name
NAME := AudioRenamer


all: clean build run

build:
	$(CC) $(FILES) -o $(NAME)

debug:
	$(CC) -g $(CFLAGS) $(FILES) -o $(NAME)

run:
	./$(NAME)

clean:
	rm -f $(NAME)
#	rm -f /home/maximilian/.config/audioRename
