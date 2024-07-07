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

char dirBuf[256];

int running = 1;

int main()
{
    char *homeDir = getenv("HOME");
    if (homeDir == NULL)
    {
        printf("$HOME environment variable not set\n");
        exit(1);
    }

    snprintf(dirBuf, sizeof(dirBuf), "%s/.config/audioRename", homeDir);

    readSettings(dirBuf);

    while (running)
    {
        char inp[256];

        system("@cls||clear");
        printf("AUDIO RENAMER\n\n");

        printf("volume=%.2f\ndirectory=%s\nlast played song=%s\n\n", Settings.volume, Settings.directory, Settings.lastPlayedSong);

        printf("change volume           (v):\n"
               "set or change directory (d):\n");
        if (strlen(Settings.directory) > 0 && strlen(Settings.lastPlayedSong) > 0)
            printf("continue playing songs (p):\n");
        else
            printf("start playing           (p):\n");
        printf("quit                    (q):\n> ");

        scanf("%s", inp);

        if (!strcmp(inp, "vol") || !strcmp(inp, "volume") || !strcmp(inp, "v"))
        {
            printf("enter volume (0.0 -> 1.0): ", Settings.volume);
            scanf("%f", &Settings.volume);
        }
        else if (!strcmp(inp, "dir") || !strcmp(inp, "directory") || !strcmp(inp, "d"))
        {
            printf("enter full path to music directory: ", Settings.directory);
            scanf("%s", &Settings.directory);
            int len = strlen(Settings.directory);
            if (Settings.directory[len - 1] != '/' && len < 255)
            {
                Settings.directory[len] = '/';
                Settings.directory[len + 1] = '\0';
            }
            else if (len >= 255)
            {
                perror("path too long!");
                exit(1);
            }
        }
        else if (!strcmp(inp, "play") || !strcmp(inp, "p"))
            playDir();
        else if (!strcmp(inp, "quit") || !strcmp(inp, "q"))
            running = 0;
        saveSettings(dirBuf);
    }
}

void play(char const *filename, unsigned int retries)
{
    pid_t soundPid = fork();
    if (soundPid < 0)
    {
        perror("forking error, trying again");
        if (retries > 5)
            exit(1);

        play(filename, retries + 1);
    }
    else if (soundPid == 0)
        playMusic(filename, Settings.volume);

    char inp[256];
    printf("next (n):\n"
           "quit (q):\n> ");
    scanf("%s", inp);

    if (!strcmp(inp, "n") || !strcmp(inp, "next"))
        kill(soundPid, SIGKILL);
    if (!strcmp(inp, "q") || !strcmp(inp, "quit"))
    {
        kill(soundPid, SIGKILL);
        waitpid(soundPid, NULL, 0);
        exit(0);
    }

    waitpid(soundPid, NULL, 0);
}

void playDir()
{
    system("@cls||clear");
    scanDirectory(Settings.directory);

    struct ListNode *item = Files;

    for (; item; item = item->next)
    {
        char pathAndName[512];
        sprintf(pathAndName, "%s%s", Settings.directory, item->name);
        printf("playing: %s\n", item->name);

        play(pathAndName, 0);
    }
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

void readSettings(char *dirBuf)
{
    if ((settingsFile = fopen(dirBuf, "rb")) == NULL)
        saveSettings(dirBuf);
    else
        loadSettings();
}

void saveSettings(char *dirBuf)
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