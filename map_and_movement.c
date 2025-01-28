#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

// g f i p e are special keys till now

struct position {
    int y;
    int x;
};
struct position2 {
    int y;
    int x;
    char s;
};
struct position location;
struct position2 last_cell;
struct position2 last_last_cell;
struct position2 enemy_location;

int tabaghe = 1, count = 0;
struct enemy {
    int health;
    int woke;
};
struct player_health_money_wapon {
    char username[30];
    int color;
    char current_weapon;

    int gold;
    int point;
    int health;

    int dagger;
    int magic_wand;
    int normal_arrow;
    int sword;

    int potion_health;
    int potion_speed;
    int potion_damage;

    int food;
};
struct player_health_money_wapon player;
struct enemy deamon; struct enemy snake; struct enemy undeed; struct enemy fire; struct enemy giant;


int save_game(int won);
void make_random_map();
int random_number(int a, int b);
int valid_move(int x, int y);
void move_character(int i);
void move_and_message();
void initialize_enemy();
int wake_enemy(int x, int y);

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
    switch (next_ch)
    {
    case '.':
        return 1;
        break;
    case '#':
        return 1;
        break;
    case '+':
        return 1;
        break;
    case '<':
        tabaghe++;
        if (tabaghe <= 4)
            make_random_map();
        break;
    case '$':
        player.gold++;
        player.point += 10;
        return 1;
        break;
    case 'g':
        player.gold++;
        player.point += 50;
        return 1;
        break;
    case '^':
        player.health -= 5;
        return 1;
        break;
    case 's':
        player.potion_speed++;
        return 1;
        break;
    case 'h':
        player.potion_health++;
        return 1;
        break;
    case 'd':
        player.potion_damage++;
        return 1;
        break;
    case 'S':
        player.sword++;
        return 1;
        break;
    case 'A':
        player.normal_arrow += 20;
        return 1;
        break;
    case 'K':
        player.dagger += 10;
        return 1;
        break;
    case 'W':
        player.magic_wand += 8;
        return 1;
        break;
    case 'f':
        player.food++;
        return 1;
        break;
    case '!':
        return 1;
        break;
    }
    return 0;
}

