#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renameSong.h"

// #define CLEAR //
#define CLEAR system("@cls||clear");

void printEditStart(char *name)
{
    printf("> editing : %s\n\n"
           "> set name            (n):\n"
           "> set artist          (a):\n"
           "> set genre           (g):\n"
           "> set custom filename (f):\n"
           "> cancel              (c):\n"
           "> ",
           name);
}

void editSong(char const *filepath, char *name)
{
    char newName[512] = {""};
    char input[16] = {""};

    CLEAR
    printEditStart(name);

    fgets(input, 16, stdin);
    input[strlen(input) - 1] = '\0';

    if (!strcmp(input, "n") || !strcmp(input, "name"))
    {
        /* code */
    }
    else if (!strcmp(input, "a") || !strcmp(input, "artist") || !strcmp(input, "art"))
    {
        /* code */
    }
    else if (!strcmp(input, "g") || !strcmp(input, "genre"))
    {
        /* code */
    }
    else if (!strcmp(input, "f") || !strcmp(input, "filename") || !strcmp(input, "file"))
    {
        /* code */
    }
    else if (!strcmp(input, "c") || !strcmp(input, "cancel"))
        return;
}
