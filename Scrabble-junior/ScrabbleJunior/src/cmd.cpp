// Adapted from the file supplied by the teacher on Moodle.

#include "cmd.h"

using namespace std;

void clrscr(int x, int y) {
    COORD coordScreen;
    coordScreen.X = (SHORT) x;
    coordScreen.Y = (SHORT) y;
    DWORD cCharsWritten;
    DWORD dwConSize;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hCon, &csbi);
    dwConSize = (csbi.dwSize.X-x) * (csbi.dwSize.Y-y);
    // fill with spaces
    FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hCon, &csbi);
    FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    
    SetConsoleCursorPosition(hCon, coordScreen);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT) x;
    coord.Y = (SHORT) y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setcolor(Color color, Color background_color) {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (background_color == BLACK) {
        SetConsoleTextAttribute(hCon, (WORD) color);
    } else {
        SetConsoleTextAttribute(hCon, (WORD) (color | (background_color << 4)));
    }
}
