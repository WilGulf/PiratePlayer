#ifndef PLATFORM_H
#define PLATFORM_H

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

#ifdef __cplusplus
extern "C" {
#endif

int platform_init(void);
void platform_refresh(void);
int platform_exit(void);
int platform_poll(void);

#ifdef DESKTOP_SDL
int sdl_init(void);
void sdl_refresh(void);
int sdl_exit(void);
int sdl_poll(void);
#endif

#ifdef __cplusplus
}
#endif

#endif