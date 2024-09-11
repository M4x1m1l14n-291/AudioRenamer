#include "editSong.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int editSong(char *path, char *path_and_name, char *name, char *file_type_ending) {
    char input[16];
    char final_name[256] = "";

    struct Metadata *Metadata = malloc(sizeof(struct Metadata));
    Metadata->favorite = 0;
    strcpy(Metadata->artist, "");
    strcpy(Metadata->title, "");

reset_name: {}
    char short_name[256];
    strcpy(short_name, name);
    short_name[strlen(short_name) - 4] = '\0';

start:
    system("@cls||clear");
    printf(
        "> editing: %s%s\n"
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
        short_name, file_type_ending, Metadata->artist, Metadata->title, Metadata->favorite ? "true" : "false");

    // get user input
    fgets(input, 16, stdin);
    input[strlen(input) - 1] = '\0';

    if (!strcmp(input, "a") || !strcmp(input, "artist")) {
        printf("> enter artist: ");
        fgets(Metadata->artist, 256, stdin);
        Metadata->artist[strlen(Metadata->artist) - 1] = '\0';
    } else if (!strcmp(input, "t") || !strcmp(input, "title")) {
        printf("> enter title: ");
        fgets(Metadata->title, 256, stdin);
        Metadata->title[strlen(Metadata->title) - 1] = '\0';
    } else if (!strcmp(input, "f") || !strcmp(input, "fav") || !strcmp(input, "favorite"))
        Metadata->favorite = !Metadata->favorite;

    else if (!strcmp(input, "r") || !strcmp(input, "ren") || !strcmp(input, "rename")) {
        printf("> WIP! \n");
        /*
        printf("> enter custom file name: ");
        fgets(short_name, 256, stdin);
        short_name[strlen(short_name) - 1] = '\0';
        */
    } else if (!strcmp(input, "c") || !strcmp(input, "cancel"))
        goto end;

    else if (!strcmp(input, "d") || !strcmp(input, "done")) {
        if (strlen(Metadata->artist) > 0 && strlen(Metadata->title) > 0 && Metadata->favorite)
            snprintf(final_name, 256, "%s - %s - FAV", Metadata->artist, Metadata->title);

        else if (strlen(Metadata->artist) > 0 && strlen(Metadata->title) > 0)
            snprintf(final_name, 256, "%s - %s", Metadata->artist, Metadata->title);

        else if (strlen(Metadata->artist) == 0 && strlen(Metadata->title) == 0 && !Metadata->favorite)
            goto reset_name;

        strcat(final_name, file_type_ending);
        printf(
            "> new name = %s\n"
            "> confirm changes? (c): ",
            final_name);
        fgets(input, 16, stdin);
        input[strlen(input) - 1] = '\0';

        if (!strcmp(input, "c") || !strcmp(input, "confirm")) {
            char final_path_and_name[512];
            sprintf(final_path_and_name, "%s%s", path, final_name);

            rename(path_and_name, final_path_and_name);

            strcpy(name, final_name);
            free(Metadata);
            return 1;
        } else
            goto reset_name;
    }

    goto start;

end:
    free(Metadata);
    return 0;
}