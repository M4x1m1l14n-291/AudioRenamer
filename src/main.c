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

// #define CLEAR //
#define CLEAR system("@cls||clear");

struct ListNode *Files;
struct settings Settings = {1.0f, "", ""};

FILE *settingsFile;

char dirBuf[256];
char lastDir[256];

int running = 1;
int playing = 1;

int main()
{
    char inp[256] = {0};

    char *homeDir = getenv("HOME");
    if (homeDir == NULL)
    {
        printf("$HOME environment variable not set\n");
        exit(1);
    }
    else if (strlen(homeDir) > 256 - 23)
    {
        printf("$HOME environment variable to long\n");
        exit(1);
    }

    snprintf(dirBuf, sizeof(dirBuf), "%s/.config/audioRename", homeDir);
    readSettings(dirBuf);

    while (running)
    {
        CLEAR
        printf("AUDIO RENAMER\n\n"
               "volume=%.2f\n"
               "directory=%s\n"
               "last played song=%s\n\n",
               "change volume           (v):\n"
               "set or change directory (d):\n",
               Settings.volume,
               Settings.directory,
               Settings.lastPlayedSong);
        if (strlen(Settings.directory) > 0 && strlen(Settings.lastPlayedSong) > 0)
            printf("continue playing songs  (p):\n"
                   "restart from begining   (r):\n");
        else
            printf("start playing           (p):\n");
        printf("quit                    (q):\n"
               "> ");

        fgets(inp, 256, stdin);
        inp[strlen(inp) - 1] = '\0';

        if (!strcmp(inp, "vol") || !strcmp(inp, "volume") || !strcmp(inp, "v"))
        {
            printf("enter volume (0.0 -> 1.0): ");
            fgets(inp, 256, stdin);
            Settings.volume = strtof(inp, NULL);
        }
        else if (!strcmp(inp, "dir") || !strcmp(inp, "directory") || !strcmp(inp, "d"))
        {
            printf("enter full path to music directory: ");
            fgets(Settings.directory, 255, stdin);
            int len = strlen(Settings.directory);
            Settings.directory[len-- - 1] = '\0';

            if (Settings.directory[len - 1] != '/')
            {
                Settings.directory[len] = '/';
                Settings.directory[len + 1] = '\0';
            }
        }
        else if (!strcmp(inp, "play") || !strcmp(inp, "p"))
            playDir();
        else if (!strcmp(inp, "reset") || !strcmp(inp, "r"))
            strcpy(Settings.lastPlayedSong, "");
        else if (!strcmp(inp, "quit") || !strcmp(inp, "q"))
            running = 0;
        saveSettings(dirBuf);
    }
    CLEAR
}

void play(char const *filename, char *name, unsigned int retries)
{
    pid_t soundPid = fork();
    if (soundPid < 0)
    {
        perror("forking error, trying again");
        if (retries > 5)
            exit(1);

        play(filename, name, retries + 1);
    }
    else if (soundPid == 0)
        playMusic(filename, Settings.volume);

    char input[256];

start:
    CLEAR
    printf("playing : %s\n\n"
           "stop (s):\n"
           "next (n):\n"
           "edit (e):\n"
           "quit (q):\n"
           "> ",
           name);
    fgets(input, 256, stdin);
    input[strlen(input) - 1] = '\0';

    if (!strcmp(input, "s") || !strcmp(input, "stop"))
    {
        kill(soundPid, SIGKILL);
        playing = 0;
        return;
    }
    else if (!strcmp(input, "n") || !strcmp(input, "next"))
        kill(soundPid, SIGKILL);
    else if (!strcmp(input, "q") || !strcmp(input, "quit"))
    {
        kill(soundPid, SIGKILL);
        waitpid(soundPid, NULL, 0);
        exit(0);
    }
    else if (!strcmp(input, "e") || !strcmp(input, "edit"))
    {
    }
    else
        goto start;

    waitpid(soundPid, NULL, 0);
}

void playDir()
{
    CLEAR
    if (strcmp(Settings.directory, lastDir))
        scanDirectory(Settings.directory);
    strcpy(lastDir, Settings.directory);

    struct ListNode *item = Files;
    struct ListNode *test = Files;

    for (; test; test = test->next)
        if (!strcmp(test->name, Settings.lastPlayedSong))
            item = test;

    for (; item && playing; item = item->next)
    {
        char pathAndName[512];
        sprintf(pathAndName, "%s%s", Settings.directory, item->name);

        strcpy(Settings.lastPlayedSong, item->name);
        saveSettings();
        play(pathAndName, item->name, 0);
    }
    playing = 1;
}

void scanDirectory(char const *path)
{
    if (Files)
        freeNodes(Files);

    Files = malloc(sizeof(struct ListNode));
    strcpy(Files->name, "");
    Files->next = NULL;
    Files->prev = NULL;

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

    sortListAlpha(Files);

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
    if ((settingsFile = fopen(dirBuf, "rb")) == NULL)
        saveSettings(dirBuf);
    else
        loadSettings();
}

void saveSettings()
{
    settingsFile = fopen(dirBuf, "w+b");

    fwrite(&Settings.volume, 4, 1, settingsFile);
    fwrite(&Settings.directory, 256, 1, settingsFile);
    fwrite(&Settings.lastPlayedSong, 256, 1, settingsFile);

    fclose(settingsFile);
}

void loadSettings()
{
    fread(&Settings.volume, 4, 1, settingsFile);
    fread(&Settings.directory, 256, 1, settingsFile);
    fread(&Settings.lastPlayedSong, 256, 1, settingsFile);

    fclose(settingsFile);
}