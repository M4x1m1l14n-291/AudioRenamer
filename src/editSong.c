#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "editSong.h"

int editSong(char *path_and_name, char *name, char *file_type_ending)
{
    char input[16];

    char final_name[1024];
    strcpy(final_name, name);

    struct Metadata *Metadata = malloc(sizeof(struct Metadata));
    Metadata->favorite = 0;
    strcpy(Metadata->artist, "");
    strcpy(Metadata->title, "");

start:
    system("@cls||clear");
    printf("> editing: %s\n"
           "\n"
           "> artist   = %s\n"
           "> title    = %s\n"
           "> favorite = %s\n"
           "\n"
           "> set artist    (a):\n"
           "> set title     (t):\n"
           "> set favorite  (f):\n"
           "\n"
           "> custom rename (r):\n"
           "> cancel        (c):\n"
           "> done          (d):\n"
           "> ",
           final_name,
           Metadata->artist,
           Metadata->title,
           Metadata->favorite ? "true" : "false");

    // get user input
    fgets(input, 16, stdin);
    input[strlen(input) - 1] = '\0';

    if (!strcmp(input, "a") || !strcmp(input, "artist"))
    {
        printf("> enter artist: ");
        fgets(Metadata->artist, 256, stdin);
        Metadata->artist[strlen(Metadata->artist) - 1] = '\0';
    }
    else if (!strcmp(input, "t") || !strcmp(input, "title"))
    {
        printf("> enter title: ");
        fgets(Metadata->title, 256, stdin);
        Metadata->title[strlen(Metadata->title) - 1] = '\0';
    }
    else if (!strcmp(input, "f") || !strcmp(input, "fav") || !strcmp(input, "favorite"))
        Metadata->favorite = !Metadata->favorite;

    else if (!strcmp(input, "r") || !strcmp(input, "ren") || !strcmp(input, "rename"))
    {
    }
    else if (!strcmp(input, "c") || !strcmp(input, "cancel"))
        goto end;

    else if (!strcmp(input, "d") || !strcmp(input, "done"))
    {
        goto end;
    }

    goto start;

end:
    free(Metadata);
    return 0;
}