#include "main.h"

#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "editSong.h"
#include "linkedList.h"
#include "playMusic.h"

struct ListNode *Files = NULL;
struct Settings Settings = {1.0f, "", ""};

FILE *settings_file = NULL;

char dir_buf[256];
char last_dir[256];

int running = 1;
int playing = 1;

int main(void) {
    char user_input[16];

    char *home_dir = getenv("HOME");
    if (home_dir == NULL || strlen(home_dir) > 256 - 23)
        printf(
            "$HOME environment variable not set or $HOME environment variable "
            "to long\n");

    else {
        snprintf(dir_buf, sizeof(dir_buf), "%s/.config/audioRename", home_dir);
        readSettings();
    }

    while (running) {
        printProgramMenu();

        fgets(user_input, 16, stdin);
        user_input[strlen(user_input) - 1] = '\0';

        if (!strcmp(user_input, "vol") || !strcmp(user_input, "volume") || !strcmp(user_input, "v"))
            inputVolume(user_input);

        else if (!strcmp(user_input, "dir") || !strcmp(user_input, "directory") || !strcmp(user_input, "d"))
            inputDirectory();

        else if (!strcmp(user_input, "play") || !strcmp(user_input, "p"))
            playSongsInDirectory();

        else if (!strcmp(user_input, "reset") || !strcmp(user_input, "r"))
            strcpy(Settings.last_played_song, "");

        else if (!strcmp(user_input, "quit") || !strcmp(user_input, "q"))
            running = 0;

        saveSettings();
    }
    system("@cls||clear");
    exit(0);
}

void printProgramMenu() {
    system("@cls||clear");
    printf(
        "--- AUDIO RENAMER ---\n"
        "\n"
        "> volume           = %.2f\n"
        "> directory        = %s\n"
        "> last played song = %s\n"
        "\n"
        "> change volume           (v):\n"
        "> set or change directory (d):\n"
        "%s"
        "> quit                    (q):\n"
        "> ",
        Settings.volume, Settings.directory, Settings.last_played_song,
        strlen(Settings.directory) > 0 && strlen(Settings.last_played_song) > 0 ? "> continue playing songs  (p):\n"
                                                                                  "> restart from beginning   (r):\n"
                                                                                : "> start playing           (p):\n");
}

void inputVolume(char *user_input) {
    printf("> enter volume (0.0 -> 1.0): ");
    fgets(user_input, 16, stdin);
    Settings.volume = strtof(user_input, NULL);
}

void inputDirectory() {
    printf("> enter full path to music directory: ");
    fgets(Settings.directory, 256, stdin);
    int len = strlen(Settings.directory);
    Settings.directory[len-- - 1] = '\0';

    if (Settings.directory[len - 1] != '/') {
        Settings.directory[len] = '/';
        Settings.directory[len + 1] = '\0';
    }
}

void playSongsInDirectory() {
    system("@cls||clear");
    if (strcmp(Settings.directory, last_dir)) scanDirectory(Settings.directory);
    strcpy(last_dir, Settings.directory);

    struct ListNode *item = Files;
    struct ListNode *test = Files;

    for (; test; test = test->next)
        if (!strcmp(test->name, Settings.last_played_song)) item = test;

    for (; item && playing; item = item->next) {
        play(Settings.directory, item->name, 0);
        strcpy(Settings.last_played_song, item->name);
        saveSettings();
    }
    if (item == NULL) strcpy(Settings.last_played_song, "");

    playing = 1;
}

void play(char const *file_dir_path, char *name, unsigned int retries) {
    char input[16];

    // full path of file
    char path_and_name[512];
    sprintf(path_and_name, "%s%s", Settings.directory, name);

    // get file type ending
    int name_len = strlen(name);
    char file_type_ending[5] = {name[name_len - 4], name[name_len - 3], name[name_len - 2], name[name_len - 1], '\0'};

    // fork process for playing audio files
    pid_t play_music_pid = fork();
    if (play_music_pid < 0) {
        perror("forking error, trying again\n");
        if (retries > 5) exit(5);

        play(file_dir_path, name, retries + 1);
    } else if (play_music_pid == 0)
        playMusic(path_and_name, Settings.volume);

start:
    system("@cls||clear");
    printf(
        "> playing: %s\n"
        "\n"
        "> stop (s):\n"
        "> next (n):\n"
        "> edit (e):\n"
        "> quit (q):\n"
        "> ",
        name);

    // get user input
    fgets(input, 16, stdin);
    input[strlen(input) - 1] = '\0';

    if (!strcmp(input, "s") || !strcmp(input, "stop")) {
        kill(play_music_pid, SIGKILL);
        playing = 0;
    } else if (!strcmp(input, "n") || !strcmp(input, "next"))
        kill(play_music_pid, SIGKILL);

    else if (!strcmp(input, "q") || !strcmp(input, "quit")) {
        kill(play_music_pid, SIGKILL);
        exit(0);
    } else if (!strcmp(input, "e") || !strcmp(input, "edit")) {
        editSong(Settings.directory, path_and_name, name, file_type_ending);
        goto start;
    }

    else
        goto start;

    waitpid(play_music_pid, NULL, 0);
}

void scanDirectory(char const *path) {
    if (Files) freeNodes(Files);

    // allocate data in heap for first node in list
    Files = malloc(sizeof(struct ListNode));
    strcpy(Files->name, "");
    Files->next = NULL;
    Files->prev = NULL;

    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("error opening path\n");
        exit(1);
    }

    // read files from directory
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' || entry->d_type == 4 || !isAudioFile(entry->d_name)) continue;
        appendNode(Files, entry->d_name);
    }

    // sort files alphabetically
    alphaSort(Files);
    closedir(dir);
}

int isAudioFile(char *filename) {
    int len = strlen(filename);
    if (len < 5) return 0;

    if (!strncmp(filename + len - 4, ".mp3", 4)) return 1;
    if (!strncmp(filename + len - 4, ".m4a", 4)) return 1;

    return 0;
}

void readSettings() {
    if ((settings_file = fopen(dir_buf, "rb")) == NULL)
        saveSettings();
    else
        loadSettings();
}

void saveSettings() {
    settings_file = fopen(dir_buf, "w+b");

    fwrite(&Settings.volume, 4, 1, settings_file);
    fwrite(&Settings.directory, 256, 1, settings_file);
    fwrite(&Settings.last_played_song, 256, 1, settings_file);

    fclose(settings_file);
}

void loadSettings() {
    fread(&Settings.volume, 4, 1, settings_file);
    fread(&Settings.directory, 256, 1, settings_file);
    fread(&Settings.last_played_song, 256, 1, settings_file);

    fclose(settings_file);
}
