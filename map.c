#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

void make_random_map() {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_CYAN,COLOR_BLACK);
    attron(COLOR_PAIR(1));
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
    int a[2] = {0,11}, b[3] = {0,24,48};
    for (int k = 0; k < 3; k++)
    {
        for (int l = 0; l < 2; l++)
        {
            y = random_number(3+a[l],5+a[l]); x = random_number(1+b[k],15+b[k]);
            move(y,x+1);
            tool = random_number(5,9); arz = random_number(5,7);
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
                    else addch('.');
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
        }
    }
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
    

    while (1)
    {
        if (getch() == 10) break;
    }
    clear();
    refresh();
}