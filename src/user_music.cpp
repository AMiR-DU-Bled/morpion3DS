#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <audiohelp.hpp>
#include <citro2d.h>
int strcasecmp(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2))
            return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        s1++;
        s2++;
    }
    return *s1 - *s2;
}


void listWavFiles(const char* path, std::vector<std::string>& wavFiles) {
    DIR* dir = opendir(path);
    if (!dir) {
        printf("Failed to open directory: %s\n", path);
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) {
            const char* name = entry->d_name;
            size_t len = strlen(name);
            if (len > 4 && strcasecmp(name + len - 4, ".wav") == 0) {
                wavFiles.push_back(std::string(path) + "/" + name);
            }
        }
    }

    closedir(dir);
}
bool ensureMusicFolderExists(const char* path) {
    // Try to create each part of the path
    std::string currentPath;
    const char* p = path;

    while (*p) {
        currentPath += *p;
        if (*p == '/' || *(p+1) == '\0') {
            mkdir(currentPath.c_str(), 0777); // create folder, ignore if exists
        }
        p++;
    }

    return true; // always returns true; mkdir fails silently if folder exists
}



void updateUserMusic(MusicPlayer*& user_music, const std::vector<std::string>& wavFiles, int& currentSong) {
    if (!user_music) return;

    user_music->update();

    // Wait for the file and NDSP buffers to finish
    if (user_music->isFinished()) {
        delete user_music;
        user_music = nullptr;

        currentSong = (currentSong + 1) % wavFiles.size();

        printf("Switching to next song: %s\n", wavFiles[currentSong].c_str());

        user_music = new MusicPlayer(wavFiles[currentSong].c_str(), 1, false);
        user_music->play();
    }
}





