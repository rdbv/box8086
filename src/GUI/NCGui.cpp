#include <stdio.h>
#include <ncurses.h>

#include "../Disasm/Disasm.hpp"
#include "../CPU/CPU.hpp"

int main() {
    
    int ch;
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    while(true) 
    {

    printw("Type any\n");
    ch = getch();

    if(ch == KEY_F(1))
        printw("f1 kp");

    else
    {
        printw("KP ");
        attron(A_BOLD);
        printw("%c", ch);
        attroff(A_BOLD);
    }
    }

    refresh();
    getch();
    endwin();

    return 1;
}
