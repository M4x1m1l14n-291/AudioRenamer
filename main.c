#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utils.h>
#include <signal.h>
#include <dirent.h>
#include <stdbool.h>

#include "main.h"
#include "linkedList.h"
#include "playMusic.h"

float volume = 1.0;
char selectedDir[256] = ".";

struct ListNode *Files;

int main()
{
    readVolume();

    printHelp();

    playDir(selectedDir);
}

void play(char const *name)
{
    char *input = malloc(512);

    if (name[0] == '.')
        return;

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
    scanf("%s", input);

    if (!strcmp(input, "s") || !strcmp(input, "stop"))
    {
        kill(soundPid, SIGKILL);
    }

    waitpid(soundPid, NULL, 0);
}

void playDir(char const *dir)
{
    scanDirectory(dir);

    struct ListNode *item = Files;

    for (; item; item = item->next)
        play(item->name);

    exit(0);
}

// Scan the 'path' directory
void scanDirectory(char const *path)
{
    printf("scanning directory %s\n\n", path);

    Files = malloc(sizeof(struct ListNode));
    strcpy(Files->name, "..");
    Files->next = NULL;

    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL)
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.' || entry->d_type == 4 || !isAudioFile(entry->d_name))
            continue;

        appendNode(Files, entry->d_name);
    }

    // printList(Files);
    closedir(dir);
}

// Check if filename ends with m4a or mp3
int isAudioFile(char *name)
{
    int len = strlen(name);
    if (len < 5)
        return false;

    if (!strncmp(name + len - 4, ".mp3", 4))
        return true;
    if (!strncmp(name + len - 4, ".m4a", 4))
        return true;

    return false;
}

// Read audio volume from config file
void readVolume()
{
    char *homeDir = getenv("HOME");
    if (homeDir == NULL)
        return;

    char dirBuf[strlen(homeDir) + 21];
    snprintf(dirBuf, sizeof(dirBuf), "%s/.config/audioRename", homeDir);

    char volumeBuf[8];
    FILE *settingsFile;

    settingsFile = fopen(dirBuf, "r");
    if (settingsFile == NULL)
    {
        settingsFile = fopen(dirBuf, "w");
        printf("please enter volume (0.0 -> 1.0)\n");
        scanf("%f", &volume);

        sprintf(volumeBuf, "%.2f", volume);
        fprintf(settingsFile, volumeBuf);
    }
    else
        fgets(volumeBuf, 8, settingsFile);

    volume = strtof(volumeBuf, NULL);

    fclose(settingsFile);
}

void printHelp()
{
    printf("AUDIO RENAMER\n"
           "help…\n");
}