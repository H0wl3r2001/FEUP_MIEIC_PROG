#ifndef CMD_H
#define CMD_H

// Adapted from the file supplied by the teacher on Moodle.
// Provides an easy way of controlling the console colors and cursor, and to clear the screen.

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

enum Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6, 
    LIGHTGRAY = 7,
    DARKGRAY = 8,
    LIGHTBLUE = 9,
    LIGHTGREEN = 10,
    LIGHTCYAN = 11,
    LIGHTRED = 12,
    LIGHTMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

// Clears the screen. If 'x' and 'y' are given, only clears the 
// part of the screen to the right of 'x' and below 'y', inclusive.
void clrscr(int x = 0, int y = 0);

// Position the cursor at column 'x', line 'y'.
void gotoxy(int x, int y);

// Set text color and background.
void setcolor(Color color, Color background_color = BLACK);

#endif