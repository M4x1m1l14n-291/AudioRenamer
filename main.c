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

struct settings Settings = {1.0f, "", ""};
struct ListNode *Files;

int main()
{
    readSettings();
    printStart();

    // playDir(Settings.directory);
}

void play(char const *filename)
{
    char *input = malloc(512);

    if (filename[0] == '.')
        return;

    printf("start playing: %s\n", filename);
    pid_t soundPid = fork();
    if (soundPid < 0)
    {
        perror("forking error, trying again");
        play(filename);
    }
    else if (soundPid == 0)
    {
        playMusic(filename, Settings.volume);
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

int isAudioFile(char *filename)
{
    int len = strlen(filename);
    if (len < 5)
        return false;

    if (!strncmp(filename + len - 4, ".mp3", 4))
        return true;
    if (!strncmp(filename + len - 4, ".m4a", 4))
        return true;

    return false;
}

void readSettings()
{
    char *homeDir = getenv("HOME");
    if (homeDir == NULL)
        return;

    char dirBuf[strlen(homeDir) + 22];
    snprintf(dirBuf, sizeof(dirBuf), "%s/.config/audioRename", homeDir);

    FILE *settingsFile;
    settingsFile = fopen(dirBuf, "r");
    if (settingsFile == NULL)
        createSettingsFile(settingsFile, dirBuf);
    else
    {
        fread(&Settings.volume, 1, sizeof(Settings.volume), settingsFile);
        fread(&Settings.directory, 1, sizeof(Settings.directory), settingsFile);
        fread(&Settings.lastPlayedSong, 1, sizeof(Settings.lastPlayedSong), settingsFile);

        fclose(settingsFile);
    }
}

void createSettingsFile(FILE *settingsFile, char *dirBuf)
{
    settingsFile = fopen(dirBuf, "w");
    printf("please enter volume (0.0 -> 1.0)\n");
    scanf("%f", &Settings.volume);

    fwrite(&Settings.volume, 1, sizeof(Settings.volume), settingsFile);
    fwrite(&Settings.directory, 1, sizeof(Settings.directory), settingsFile);
    fwrite(&Settings.lastPlayedSong, 1, sizeof(Settings.lastPlayedSong), settingsFile);

    fclose(settingsFile);
}

void printStart()
{
    printf("AUDIO RENAMER\n"
           "help\n");
}