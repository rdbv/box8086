#include <ncurses.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "../CPU/CPU.hpp"

/* 
   Ncurses GUI for debugging
   Code propably read-only :D
 */

uint8_t* loadFile(const char* fileName, unsigned int* fileSize) {
    FILE* f = fopen(fileName, "rb");
    if(f == NULL) {
        printf("Unable to open");
        return 0;
    }
    fseek(f, 0, SEEK_END);
    *fileSize = ftell(f); 
    rewind(f);

    uint8_t* buff = (uint8_t*) malloc(1024 * 1024);
    fread(buff, *fileSize, 1, f);
    return buff;

}


void update_size() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    COLS = w.ws_col;
    LINES = w.ws_row;
}

void winsz_handle(int i) {
    update_size();
}

void init() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    raw();
    keypad(stdscr, TRUE);
    
    if(has_colors() == 0) {
        endwin();
        printf("No colors support");
        exit(0);

    }

    start_color();
    
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = winsz_handle;
    sigaction(SIGWINCH, &sa, NULL);

    update_size();
}


WINDOW* create_win(int h, int w, int sy, int sx) {
    WINDOW* ww = newwin(h, w, sy, sx);
    box(ww, 0, 0);
    wrefresh(ww);
    refresh(); 
    return ww;
}

void destroy_win(WINDOW* w) {
    wborder(w, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(w);
    delwin(w);
}

void draw_memory(const Registers& regs, const Memory& mem, WINDOW* win, int saddr) {
    for(int i = 0; i < 16; ++i) {
        mvwprintw(win, 1, 10+(i*3), "%x", i);
    }
    int a = 0, b = 0;
    uint32_t absip = (regs.cs<<4)+regs.ip;
    int cp = 0;
    for(int i = 2; i < LINES-2; ++i) {
        for(int j = 10; j < 58;j+=3) {
            if( ((uint32_t)saddr+(a+b)) == absip)
                cp = 2;
            else
                cp = 1;
            wattron(win, COLOR_PAIR(cp));
            mvwprintw(win, i, j, "%02x", mem[saddr+(a+b)]);
            wattroff(win, COLOR_PAIR(cp));
            b++;
        }
        a++;
        mvwprintw(win, i, 1, "%04x", i-2);
    }
    wrefresh(win);
}

void draw_registers(const Registers& regs, WINDOW* win) {
   
    mvwprintw(win, 1, 1,  "AX:0x%04x", regs.ax);
    mvwprintw(win, 1, 11, "BX:0x%04x", regs.bx);
    mvwprintw(win, 1, 21, "CX:0x%04x", regs.cx);
    mvwprintw(win, 1, 31, "DX:0x%04x", regs.dx);
  
    mvwprintw(win, 2, 1, "IP:0x%04x", regs.ip);

    
    wrefresh(win);
}


int main() {
   
    uint32_t filesize;
    uint8_t *file = loadFile("bin/test.bin", &filesize);
    CPU cpu;
    Memory &mem = *cpu.getMemory();
    memcpy( &mem[0], file, filesize);
    Registers regs;

    init();

    WINDOW* regs_win = create_win(10, COLS/2, 0, 0);
    WINDOW* mem_win = create_win(LINES, COLS/2, 0, COLS/2);

    int c;

    regs = cpu.getRegisters();

    while( (c = getch()) != 'q') {
        clear();
        refresh();
       
        if(c == KEY_F(8)) {
            cpu.step();
            regs = cpu.getRegisters();
        }

        destroy_win(regs_win);
        regs_win = create_win(10, COLS/2, 0, 0);
        destroy_win(mem_win);
        mem_win = create_win(LINES, COLS/2, 0, COLS/2);

        draw_registers(regs, regs_win);
        draw_memory(regs, mem, mem_win, 0x0000);


        refresh();
        
    }

    endwin();

}


