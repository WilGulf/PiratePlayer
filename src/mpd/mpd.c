#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <mpd/client.h>
#include <mpd/status.h>

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

void mpd_set_playing(bool play) {
    if (!mpd_run_pause(connection, play)) {
        // ERROR
    }
}

void mpd_clear(void) {
    if (!mpd_run_clear(connection)) {
        // ERROR
    }
}

int mpd_get_progress(void) {
    struct mpd_status *status = mpd_run_status(connection);
    if (!status) {
        return 0;
    }
    int elapsed = mpd_status_get_elapsed_time(status);
    int total = mpd_status_get_total_time(status);

    mpd_status_free(status);

    if (total > 0) {
        float progress = (float)elapsed / total;
        return round(progress * 100);
    } else {
        return 0;
    }
}

bool mpd_get_playing(void) {
    struct mpd_status *status = mpd_run_status(connection);
    if (!status) {
        return false;
    }

    enum mpd_state state = mpd_status_get_state(status);
    if (state == MPD_STATE_PLAY || state == MPD_STATE_PAUSE) {
        mpd_status_free(status);
        return true;
    } else {
        mpd_status_free(status);
        return false;
    }
}