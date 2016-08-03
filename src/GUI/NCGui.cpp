#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "../Disasm/Disasm.hpp"
#include "../CPU/CPU.hpp"

int main() {
    char m[] = "String";
    int r, c;
    initscr();

    getmaxyx(stdscr, r, c);
    
    mvprintw(r/2, (c - strlen(m)) / 2, "%s %d %d", m, r, c);

    refresh();
    getch();
    endwin();

}

