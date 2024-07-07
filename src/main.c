#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>

#include "main.h"
#include "linkedList.h"
#include "playMusic.h"

struct settings Settings = {1.0f, "", ""};
struct ListNode *Files;

FILE *settingsFile;

int running = 1;

int main()
{
    readSettings();
    printf("AUDIO RENAMER\n");

    while (running)
    {
        char inp[256];

        printf("change volume           (v):\n"
               "set or change directory (d):\n");
        if (strlen(Settings.directory) > 0 && strlen(Settings.lastPlayedSong) > 0)
            printf("play saved song (p):\n");
        else
            printf("start playing           (p):\n");

        scanf("%s", inp);

        if (!strcmp(inp, "v"))
        {
            printf("volume=%.2f\n"
                   "enter volume (0.0 -> 1.0):\n",
                   Settings.volume);
            scanf("%f", Settings.volume);
            saveSettings();
        }
        else if (!strcmp(inp, "d"))
        {
            printf("directory=%s\n"
                   "enter directory:\n",
                   Settings.directory);
            scanf("%s", Settings.directory);
            saveSettings();
        }
        else if (!strcmp(inp, "p"))
        {
            playDir();
        }
    }

    fclose(settingsFile);
}

void play(char const *filename)
{
    if (filename[0] == '.')
        return;

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

    char inp;
    printf("stop (s):\n");
    scanf("%s", inp);

    if (inp == 's')
        kill(soundPid, SIGKILL);

    waitpid(soundPid, NULL, 0);
}

void playDir()
{
    scanDirectory(Settings.directory);

    struct ListNode *item = Files;

    for (; item; item = item->next)
    {
        char pathAndName[512];
        sprintf(pathAndName, "%s%s", Settings.directory, item->name);
        printf("playing: %s\n", item->name);

        play(pathAndName);
    }

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