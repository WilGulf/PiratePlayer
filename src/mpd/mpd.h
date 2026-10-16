#include <stdbool.h>
#include <mpd/client.h>

#ifndef MDP_H
#define MPD_H

#ifdef __cplusplus
extern "C" {
#endif

extern struct mpd_connection *connection;

int mpd_init(void);
void mpd_exit(void);
void mpd_add_file(const char *file);
void mpd_play(void);
void mpd_set_playing(bool play);
void mpd_clear(void);
int mpd_get_progress(void);
bool mpd_get_playing(void);
void mpd_add_pathfile(const char *path);

#ifdef __cplusplus
}
#endif

#endif