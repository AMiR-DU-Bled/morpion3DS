#include <audiohelp.hpp>

// Check WAV header
bool MusicPlayer::is16BitPCM(FILE* f) {
    if (!f) return false;

    fseek(f, 20, SEEK_SET);
    uint16_t audioFormat;
    fread(&audioFormat, sizeof(uint16_t), 1, f);

    fseek(f, 34, SEEK_SET);
    uint16_t bitsPerSample;
    fread(&bitsPerSample, sizeof(uint16_t), 1, f);

    fseek(f, 0, SEEK_SET);
    return (audioFormat == 1) && (bitsPerSample == 16);
}

// Constructor
MusicPlayer::MusicPlayer(const char* path, int ch, bool loopPlayback)
    : channel(ch), loop(loopPlayback), currentBuf(0), paused(false) {
    
    bool fromRomfs = (strncmp(path, "romfs:", 6) == 0);
    CHUNK_SIZE = fromRomfs ? (64 * 1024) : (32 * 1024);

    file = fopen(path, "rb");
    if (!file) {
        printf("Failed to open %s\n", path);
        return;
    }

    if (!is16BitPCM(file)) {
        fclose(file);
        fprintf(stderr, "Error: %s is not 16-bit PCM WAV!\n", path);
        abort();
    }

    u8 header[44];
    fread(header, 1, 44, file); // skip WAV header

    for (int i = 0; i < NUM_BUFFERS; i++)
        buffer[i] = (u8*)linearAlloc((CHUNK_SIZE + 0x1F) & ~0x1F);

    ndspChnReset(channel);
    ndspChnSetInterp(channel, NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(channel, 44100.0f);
    ndspChnSetFormat(channel, NDSP_FORMAT_STEREO_PCM16);

    memset(waveBuf, 0, sizeof(waveBuf));
    for (int i = 0; i < NUM_BUFFERS; i++) waveBuf[i].looping = false;
}

// Destructor
MusicPlayer::~MusicPlayer() {
    ndspChnWaveBufClear(channel);
    ndspChnReset(channel);
    for (int i = 0; i < NUM_BUFFERS; i++) if (buffer[i]) linearFree(buffer[i]);
    if (file) fclose(file);
}

// Play
void MusicPlayer::play() {
    paused = false;
    fileEnded = false;
    ndspChnWaveBufClear(channel);
    currentBuf = 0;

    for (int i = 0; i < NUM_BUFFERS; i++) feedNextChunk();
}

// Pause
void MusicPlayer::pause() {
    paused = true;
    ndspChnWaveBufClear(channel);
}

// Stop
void MusicPlayer::stop() {
    paused = false;
    ndspChnWaveBufClear(channel);
    ndspChnReset(channel);
    if (file) fseek(file, 44, SEEK_SET);
}

// Update called each frame
void MusicPlayer::update() {
    if (paused || !file) return;

    for (int i = 0; i < NUM_BUFFERS; i++) {
        if (waveBuf[i].status == NDSP_WBUF_DONE) {
            currentBuf = i;
            feedNextChunk();
        }
    }
}

// Feed next chunk to NDSP
void MusicPlayer::feedNextChunk() {
    if (!file) return;

    ndspWaveBuf* buf = &waveBuf[currentBuf];
    memset(buf, 0, sizeof(ndspWaveBuf));

    size_t readBytes = fread(buffer[currentBuf], 1, CHUNK_SIZE, file);

    // truncate to complete stereo frames
    if (readBytes == 0) {
        if (loop) {
            fseek(file, 44, SEEK_SET);
            readBytes = fread(buffer[currentBuf], 1, CHUNK_SIZE, file);
        } else {
            fileEnded = true;
            return; // Stop feeding — will trigger isFinished() soon
        }
    }


    s16* samples = (s16*)buffer[currentBuf];
    size_t nsamples = readBytes / 2;
    for (size_t i = 0; i < nsamples; i += 2) {
        samples[i]   = (s16)(samples[i]   * leftVolume);
        samples[i+1] = (s16)(samples[i+1] * rightVolume);
    }

    buf->data_vaddr = buffer[currentBuf];
    buf->nsamples = readBytes / 4;
    buf->looping = false;
    buf->status = NDSP_WBUF_FREE;

    DSP_FlushDataCache(buf->data_vaddr, readBytes);
    ndspChnWaveBufAdd(channel, buf);

    currentBuf = (currentBuf + 1) % NUM_BUFFERS;
}

// Set volume
void MusicPlayer::setVolume(float left, float right) {
    leftVolume = left;
    rightVolume = right;
}

bool MusicPlayer::isFinished() const {
    if (!file) return true;
    if (!fileEnded) return false;

    for (int i = 0; i < NUM_BUFFERS; i++) {
        if (waveBuf[i].status != NDSP_WBUF_DONE && waveBuf[i].status != NDSP_WBUF_FREE)
            return false;
    }
    return true;
}

