#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>

typedef struct position {
    int y;
    int x;
} position;
typedef struct position2 {
    int y;
    int x;
    char s;
} position2;
position location;
position2 last_cell;
int tabaghe = 1;

void make_random_map();


int random_number(int a, int b) {
    int random_number = (rand() % (b - a + 1)) + a;
    return random_number;
}

void hallway_rd(int y1, int x1,int y2, int x2) {
    x2--;
    mvaddch(y1,x1+1,'#'); int s; x1++;
    while ((x1 != x2) || (y1 != y2))
    {
        s = random_number(0,1);
        if (s == 0) {
            if (x1 == x2) {
                mvaddch(y1+1,x1,'#');
                y1++;
            }
            else {
                mvaddch(y1,x1+1,'#');
                x1++;
            }
        }
        if (s == 1) {
            if (y1 == y2) {
                mvaddch(y1,x1+1,'#');
                x1++;
            }
            else {
                mvaddch(y1+1,x1,'#');
                y1++;
            }
        }
    }
}

void hallway_ru(int y1, int x1,int y2, int x2) {
    x2--;
    mvaddch(y1,x1+1,'#'); int s; x1++;
    while ((x1 != x2) || (y1 != y2))
    {
        s = random_number(0,1);
        if (s == 0) {
            if (x1 == x2) {
                mvaddch(y1-1,x1,'#');
                y1--;
            }
            else {
                mvaddch(y1,x1+1,'#');
                x1++;
            }
        }
        if (s == 1) {
            if (y1 == y2) {
                mvaddch(y1,x1+1,'#');
                x1++;
            }
            else {
                mvaddch(y1-1,x1,'#');
                y1--;
            }
        }
    }
}

void hallway_ur(int y1, int x1,int y2, int x2) {
    y2++;
    mvaddch(y1-1,x1,'#'); int s; y1--;
    while ((x1 != x2) || (y1 != y2))
    {
        s = random_number(0,1);
        if (s == 0) {
            if (x1 == x2) {
                mvaddch(y1-1,x1,'#');
                y1--;
            }
            else {
                mvaddch(y1,x1+1,'#');
                x1++;
            }
        }
        if (s == 1) {
            if (y1 == y2) {
                mvaddch(y1,x1+1,'#');
                x1++;
            }
            else {
                mvaddch(y1-1,x1,'#');
                y1--;
            }
        }
    }
}

void hallway_ul(int y1, int x1,int y2, int x2) {
    y2++;
    mvaddch(y1-1,x1,'#'); int s; y1--;
    while ((x1 != x2) || (y1 != y2))
    {
        s = random_number(0,1);
        if (s == 0) {
            if (x1 == x2) {
                mvaddch(y1-1,x1,'#');
                y1--;
            }
            else {
                mvaddch(y1,x1-1,'#');
                x1--;
            }
        }
        if (s == 1) {
            if (y1 == y2) {
                mvaddch(y1,x1-1,'#');
                x1--;
            }
            else {
                mvaddch(y1-1,x1,'#');
                y1--;
            }
        }
    }
}

int valid_move(int x, int y) {
    char next_ch = mvinch(y, x) & A_CHARTEXT;
    if ((next_ch == '.') || (next_ch == '#') || (next_ch == '+')) {
        return 1;
    }
    if (next_ch == '<')
    {
        tabaghe++;
        if (tabaghe <= 4)
        {
            make_random_map();
        }
    }
    return 0;
}

void move_character(int i) {
    switch (i) {
        case KEY_HOME: // 7
            if (valid_move(location.x - 1, location.y - 1)) {
                location.y--;
                location.x--;
            }
            break;
        case KEY_UP: // 8
            if (valid_move(location.x, location.y - 1)) {
                location.y--;
            }
            break;
        case KEY_PPAGE: // 9
            if (valid_move(location.x + 1, location.y - 1)) {
                location.y--;
                location.x++;
            }
            break;
        case KEY_LEFT: // 4
            if (valid_move(location.x - 1, location.y)) {
                location.x--;
            }
            break;
        case KEY_RIGHT: // 6
            if (valid_move(location.x + 1, location.y)) {
                location.x++;
            }
            break;
        case KEY_END: // 1
            if (valid_move(location.x - 1, location.y + 1)) {
                location.y++;
                location.x--;
            }
            break;
        case KEY_DOWN: // 2
            if (valid_move(location.x, location.y + 1)) {
                location.y++;
            }
            break;
        case KEY_NPAGE: // 3
            if (valid_move(location.x + 1, location.y + 1)) {
                location.y++;
                location.x++;
            }
            break;
    }
}

