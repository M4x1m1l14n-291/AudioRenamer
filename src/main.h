struct Settings
{
    float volume;
    char directory[256];
    char last_played_song[256];
} Settings;

// print program menu
void printProgramMenu();

// get volume from user input
void inputVolume(char *user_input);

// get directory from user input
void inputDirectory();

// play all music from directory
void playSongsInDirectory();

// dispatch music playing and manage user input
void play(char const *file_dir_path, char *name, unsigned int retries);

void scanDirectory(char const *path);

// check if file is an audio file
int isAudioFile(char *filename);

// read settings
void readSettings();

// save data to settings file
void saveSettings();

// load data from settings file
void loadSettings();