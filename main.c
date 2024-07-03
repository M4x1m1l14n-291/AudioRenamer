#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

float volume = 0.5;

void *playSound(void *path)
{
    printf("start playing: %s\n", (char *)path);

    char command[256];
    snprintf(command, sizeof(command), "ffplay -v 0 -nodisp -autoexit -af \"volume=%f\" ./%s", volume, (char *)path);

    system(command);
}

int main(int argc, char const *argv[])
{
    printf("please enter volume (0.0 -> 1.0)\n");
    scanf("%f", &volume);
    if (volume > 1.0)
    {
        volume = 1.0;
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, playSound, "audio.mp3");
    pthread_join(thread_id, NULL);
    return 0;
}
