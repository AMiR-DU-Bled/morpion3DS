#ifndef USER_MUSIC_h
#define USER_MUSIC_h
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <audiohelp.hpp>

int strcasecmp(const char* s1, const char* s2);
void listWavFiles(const char* path, std::vector<std::string>& wavFiles);
bool ensureMusicFolderExists(const char* path);
void updateUserMusic(MusicPlayer*& user_music, const std::vector<std::string>& wavFiles, int& currentSong);


#endif