void make_random_map() {
    initscr();
    clear();
    noecho();
    curs_set(0);
    start_color();
    keypad(stdscr, TRUE);
    init_pair(1,COLOR_CYAN,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    // init_pair(5,COLOR_WHITE,COLOR_BLACK);
    // init_pair(6,COLOR_BLUE,COLOR_BLACK);
    // init_pair(7,COLOR_GREEN,COLOR_BLACK);
    attron(COLOR_PAIR(1));
    mvprintw(0,1,"Welcome to floor %d!",tabaghe);
    for (int i = 0; i < 79; i++)
    {
        mvprintw(2,i,"-");
        mvprintw(27,i,"_");
    }
    for (int i = 0; i < 28; i++)
    {
        mvprintw(i,0,"|");
        mvprintw(i,79,"|");
    }

    int pos[15][2];

    int x,y,tool,arz, door1,door2,door3, m,n;
    int tale1x, tale1y, tale2x, tale2y, tale3x, tale3y, tale4x, tale4y;
    int a[2] = {0,11}, b[3] = {0,24,48};
    int w1, w2, w3, w4;
    w1 = random_number(0,2);
    if (w1 == 2) w2 = 0;
    else w2 = random_number(0,1);
    int ganj_room;
    if (tabaghe == 4)
        ganj_room = random_number(1,5);
    int telesm_room = random_number(1,5);
    int current_room = 1;
    

    for (int k = 0; k < 3; k++)
    {
        for (int l = 0; l < 2; l++)
        {
            y = random_number(3+a[l],5+a[l]); x = random_number(1+b[k],15+b[k]);
            move(y,x+1);
            tool = random_number(5,9); arz = random_number(5,7);
            //the actual 'tool' and 'arz' is 1 less than this random number
            if ((k == w1) && (l == w2)) {
                w3 = random_number(1,arz-1); w4 = random_number(1,tool-1);
            }
            tale1y = random_number(1,arz-1); tale1x = random_number(1,tool-1);
            if (current_room == telesm_room) attron(COLOR_PAIR(3));
            if (tabaghe == 4 && current_room == ganj_room) {
                attron(COLOR_PAIR(4));
                tale2y = random_number(1,arz-1); tale2x = random_number(1,tool-1);
                tale3y = random_number(1,arz-1); tale3x = random_number(1,tool-1);
                tale4y = random_number(1,arz-1); tale4x = random_number(1,tool-1);
            }

            for (int i = 1; i < tool; i++)
                addch('_');
            
            int sotoon1 = random_number(2,arz-2); int sotoon2 = random_number(2,tool-2);
            for (int i = 1; i <= arz; i++)
            {
                mvaddch(y+i,x,'|');
                for (int j = 1; j < tool; j++)
                {
                    if ((i == sotoon1) && (j == sotoon2)) {
                        addch('O');
                    }
                    else if ((tabaghe != 4) && (k == w1) && (l == w2) && (i == w3) && (j == w4)) {
                        addch('<');
                    }
                    else if ((tabaghe == 4) && (current_room == ganj_room) && ( ((i == tale1y) && (j == tale1x)) || ((i == tale2y) && (j == tale2x)) || ((i == tale3y) && (j == tale3x)) || ((i == tale4y) && (j == tale4x)) ) ) {
                            addch('^');
                    }
                    else if (((tabaghe == 4 && current_room != ganj_room) || (tabaghe != 4)) && (i == tale1y) && (j == tale1x))
                        addch('^');
                    else {
                        attron(COLOR_PAIR(1));
                        addch('.');
                        if (current_room == telesm_room) attron(COLOR_PAIR(3));
                        if (tabaghe == 4 && current_room == ganj_room) attron(COLOR_PAIR(4));
                    }
                }
                mvaddch(y+i,x+tool,'|');
            }
            move(y+arz,x+1);
            for (int i = 1; i < tool; i++)
                addch('_');

            tool--; arz--;

            if (k == 0 && l == 0) {
                door1 = random_number(1,arz); door2 = random_number(1,tool);
                pos[1][0] = (y + door1); pos[1][1] = (x + tool+1);
                pos[2][0] = (y + arz + 1); pos[2][1] = (x + door2);
            }
            if (k == 0 && l == 1) {
                door1 = random_number(1,arz); door2 = random_number(1,tool);
                pos[3][0] = (y + door1); pos[3][1] = (x + tool+1);
                pos[4][0] = (y); pos[4][1] = (x + door2);
            }
            if (k == 1 && l == 0) {
                door1 = random_number(1,arz); door2 = random_number(1,arz); door3 = random_number(1,tool);
                pos[5][0] = (y + door1); pos[5][1] = (x);
                pos[6][0] = (y + door2); pos[6][1] = (x + tool + 1);
                pos[13][0] = (y + arz + 1); pos[13][1] = (x + door3);
            }
            if (k == 1 && l == 1) {
                door1 = random_number(1,arz); door2 = random_number(1,arz); door3 = random_number(1,tool);
                pos[7][0] = (y + door1); pos[7][1] = (x);
                pos[8][0] = (y + door2); pos[8][1] = (x + tool + 1);
                pos[14][0] = (y); pos[14][1] = (x + door3);
            }
            if (k == 2 && l == 0) {
                door1 = random_number(1,arz); door2 = random_number(1,tool);
                pos[9][0] = (y + door1); pos[9][1] = (x);
                pos[10][0] = (y + arz + 1); pos[10][1] = (x + door2);
            }
            if (k == 2 && l == 1) {
                door1 = random_number(1,arz); door2 = random_number(1,tool);
                pos[11][0] = (y + door1); pos[11][1] = (x);
                pos[12][0] = (y); pos[12][1] = (x + door2);
            }
            attron(COLOR_PAIR(1));
            current_room++;
        }
    }
    location.x = x+1; location.y = y+1;
    for (int i = 1; i <= 14; i++)
    {
        mvaddch(pos[i][0],pos[i][1],'+');
    }
    
    if (pos[1][0] > pos[5][0]) hallway_ru(pos[1][0],pos[1][1],pos[5][0],pos[5][1]);
    else hallway_rd(pos[1][0],pos[1][1],pos[5][0],pos[5][1]);

    if (pos[6][0] > pos[9][0]) hallway_ru(pos[6][0],pos[6][1],pos[9][0],pos[9][1]);
    else hallway_rd(pos[6][0],pos[6][1],pos[9][0],pos[9][1]);

    if (pos[3][0] > pos[7][0]) hallway_ru(pos[3][0],pos[3][1],pos[7][0],pos[7][1]);
    else hallway_rd(pos[3][0],pos[3][1],pos[7][0],pos[7][1]);

    if (pos[8][0] > pos[11][0]) hallway_ru(pos[8][0],pos[8][1],pos[11][0],pos[11][1]);
    else hallway_rd(pos[8][0],pos[8][1],pos[11][0],pos[11][1]);

    if (pos[2][1] > pos[4][1]) hallway_ur(pos[4][0],pos[4][1],pos[2][0],pos[2][1]);
    else hallway_ul(pos[4][0],pos[4][1],pos[2][0],pos[2][1]);

    if (pos[10][1] > pos[12][1]) hallway_ur(pos[12][0],pos[12][1],pos[10][0],pos[10][1]);
    else hallway_ul(pos[12][0],pos[12][1],pos[10][0],pos[10][1]);

    if (pos[13][1] > pos[14][1]) hallway_ur(pos[14][0],pos[14][1],pos[13][0],pos[13][1]);
    else hallway_ul(pos[14][0],pos[14][1],pos[13][0],pos[13][1]);
    

    attroff(COLOR_PAIR(1));
    
    int ch;
    attron(COLOR_PAIR(2));
    mvaddch(location.y,location.x,'@');
    last_cell.y = location.y; last_cell.x = location.x; last_cell.s = '.';
    while (1)
    {
        ch = getch();
        if (ch == 'q') break;
        else {
            move_character(ch);
            attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(1));
            mvaddch(last_cell.y,last_cell.x,last_cell.s);
            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(2));
            last_cell.s = mvinch(location.y,location.x);
            last_cell.y = location.y; last_cell.x = location.x;
            mvaddch(location.y,location.x,'@');
        }
    }
    attroff(COLOR_PAIR(2));
    clear();
    refresh();
}