int wake_enemy(int x, int y) {
    char ch1 = mvinch(y-1, x-1) & A_CHARTEXT;
    char ch2 = mvinch(y-1, x) & A_CHARTEXT;
    char ch3 = mvinch(y-1, x+1) & A_CHARTEXT;
    char ch4 = mvinch(y, x-1) & A_CHARTEXT;
    char ch5 = mvinch(y, x+1) & A_CHARTEXT;
    char ch6 = mvinch(y+1, x-1) & A_CHARTEXT;
    char ch7 = mvinch(y+1, x) & A_CHARTEXT;
    char ch8 = mvinch(y+1, x+1) & A_CHARTEXT;

    if (ch1 == 'D' || ch2 == 'D' || ch3 == 'D' || ch4 == 'D' || ch5 == 'D' || ch6 == 'D' || ch7 == 'D' || ch8 == 'D')
        return 1;

    if (ch1 == 'F' || ch2 == 'F' || ch3 == 'F' || ch4 == 'F' || ch5 == 'F' || ch6 == 'F' || ch7 == 'F' || ch8 == 'F')
        return 2;

    if (ch1 == 'G' || ch2 == 'G' || ch3 == 'G' || ch4 == 'G' || ch5 == 'G' || ch6 == 'G' || ch7 == 'G' || ch8 == 'G')
        return 3;

    if (ch1 == 'M' || ch2 == 'M' || ch3 == 'M' || ch4 == 'M' || ch5 == 'M' || ch6 == 'M' || ch7 == 'M' || ch8 == 'M')
        return 4;

    if (ch1 == 'U' || ch2 == 'U' || ch3 == 'U' || ch4 == 'U' || ch5 == 'U' || ch6 == 'U' || ch7 == 'U' || ch8 == 'U')
        return 5;
    
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
    init_pair(5,COLOR_BLUE,COLOR_BLACK);
    init_pair(6,COLOR_GREEN,COLOR_BLACK);
    init_pair(7,COLOR_WHITE,COLOR_BLACK);
    attron(COLOR_PAIR(1));
    mvprintw(0,1,"Welcome to floor %d!",tabaghe);
    mvprintw(1,1,"Gold: %d ",player.gold);
    mvprintw(1,10,"Point: %d ",player.point);
    mvprintw(1,21,"Health: %d ",player.health);
    for (int i = 0; i < 110; i++)
    {
        mvaddch(2,i,'-');
        mvaddch(27,i,'_');
    }
    for (int i = 0; i < 28; i++)
    {
        mvaddch(i,0,'|');
        mvaddch(i,79,'|');
        mvaddch(i,110,'|');
    }
    mvprintw(21,80," ____                         ");
    mvprintw(22,80,"|  _ \\ ___   __ _ _   _  ___  ");
    mvprintw(23,80,"| |_) / _ \\ / _` | | | |/ _ \\ ");
    mvprintw(24,80,"|  _ < (_) | (_| | |_| |  __/ ");
    mvprintw(25,80,"|_| \\_\\___/ \\__, |\\__,_|\\___| ");
    mvprintw(26,80,"            |___/");

    int pos[15][2];
    initialize_enemy();

    int x,y,tool,arz, door1,door2,door3, m,n;
    int tale1x, tale1y, tale2x, tale2y, tale3x, tale3y, tale4x, tale4y;
    int telesm1x, telesm1y, telesm2x, telesm2y, telesm3x, telesm3y;
    int gx, gy; int weaponx, weapony;
    int a[2] = {0,11}, b[3] = {0,24,48};
    int w1, w2, w3, w4;
    w1 = random_number(0,2);
    if (w1 == 2) w2 = 0;
    else w2 = random_number(0,1);
    int ganj_room;
    int telesm_room = random_number(1,5);
    if (tabaghe == 4)
        ganj_room = ((telesm_room + 2) % 5 ) + 1;
    int current_room = 1;
    int black_gold_room = random_number(1,6);
    int foodx, foody; int enemyx, enemyy;
    int a2 = random_number(1,5);

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
            gy = random_number(1,arz-1); gx = random_number(1,tool-1);
            tale1y = random_number(1,arz-1); tale1x = random_number(1,tool-1);
            if (current_room == telesm_room) {
                attron(COLOR_PAIR(3));
                telesm2x = random_number(1,tool-1); telesm2y = random_number(1,arz-1);
                telesm3x = random_number(1,tool-1); telesm3y = random_number(1,arz-1);
            }
            if (tabaghe == 4 && current_room == ganj_room) {
                attron(COLOR_PAIR(4));
                tale2y = random_number(1,arz-1); tale2x = random_number(1,tool-1);
                tale3y = random_number(1,arz-1); tale3x = random_number(1,tool-1);
                tale4y = random_number(1,arz-1); tale4x = random_number(1,tool-1);
            }
            telesm1x = random_number(1,tool-1); telesm1y = random_number(1,arz-1);
            weaponx = random_number(1,tool-1); weapony = random_number(1,arz-1);
            foodx = random_number(1,tool-1); foody = random_number(1,arz-1);
            enemyx = random_number(1,tool-1); enemyy = random_number(1,arz-1);



            for (int i = 1; i < tool; i++)
                addch('_');
            
            int sotoon1 = random_number(2,arz-2); int sotoon2 = random_number(2,tool-2);
            for (int i = 1; i <= arz; i++)
            {
                mvaddch(y+i,x,'|');
                for (int j = 1; j < tool; j++)
                {
                    if ((i == sotoon1) && (j == sotoon2)) {
                        attron(COLOR_PAIR(1));
                        addch('O');
                        attroff(COLOR_PAIR(1));
                    }
                    else if ((i == gy) && (j == gx) && (current_room == black_gold_room)) {
                        attron(COLOR_PAIR(4));
                        addch('g');
                        attroff(COLOR_PAIR(4));
                    }
                    else if ((i == gy) && (j == gx) && (current_room != black_gold_room)) {
                        attron(COLOR_PAIR(4));
                        addch('$');
                        attroff(COLOR_PAIR(4));
                    }
                    else if ((tabaghe == 4) && (current_room == ganj_room) && ( ((i == tale1y) && (j == tale1x)) || ((i == tale2y) && (j == tale2x)) || ((i == tale3y) && (j == tale3x)) || ((i == tale4y) && (j == tale4x)) ) ) {
                        attron(COLOR_PAIR(1));
                        addch('^');
                        attroff(COLOR_PAIR(1));
                    }
                    else if (((tabaghe == 4 && current_room != ganj_room) || (tabaghe != 4)) && (i == tale1y) && (j == tale1x)) {
                        attron(COLOR_PAIR(1));
                        addch('^');
                        attroff(COLOR_PAIR(1));
                    }
                    else if ( (current_room == telesm_room) && ((i == telesm1y) && (j == telesm1x)) ) {
                        attron(COLOR_PAIR(3));
                        addch('h');
                        attroff(COLOR_PAIR(3));
                    }
                    else if ( (current_room == telesm_room) && ((i == telesm2y) && (j == telesm2x)) ) {
                        attron(COLOR_PAIR(3));
                        addch('d');
                        attroff(COLOR_PAIR(3));
                    }
                    else if ( (current_room == telesm_room) && ((i == telesm3y) && (j == telesm3x)) ) {
                        attron(COLOR_PAIR(3));
                        addch('s');
                        attroff(COLOR_PAIR(3));
                    }
                    else if ( (current_room != telesm_room) && ((i == telesm1y) && (j == telesm1x)) ) {
                        int a = random_number(1,3);
                        attron(COLOR_PAIR(3));
                        switch (a)
                        {
                        case 1:
                            addch('d');
                            break;
                        case 2:
                            addch('s');
                            break;
                        case 3:
                            addch('h');
                            break;
                        }
                        attroff(COLOR_PAIR(3));
                    }
                    else if ( (i == weapony) && (j == weaponx) ) {
                        int a = random_number(1,4);
                        attron(COLOR_PAIR(6));
                        switch (a)
                        {
                        case 1:
                            addch('S');
                            break;
                        case 2:
                            addch('K');
                            break;
                        case 3:
                            addch('W');
                            break;
                        case 4:
                            addch('A');
                            break;
                        }
                        attroff(COLOR_PAIR(6));
                    }
                    else if (((tabaghe == 4 && current_room != ganj_room) || (tabaghe != 4)) && (current_room != telesm_room) && (current_room != 6) && (i == enemyy) && (j == enemyx) ) {
                        a2 = ((a2+2) % 5) + 1;
                        attron(COLOR_PAIR(2));
                        switch (a2)
                        {
                        case 1:
                            addch('D');
                            break;
                        case 2:
                            addch('F');
                            break;
                        case 3:
                            addch('G');
                            break;
                        case 4:
                            addch('M');
                            break;
                        case 5:
                            addch('U');
                            break;
                        }
                        attroff(COLOR_PAIR(2));
                    }
                    else if ((i == foody) && (j == foodx)) {
                        attron(COLOR_PAIR(7));
                        addch('f');
                        attroff(COLOR_PAIR(7));
                    }
                    else {
                        attron(COLOR_PAIR(1));
                        addch('.');
                    }
                    if ((tabaghe == 4) && (current_room == ganj_room) && (i == (arz-1)) && (j == (tool-2))) {
                        attron(COLOR_PAIR(2));
                        addch('!');
                        attroff(COLOR_PAIR(2));
                    }
                    if ((tabaghe != 4) && (k == w1) && (l == w2) && (i == 1) && (j == (tool-2))) {
                        attron(COLOR_PAIR(1));
                        addch('<');
                        attroff(COLOR_PAIR(1));
                    }
                }
                if (current_room == telesm_room) attron(COLOR_PAIR(3));
                else if (tabaghe == 4 && current_room == ganj_room) attron(COLOR_PAIR(4));
                else attron(COLOR_PAIR(1));
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

    move_and_message();
}

void move_and_message() {
    noecho();
    curs_set(0);
    start_color();
    keypad(stdscr, TRUE);
    init_pair(1,COLOR_CYAN,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    init_pair(5,COLOR_BLUE,COLOR_BLACK);
    init_pair(6,COLOR_GREEN,COLOR_BLACK);
    init_pair(7,COLOR_WHITE,COLOR_BLACK);

    int ch, ch2;
    attron(COLOR_PAIR(player.color));
    mvaddch(location.y,location.x,'@');

    last_cell.y = location.y; last_cell.x = location.x; last_cell.s = '.';
    enemy_location.y = last_cell.y; enemy_location.x = last_cell.x; enemy_location.s = '.';
    last_last_cell.y = enemy_location.y; last_last_cell.x = enemy_location.x; last_last_cell.s = '.';
    while (1)
    {
        ch = getch();

        if (ch == 'q') break;

        else if (ch == 'g') {
            ch2 = getch();
            move_character(ch2);
            attroff(COLOR_PAIR(player.color));
            attron(COLOR_PAIR(1));
            mvaddch(last_cell.y,last_cell.x,last_cell.s);
            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(player.color));
            last_cell.s = mvinch(location.y,location.x);


            switch (last_cell.s)
            {
            case '$':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'normal gold'!                     ");
                player.gold--; player.point-=10;
                mvprintw(1,1,"Gold: %d ",player.gold);
                mvprintw(1,10,"Point: %d ",player.point);
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case 'g':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'black gold'!                      ");
                player.gold--; player.point-=50;
                mvprintw(1,1,"Gold: %d ",player.gold);
                mvprintw(1,10,"Point: %d ",player.point);
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case '^':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You were Trapped! You lost 5 points!                 ");
                mvprintw(1,21,"Health: %d ",player.health);
                if (player.health == 0) {
                    mvprintw(1,21,"Health: %d ",0);
                    mvprintw(0,1,"Game Over!!                             ");
                    //save_game(0);
                    refresh();
                    napms(5000);
                    refresh();
                    clear();
                    //main_menu();
                }
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case 's':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'speed potion'!                        ");
                player.potion_speed--;
                break;
            case 'h':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'health potion'!                        ");
                player.potion_health--;
                break;
            case 'd':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'damage potion'!                        ");
                player.potion_damage--;
                break;
            case 'A':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'normal arrow'!                         ");
                player.normal_arrow--;
                break;
            case 'S':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'sword'!                                ");
                if (player.sword == 1) player.sword = 0;
                break;
            case 'W':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'magic wand'!                          ");
                player.magic_wand--;
                break;
            case 'K':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'dagger'!                              ");
                player.dagger--;
                break;
            case 'f':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'food'!                                 ");
                player.food--;
                break;
            }
            last_cell.y = location.y; last_cell.x = location.x;
            mvaddch(location.y,location.x,'@');
        }
        
        else if (ch == 'f') {
            ch2 = getch();
            for (int i = 0; i < 40; i++)
            {
                move_character(ch2);
                attroff(COLOR_PAIR(player.color));
                attron(COLOR_PAIR(1));
                mvaddch(last_cell.y,last_cell.x,last_cell.s);
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                last_cell.s = mvinch(location.y,location.x);


                switch (last_cell.s)
                {
                case '$':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You've got a normal gold with 10 points!       ");
                    mvprintw(1,1,"Gold: %d ",player.gold);
                    mvprintw(1,10,"Point: %d ",player.point);
                    attroff(COLOR_PAIR(1));
                    attron(COLOR_PAIR(player.color));
                    last_cell.s = '.';
                    break;
                case 'g':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You've got a black gold with 50 points!      ");
                    mvprintw(1,1,"Gold: %d ",player.gold);
                    mvprintw(1,10,"Point: %d ",player.point);
                    attroff(COLOR_PAIR(1));
                    attron(COLOR_PAIR(player.color));
                    last_cell.s = '.';
                    break;
                case '^':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You were Trapped! You lost 5 points!                 ");
                    mvprintw(1,21,"Health: %d ",player.health);
                    if (player.health == 0) {
                        mvprintw(1,21,"Health: %d ",0);
                        mvprintw(0,1,"Game Over!!                              ");
                        //save_game(0);
                        refresh();
                        napms(5000);
                        refresh();
                        clear();
                        //main_menu();
                    }
                    attroff(COLOR_PAIR(1));
                    attron(COLOR_PAIR(player.color));
                    break;
                case 's':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'speed potion' to your backpack!            ");
                last_cell.s = '.';
                break;
                case 'h':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'health potion' to your backpack!            ");
                    last_cell.s = '.';
                    break;
                case 'd':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'damage potion' to your backpack!            ");
                    last_cell.s = '.';
                    break;
                case 'A':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'normal arrow' to your backpack!             ");
                    last_cell.s = '.';
                    break;
                case 'S':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 'sword' to your backpack!                     ");
                    last_cell.s = '.';
                    break;
                case 'W':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'magic wand' to your backpack!              ");
                    last_cell.s = '.';
                    break;
                case 'K':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'dagger' to your backpack!                 ");
                    last_cell.s = '.';
                    break;
                case 'f':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'food' to your backpack!                   ");
                    last_cell.s = '.';
                    break;
                case '!':
                    attron(COLOR_PAIR(2));
                    mvprintw(0,1,"Congratulations!! You've finished Rouge successfully!!");
                    //save_game(1);
                    refresh();
                    napms(5000);
                    refresh();
                    clear();
                    //main_menu();
                    break;
                }
                last_cell.y = location.y; last_cell.x = location.x;
                mvaddch(location.y,location.x,'@');
                }
        }
        
        else if (ch == 'e') {
            attron(COLOR_PAIR(2));
            mvprintw(0,87,"Food Menu");
            mvprintw(4,83,"Normal Food:    %d   ",player.food);
            mvprintw(5,83,"Suoreme Food:   0   ");
            mvprintw(6,83,"Magic Food:     0   ");
            mvprintw(7,83,"Corrupt Food:   0   ");
            mvprintw(8,81,"                      ");
            mvprintw(9,83,"press enter to eat");
            mvprintw(10,83,"a piece of food...");
            int ch3 = getch();
            if ((ch3 == 10) && (player.food > 0)) {
                player.food--;
                player.health+= 5;
                attron(COLOR_PAIR(1));
                mvprintw(1,21,"Health: %d ",player.health);
                mvprintw(0,1,"You ate a food and got 5 points!              ");
            }
            else if ((ch3 == 10) && (player.food == 0)) {
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"Sorry! You don't have any food!                   ");
            }
        }

        else if (ch == 'p') {
            attron(COLOR_PAIR(2));
            mvprintw(0,87,"Potion Menu");
            mvprintw(4,83,"Health Potion:    %d   ",player.potion_health);
            mvprintw(5,83,"Speed Potion:     %d   ",player.potion_speed);
            mvprintw(6,83,"Damage Potion:    %d   ",player.potion_damage);
            mvprintw(7,81,"                      ");
            mvprintw(8,81,"                      ");
            mvprintw(9,81,"                      ");
            mvprintw(10,81,"                      ");
        }

        else if (ch == 'i') {
            attron(COLOR_PAIR(2));
            mvprintw(0,87,"Weapon Menu");
            mvprintw(4,80,"weapon     dmg  rng  lft");
            mvprintw(5,80,"Mace:      5    1    inf");
            mvprintw(6,80,"Dagger(K): 12   5    %d  ",player.dagger);
            mvprintw(7,80,"Wand(W):   15   10   %d  ",player.magic_wand);
            mvprintw(8,80,"Arrow(A):  5    5    %d  ",player.normal_arrow);
            player.sword == 1 ? mvprintw(9,80,"Sword(S):  10   1    Yes") : mvprintw(9,80,"Sword(S):  10   1    No ");
            mvprintw(10,81,"                      ");
        }

        else {
            mvprintw(0,81,"                      ");
            mvprintw(4,81,"                      ");
            mvprintw(5,81,"                      ");
            mvprintw(6,81,"                      ");
            mvprintw(7,81,"                      ");
            mvprintw(8,81,"                      ");
            mvprintw(9,81,"                      ");
            mvprintw(10,81,"                      ");


            move_character(ch);
            attroff(COLOR_PAIR(player.color));
            attron(COLOR_PAIR(1));

            mvaddch(last_cell.y,last_cell.x,last_cell.s);
            mvaddch(last_last_cell.y,last_last_cell.x,last_last_cell.s);

            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(player.color));

            last_cell.s = mvinch(location.y,location.x);
            last_last_cell.s = mvinch(enemy_location.y,enemy_location.x);

            



            switch (last_cell.s)
            {
            case '$':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You've got a normal gold with 10 points!       ");
                mvprintw(1,1,"Gold: %d ",player.gold);
                mvprintw(1,10,"Point: %d ",player.point);
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                last_cell.s = '.';
                break;
            case 'g':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You've got a black gold with 50 points!      ");
                mvprintw(1,1,"Gold: %d ",player.gold);
                mvprintw(1,10,"Point: %d ",player.point);
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                last_cell.s = '.';
                break;
            case '^':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You were Trapped! You lost 5 points!                 ");
                mvprintw(1,21,"Health: %d ",player.health);
                if (player.health == 0) {
                    mvprintw(1,21,"Health: %d ",0);
                    mvprintw(0,1,"Game Over!!                               ");
                    //save_game(0);
                    refresh();
                    napms(5000);
                    refresh();
                    clear();
                    //main_menu();
                }
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case 's':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'speed potion' to your backpack!            ");
                last_cell.s = '.';
                break;
            case 'h':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'health potion' to your backpack!            ");
                last_cell.s = '.';
                break;
            case 'd':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'damage potion' to your backpack!            ");
                last_cell.s = '.';
                break;
            case 'A':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'normal arrow' to your backpack!             ");
                last_cell.s = '.';
                break;
            case 'S':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 'sword' to your backpack!                      ");
                last_cell.s = '.';
                break;
            case 'W':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'magic wand' to your backpack!              ");
                last_cell.s = '.';
                break;
            case 'K':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'dagger' to your backpack!                 ");
                last_cell.s = '.';
                break;
            case 'f':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'food' to your backpack!                   ");
                last_cell.s = '.';
                break;
            case '!':
                attron(COLOR_PAIR(2));
                mvprintw(0,1,"Congratulations!! You've finished Rouge successfully!!");
                //save_game(1);
                refresh();
                napms(5000);
                refresh();
                clear();
                //main_menu();
                break;
            }

            if (deamon.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 1)
                deamon.woke = 1;

            if (deamon.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'D');
                attroff(COLOR_PAIR(2));
            }

            if (fire.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 2)
                fire.woke = 1;

            if (fire.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'F');
                attroff(COLOR_PAIR(2));
            }

            if (giant.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 3)
                giant.woke = 1;

            if (giant.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'G');
                attroff(COLOR_PAIR(2));
            }

            if (snake.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 4)
                snake.woke = 1;

            if (snake.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'M');
                attroff(COLOR_PAIR(2));
            }

            if (undeed.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 5)
                undeed.woke = 1;

            if (undeed.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'U');
                attroff(COLOR_PAIR(2));
            }

            last_last_cell.y = enemy_location.y; last_last_cell.x = enemy_location.x;
            enemy_location.y = last_cell.y; enemy_location.x = last_cell.x;
            last_cell.y = location.y; last_cell.x = location.x;
            
            attron(COLOR_PAIR(player.color));
            mvaddch(location.y,location.x,'@');
        }
    }
    attroff(COLOR_PAIR(player.color));
    clear();
    refresh();
}

