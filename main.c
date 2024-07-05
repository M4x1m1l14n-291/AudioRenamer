#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utils.h>
#include <signal.h>

#include "main.h"

float volume = 1.0;
char selectedDir[256] = "./";

int main(int argc, char const *argv[])
{
    readSettings();

    if (argc == 1)
    {
        playCurrDir();
    }
    else if (argc == 2 && !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))
    {
        printHelp();
    }
    else if (argc == 2)
    {
        play(argv[1]);
    }
    else if (argc == 3 && !strcmp(argv[1], "--directory") || !strcmp(argv[1], "-d"))
    {
        playDir(argv[2]);
    }
    else
    {
        printHelp();
    }
}

void play(char const *name)
{
    char input[512];

    printf("start playing: %s\n", name);
    pid_t soundPid = fork();
    if (soundPid < 0)
    {
        perror("forking error, trying again");
        play(name);
    }
    else if (soundPid == 0)
    {
        playMusic(name, volume);
    }

    printf("stop ('s')\n");
    scanf("%s", &input);

    if (!strcmp(input, "s") || !strcmp(input, "stop"))
    {
        kill(soundPid, SIGKILL);
    }

    waitpid(soundPid, NULL, 0);

    exit(0);
}

void playCurrDir()
{
    playDir("./");
}

void playDir(char const *dir)
{
    printf("WIP: scan directory: %s\n", dir);

    exit(0);
}

void readSettings()
{
    char volumeBuf[8];
    FILE *settingsFile;

    settingsFile = fopen("audioRename.txt", "r");
    if (settingsFile == NULL)
    {
        settingsFile = fopen("audioRename.txt", "w");
        printf("please enter volume (0.0 -> 1.0)\n");
        scanf("%f", &volume);
        sprintf(volumeBuf, "%f", volume);
        fprintf(settingsFile, volumeBuf);
        fprintf(settingsFile, selectedDir);
    }
    else
    {
        fgets(volumeBuf, 8, settingsFile);
        fgets(selectedDir, 255, settingsFile);
    }
    volume = strtof(volumeBuf, NULL);

    fclose(settingsFile);
}

void printHelp()
{
    printf("AUDIO RENAMER\n"
           "help…\n");
}