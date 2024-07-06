struct settings
{
    float volume;
    char directory[256];
    char lastPlayedSong[256];
};

void play(char const *filename);
void playDir(char const *dir);

// Scan the 'path' directory
void scanDirectory(char const *path);

// Check if filename ends with m4a or mp3
int isAudioFile(char *filename);

// Read settings from config file
void readSettings();

// Create settings file in user config directory
void createSettingsFile(FILE *settingsFile, char *dirBuf);

void printStart();