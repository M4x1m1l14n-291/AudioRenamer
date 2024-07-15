struct settings
{
    float volume;
    char directory[256];
    char lastPlayedSong[256];
};

// play song
void play(char const *filename, char *name, unsigned int retries);

// play songs of a directory
void playDir();

// Scan the 'path' directory
void scanDirectory(char const *path);

// Check if filename ends with m4a or mp3
int isAudioFile(char *filename);

// Read settings from config file
void readSettings();

void saveSettings();
void loadSettings();

// Print program settings
void printStart();

// Print song options
void printOptions(char *name);
void enterVolume(char *input);
void enterDirectoryPath();