void initialize_player() {
    player.gold = 0; player.point = 0; player.health = 100; player.color = 2;
    player.dagger = 0; player.potion_damage = 0;
    player.magic_wand = 0; player.normal_arrow = 0;
    player.sword = 0; player.potion_speed = 0; player.potion_health = 0; player.food = 0;
}

void initialize_enemy() {
    deamon.woke = 0; snake.woke = 0; undeed.woke = 0; fire.woke = 0; giant.woke = 0;
    deamon.health = 5; snake.health = 20; undeed.health = 30; fire.health = 10; giant.health = 15;
}

// int save_game(int won) {
//     if (strcmp(player.username,"guest") == 0) return 0;
//     FILE* user = fopen("username.txt","r");
//     FILE* gold = fopen("gold.txt","r");
//     FILE* point = fopen("points.txt","r");
//     FILE* sg = fopen("started_game_number.txt","r");
//     FILE* fg = fopen("finished_game_number.txt","r");
//     char u[30]; int line = 0; int x = 0;
//     char g[6], s[6], f[6], p[6]; int g2=0, s2=0, f2=0, p2=0;
//     int g3[50], s3[50], f3[50], p3[50];
//     while (fgets(u,30,user) != NULL)
//     {
//         u[strlen(u)-1] = '\0';
//         if (strcmp(u,player.username) == 0) break;
//         line++;
//     }
//     while (fgets(g,4,gold) != NULL)
//     {
//         g[strlen(g)-1] = '\0';
//         sscanf(g,"%d",&g2);
//         if (x == line) {
//             g2 += player.gold;
//         }
//         g3[x] = g2;
//         x++;
//     }
//     fclose(gold);
//     gold = fopen("gold.txt","w");
//     for (int i = 0; i < x; i++)
//     {
//         fprintf(gold,"%d\n",g3[i]);
//     }
//     fclose(gold); x = 0;
//     while (fgets(p,4,point) != NULL)
//     {
//         p[strlen(p)-1] = '\0';
//         sscanf(p,"%d",&p2);
//         if (x == line) {
//             p2 += player.point;
//         }
//         p3[x] = p2;
//         x++;
//     }
//     fclose(point);
//     point = fopen("points.txt","w");
//     for (int i = 0; i < x; i++)
//     {
//         fprintf(point,"%d\n",p3[i]);
//     }
//     fclose(point); x = 0;
//     while (fgets(s,4,sg) != NULL)
//     {
//         s[strlen(s)-1] = '\0';
//         sscanf(s,"%d",&s2);
//         if (x == line) {
//             s2++;
//         }
//         s3[x] = s2;
//         x++;
//     }
//     fclose(sg);
//     sg = fopen("started_game_number.txt","w");
//     for (int i = 0; i < x; i++)
//     {
//         fprintf(sg,"%d\n",s3[i]);
//     }
//     fclose(sg); x = 0;
//     if (won != 1)
//         return 0;
//     while (fgets(f,4,fg) != NULL)
//     {
//         f[strlen(f)-1] = '\0';
//         sscanf(f,"%d",&f2);
//         if (x == line) {
//             f2++;
//         }
//         f3[x] = f2;
//         x++;
//     }
//     fclose(fg);
//     fg = fopen("finished_game_number.txt","w");
//     for (int i = 0; i < x; i++)
//     {
//         fprintf(fg,"%d\n",f3[i]);
//     }
//     fclose(fg);
// }

