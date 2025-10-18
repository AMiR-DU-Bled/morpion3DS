#ifndef AUDIOHELP_HPP
#define AUDIOHELP_HPP

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class MusicPlayer {
private:
    int channel;
    bool loop = false;
    int currentBuf = 0;
    bool paused = false;
    FILE* file = nullptr;
    bool fileEnded = false;

    static const int NUM_BUFFERS = 2;
    ndspWaveBuf waveBuf[NUM_BUFFERS];
    u8* buffer[NUM_BUFFERS] = {nullptr};
    size_t CHUNK_SIZE;

    float leftVolume = 1.0f;
    float rightVolume = 1.0f;

public:
    MusicPlayer(const char* path, int ch, bool loopPlayback = false);
    ~MusicPlayer();

    void play();
    void pause();
    void stop();
    void update();
    void setVolume(float left, float right);

    bool isFinished() const;         // returns true if song fully played
    int waveBufStatus(int i) const { return waveBuf[i].status; }
    bool isFileEnded() const { return fileEnded; }

private:
    void feedNextChunk();
    static bool is16BitPCM(FILE* f);
};

#endif
