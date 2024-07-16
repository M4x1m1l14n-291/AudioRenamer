#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "playMusic.h"

void playMusic(char const *filepath, float volume)
{
    char vol[32];
    snprintf(vol, sizeof(vol), "volume=%f", volume);

    execlp("/bin/ffplay", "ffplay", "-v", "0", "-nodisp", "-autoexit", "-af", vol, filepath, NULL);
    exit(0);
}