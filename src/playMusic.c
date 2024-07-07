#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "playMusic.h"

void playMusic(char const *path, float volume)
{
    char vol[32];
    snprintf(vol, sizeof(vol), "volume=%f", volume);

    execlp("/bin/ffplay", "ffplay", "-v", "0", "-nodisp", "-autoexit", "-af", vol, path, NULL);
    exit(0);
}