#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utils.h>
#include <signal.h>

void playSound(char const *path);
void readVolume();
void playMusicCurrDir();
void playMusicDir();
void playMusic();