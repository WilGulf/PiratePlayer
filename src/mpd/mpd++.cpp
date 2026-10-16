#include <string>
#include <filesystem>
#include <mpd/client.h>

#include "mpd.h"

void mpd_add_pathfile(const char *path) {
    std::filesystem::path newpath = path;
    std::filesystem::path absolute = std::filesystem::absolute(newpath);
    std::string full_string = "file://" + absolute.string();
    if (!mpd_run_add(connection, full_string.c_str())) {
        // ERROR
    }
}