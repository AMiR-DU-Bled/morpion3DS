#include <stdio.h>
#include <string.h>
#include <iostream>
#include <3ds.h>
#include <fstream>
#include <varhelp.h>

void loadLanguage(const std::string& path) {
    translations.clear();  // remove old translations
    std::ifstream file(path);
    if (!file.is_open()) {
        printf("Failed to open %s\n", path.c_str());
        svcSleepThread(10*100000000LL);
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
        line.pop_back(); // remove carriage return
        if (line.empty() || line[0] == '#') continue; // skip blank/comments
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string key = line.substr(0, eq);
            std::string value = line.substr(eq + 1);
            translations[key] = value;
            std::cout<<value;
        }
    }
}

const char* tr(const std::string& key) {
    auto it = translations.find(key);
    if (it != translations.end())
        return it->second.c_str();
    return key.c_str(); // fallback if key not found
}