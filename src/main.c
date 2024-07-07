#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utils.h>
#include <signal.h>
#include <dirent.h>

#include <ctype.h>

#include "main.h"
#include "linkedList.h"
#include "playMusic.h"

struct settings Settings = {1.0f, ".", ""};
struct ListNode *Files;

FILE *settingsFile;

int main()
{
    readSettings();
    printStart();

    playDir(Settings.directory);

    fclose(settingsFile);
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
    Files = malloc(sizeof(struct ListNode));

    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("error opening path");
        return;
    }

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
        return 0;

    if (!strncmp(filename + len - 4, ".mp3", 4))
        return 1;
    if (!strncmp(filename + len - 4, ".m4a", 4))
        return 1;

    return 0;
}

void readSettings()
{
    char *homeDir = getenv("HOME");
    if (homeDir == NULL)
        return;

    char dirBuf[strlen(homeDir) + 22];
    snprintf(dirBuf, sizeof(dirBuf), "%s/.config/audioRename", homeDir);

    if ((settingsFile = fopen(dirBuf, "r")) == NULL)
        createSettingsFile(dirBuf);
    else
        loadSettings();
}

void createSettingsFile(char *dirBuf)
{
    settingsFile = fopen(dirBuf, "w");
    printf("please enter volume (0.0 -> 1.0)\n");
    scanf("%f", &Settings.volume);

    saveSettings();
}

void saveSettings()
{
    fwrite(&Settings.volume, 1, sizeof(Settings.volume), settingsFile);
    fwrite(&Settings.directory, 1, sizeof(Settings.directory), settingsFile);
    fwrite(&Settings.lastPlayedSong, 1, sizeof(Settings.lastPlayedSong), settingsFile);
}

void loadSettings()
{
    fread(&Settings.volume, 1, sizeof(Settings.volume), settingsFile);
    fread(&Settings.directory, 1, sizeof(Settings.directory), settingsFile);
    fread(&Settings.lastPlayedSong, 1, sizeof(Settings.lastPlayedSong), settingsFile);
}

void printStart()
{
    printf("AUDIO RENAMER\n");
}