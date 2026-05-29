// Header to declare BitSweeper core types and function prototypes
#ifndef BITSWEEPER_H
#define BITSWEEPER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    const int gwidth, gheight; // width and height of the game field
    int winwidth, winheight;
    int px, py, gpx, gpy; // global coordinates (px, py) and game coordinates (x, y)
    int ghx, ghy, gmx, gmy; // home coords and max coords
    int runcond;
    char *openfield; // pointer to the open field array
} field;

/* Function prototypes from main.c */
void outfield(field *face, const char *filename);
void simple_set(field *face, int x, int y, char value);
void simple_set_1d(field *face, int xypos, char value);
char simple_get(field *face, int x, int y);
char simple_get_1d(field *face, int xypos);
void renderfield(field *face);
void player(field *face);
int gameloop(field *face);
field setfield(int gwidth, int gheight);
void middlecol(field *face, int width);

/* Prototypes for ANSI escape helpers (defined in ANSIescape.h) */
void disable_raw_mode(void);
void enable_raw_mode(void);
void bgcolor(int red,int green, int blue);
void clearcolor(void);
void fgcolor(int red,int green,int blue);
void clearterm(void);
void chome(void);
void cpos(int x, int y);
void cshow(void);
void chide(void);

#endif // BITSWEEPER_H
