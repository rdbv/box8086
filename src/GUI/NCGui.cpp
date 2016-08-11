#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "../Disasm/Disasm.hpp"
#include "../CPU/CPU.hpp"

uint8_t* loadFile(const std::string& fileName, unsigned int* fileSize) {
    FILE* f = fopen(fileName.c_str(), "rb");
    if(f == NULL) {
        printf("Unable to open");
        return nullptr;
    }
    fseek(f, 0, SEEK_END);
    *fileSize = ftell(f); 
    rewind(f);

    uint8_t* buff = (uint8_t*) malloc(1024 * 1024);
    fread(buff, *fileSize, 1, f);
    return buff;

}

struct Vec2 {
    unsigned int y = 0;
    unsigned int x = 0; 
    unsigned int& w = x;
    unsigned int& h = y;

    Vec2() {
    }

    Vec2(unsigned int _y, unsigned int _x) :
        y(_y), x(_x) {
    }

    void operator=(Vec2 v) {
        y = v.y, x = v.x;
    }

};

struct Border {
    /*
    char ls = '|', rs = '|', ts = '-', bs = '-';
    char tl = '+', tr = '+', bl = '+', br = '+';
    */
    char ls, rs, ts, bs;
    char tl, tr, bl, br;

};

const static Border b_clear = Border{' ',' ',' ',' ',
                                     ' ',' ',' ',' '};

const static Border b_box = Border{'|','|','-','-',
                                   '+','+','+','+'};

class Window {
public:
    Window(Vec2 _size, Vec2 _pos) 
        : size(_size), pos(_pos) {
        win = newwin(size.h, size.w, pos.y, pos.x);
        box(win, 0, 0);
        wrefresh(win);
    }

    void setPosition(Vec2 _pos) {
        pos = _pos;
    }

    void setSize(Vec2 _size) {
        size = _size;
    }

    Vec2 getPosition() {
        return pos;
    }

    Vec2 getSize() {
        return size;
    }

    void createBorder(Border border) {
        int x, y, w, h;

        x = pos.x, y = pos.y;
        w = size.w, h = size.h;

        mvaddch(y, x, border.tl);
        mvaddch(y, x + w - 1, border.tr);
        mvaddch(y + h - 1, x, border.bl);
        mvaddch(y + h - 1, x + w - 1, border.br);
        
        mvhline(y, x + 1, border.ts, w - 2);
        mvhline(y + h - 1, x + 1, border.bs, w - 2);
        mvvline(y + 1, x, border.ls, h - 2);
        mvvline(y + 1, x + w - 1, border.rs, h - 2);
    }

    void refresh() {
        wrefresh(win);
    }

    void destroy() {
        wborder(win, ' ', ' ', ' ', ' ',
                     ' ', ' ', ' ', ' ');
        wrefresh(win);
        delwin(win);
    }

private:
   Vec2 size, pos; 
   WINDOW* win;
};

WINDOW* createWindow(int h, int w, int sy, int sx) {
    WINDOW* win;
    win = newwin(h, w, sy, sx);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

void destroy_win(WINDOW* win) {
    wborder(win, ' ', ' ', ' ', ' ',
                 ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

int main() {
    unsigned int fileSize = 0;
    uint8_t *file = loadFile("bin/test.bin", &fileSize);
    char ch = 0;
    std::size_t win_y = 0, win_x = 0;

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, win_y, win_x);
    
    /*
    int x, y;
    x = y = 0;
    WINDOW* win = createWindow(5, 5, 1, 1);
    wrefresh(win);
    refresh();
    */

    Window win0(Vec2(5, 10), Vec2(1, 1) );
    win0.createBorder(b_box);

    while((ch != 'q')) {
        ch = getch();

        Vec2 p;
        switch(ch) 
        {
            case 'w':
                p = win0.getPosition(), p.y -= 1;
                win0.createBorder(b_clear);
                win0.setPosition(p);
                win0.createBorder(b_box);
                break;

            case 's':
                p = win0.getPosition(), p.y += 1;
                win0.createBorder(b_clear);
                win0.setPosition(p);
                win0.createBorder(b_box);
                break;

            case 'a':
                p = win0.getPosition(), p.x -= 1;
                win0.createBorder(b_clear);
                win0.setPosition(p);
                win0.createBorder(b_box);
                break;

            case 'd':
                p = win0.getPosition(), p.x += 1;
                win0.createBorder(b_clear);
                win0.setPosition(p);
                win0.createBorder(b_box);
                break;
        }


        /*
        switch(ch)
        {
            case 'a':
                x--;
                destroy_win(win);
                win = createWindow(5, 5, y, x);
                break;

            case 'd':
                x++;
                destroy_win(win);
                win = createWindow(5, 5, y, x);
                break;

            case 'w':
                y--;
                destroy_win(win);
                win = createWindow(5, 5, y, x);
                break;

            case 's':
                y++;
                destroy_win(win);
                win = createWindow(5, 5, y, x);
                break;

        }
        */

        refresh();
    }
    
    //printw("KONIEC");
    refresh();
    getch();

    endwin();
}

