#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

int platform_init(void);
void platform_refresh(void);
int platform_exit(void);

int sdl_init(void);
void sdl_refresh(void);
int sdl_exit(void);

#ifdef __cplusplus
}
#endif

#endif