struct Metadata
{
    int favorite;
    char artist[256];
    char title[256];
};

// set artist, title and favorite of a song
int editSong(char *path_and_name, char *name, char *file_type_ending);