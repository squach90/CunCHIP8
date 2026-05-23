#include <SDL.h>
#include <math.h>
#include <string.h>
#include "../includes/audio.h"

#define SAMPLE_RATE 44100
#define FREQUENCY 800    // Hz - CHIP-8 beep frequency
#define AMPLITUDE 0.3    // Volume (0.0 to 1.0)

struct {
    SDL_AudioDeviceID device;
    int is_playing;
    double phase;
} audio_state = {0, 0, 0.0};

void audio_callback(void *userdata, uint8_t *stream, int len) {
    float *fstream = (float *)stream;
    int samples = len / sizeof(float);
    int *playing = (int *)userdata;
    
    if (!*playing) {
        memset(stream, 0, len);
        return;
    }
    
    for (int i = 0; i < samples; i++) {
        audio_state.phase += (float)FREQUENCY / SAMPLE_RATE;
        if (audio_state.phase >= 1.0) {
            audio_state.phase -= 1.0;
        }
        
        // Generate sine wave
        float sample = sinf(2.0f * 3.14159265f * audio_state.phase) * AMPLITUDE;
        fstream[i] = sample;
    }
}

void audio_init() {
    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_F32;
    want.channels = 1;
    want.samples = 4096;
    want.callback = audio_callback;
    want.userdata = &audio_state.is_playing;
    
    audio_state.device = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (audio_state.device == 0) {
        SDL_Log("❌ Failed to open audio device: %s", SDL_GetError());
        return;
    }
    
    audio_state.is_playing = 0;
    SDL_PauseAudioDevice(audio_state.device, 0); // Start playback
}

void audio_update(uint8_t sound_timer) {
    if (sound_timer > 0) {
        audio_state.is_playing = 1;
    } else {
        audio_state.is_playing = 0;
    }
}

void audio_cleanup() {
    if (audio_state.device != 0) {
        SDL_CloseAudioDevice(audio_state.device);
        audio_state.device = 0;
    }
}
