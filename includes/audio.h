#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

void audio_init();
void audio_update(uint8_t sound_timer);
void audio_cleanup();

#endif
