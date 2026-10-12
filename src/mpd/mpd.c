#include <stdbool.h>
#include <stdio.h>
#include <mpd/client.h>

#include "mpd.h"

struct mpd_connection *connection = NULL;

int mpd_init(void) {
    connection = mpd_connection_new(NULL, 0, 0);
    if (!connection) {
        return -1;
    }

    return 0;
}

void mpd_exit(void) {
    mpd_connection_free(connection);
}

void mpd_add_file(const char *file) {
    if (!mpd_run_add(connection, file)) {
        // ERROR
    }
}

void mpd_play(void) {
    if (!mpd_run_play(connection)) {
        // ERROR
    }
}

void mpd_toggle_playing(bool play) {
    if (!mpd_run_pause(connection, play)) {
        // ERROR
    }
}