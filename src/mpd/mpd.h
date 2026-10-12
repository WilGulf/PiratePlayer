#include <stdbool.h>

#ifndef MDP_H
#define MPD_H

#ifdef __cplusplus
extern "C" {
#endif

int mpd_init(void);
void mpd_exit(void);
void mpd_add_file(const char *file);
void mpd_play(void);
void mpd_toggle_playing(bool play);

#ifdef __cplusplus
}
#endif

#endif