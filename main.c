#include "main.h"

float volume = 1.0;
char input[512];

int main(int argc, char const *argv[])
{
    readVolume();

    if (argc == 1)
    {
        playMusicCurrDir();
    }
    else if (argc == 2)
    {
        playMusic(argv);
    }
    else if (argc == 3 && !strcmp(argv[1], "--directory") || !strcmp(argv[1], "-d"))
    {
        playMusicDir();
    }
}

void playMusic(char const *argv[])
{
    printf("start playing: %s\n", argv[1]);
    pid_t soundPid = fork();
    if (soundPid < 0)
    {
        perror("forking error");
        exit(1);
    }
    else if (soundPid == 0)
    {
        playSound(argv[1]);
    }

    printf("quit ('q')\n");
    scanf("%s", &input);

    if (!strcmp(input, "q") || !strcmp(input, "quit"))
    {
        kill(soundPid, SIGKILL);
    }

    waitpid(soundPid, NULL, 0);
    exit(0);
}

void playMusicCurrDir()
{
    printf("WIP :scan current directory\n");
    exit(0);
}

void playMusicDir()
{
    printf("WIP: scan directory\n");
    exit(0);
}

void playSound(char const *path)
{
    char vol[32];
    snprintf(vol, sizeof(vol), "volume=%f", volume);

    execlp("/bin/ffplay", "ffplay", "-v", "0", "-nodisp", "-autoexit", "-af", vol, path, NULL);
    exit(0);
}

void readVolume()
{
    char buf[8];
    FILE *volumeFile;

    volumeFile = fopen("audioRename.txt", "r");
    if (volumeFile == NULL)
    {
        volumeFile = fopen("audioRename.txt", "w");
        printf("please enter volume (0.0 -> 1.0)\n");
        scanf("%f", &volume);
        sprintf(buf, "%f", volume);
        fprintf(volumeFile, buf);
    }
    else
    {
        fgets(buf, 8, volumeFile);
    }
    fclose(volumeFile);

    volume = strtof(buf, NULL);
}
