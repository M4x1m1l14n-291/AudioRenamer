#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

float volume = 0.5;

void *playSound(void *path)
{
    printf("start playing: %s\n", (char *)path);
    char command[256];
    snprintf(command, sizeof(command), "ffplay -v 0 -nodisp -autoexit -af \"volume=%f\" \"./%s\"", volume, (char *)path);
    system(command);
}

int main(int argc, char const *argv[])
{
    printf("please enter volume (0.0 -> 1.0)\n");
    scanf("%f", &volume);

    if (argc == 1)
    {
        printf("WIP :scan current directory\n");
        return 0;
    }
    else if (argc == 2)
    {
        char input;
        pthread_t thread_id;

        pthread_create(&thread_id, NULL, playSound, argv[1]);
        pthread_join(thread_id, NULL);
    }
    else if (argc == 3 && !strcmp(argv[1], "--directory"))
    {
        printf("WIP: scan directory\n");
        return 0;
    }

    return 0;
}
