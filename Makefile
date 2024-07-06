files := main.c playMusic.c linkedList.c

build:
	gcc $(files) -o AudioRenamer

clean:
	rm AudioRenamer
