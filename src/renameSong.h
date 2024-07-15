struct songMetadata
{
    int favorite;
    char title[256];
    char artist[256];
    char genre[256];
};

// rename/favorite song file
void editSong(char const *filepath, char *name);