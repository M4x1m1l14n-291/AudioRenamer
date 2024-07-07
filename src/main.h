struct settings
{
    float volume;
    char directory[256];
    char lastPlayedSong[256];
};

void play(char const *filename, unsigned int retries);
void playDir();

// Scan the 'path' directory
void scanDirectory(char const *path);

// Check if filename ends with m4a or mp3
int isAudioFile(char *filename);

// Read settings from config file
void readSettings(char *dirBuf);

void saveSettings(char *dirBuf);
void loadSettings();
