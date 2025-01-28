#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>


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

int tabaghe = 1;
struct enemy {
    int health;
    int woke;
    int y;
    int x;
};
struct player_health_money_wapon {
    char username[30];
    int color;
    char current_weapon[30];

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
void clean_right_side();
void make_random_map();
int random_number(int a, int b);
int valid_move(int x, int y);
void move_character(int i);
void move_and_message();
void enemy_hit_us(int y, int x);
void initialize_enemy();
int wake_enemy(int x, int y);
void game_over();
void enemy_hit_long_range(int x, int y, int direction, int range, int hit);
char enemy_that_were_close_hit(int x, int y);

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
    for (int i = 80; i < 110; i++)
    {
        mvaddch(20,i,'_');
    }
    attron(COLOR_PAIR(4));
    mvprintw(21,80," ____                         ");
    mvprintw(22,80,"|  _ \\ ___   __ _ _   _  ___  ");
    mvprintw(23,80,"| |_) / _ \\ / _` | | | |/ _ \\ ");
    mvprintw(24,80,"|  _ < (_) | (_| | |_| |  __/ ");
    mvprintw(25,80,"|_| \\_\\___/ \\__, |\\__,_|\\___| ");
    mvprintw(26,80,"            |___/");
    attron(COLOR_PAIR(1));

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
                            deamon.y = y + i; deamon.x = x + j;
                            break;
                        case 2:
                            addch('F');
                            fire.y = y + i; fire.x = x + j;
                            break;
                        case 3:
                            addch('G');
                            giant.y = y + i; giant.x = x + j;
                            break;
                        case 4:
                            addch('M');
                            snake.y = y + i; snake.x = x + j;
                            break;
                        case 5:
                            addch('U');
                            undeed.y = y + i; undeed.x = x + j;
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
    int count_d = 0, count_f = 0;
    attron(COLOR_PAIR(player.color));
    mvaddch(location.y,location.x,'@');

    last_cell.y = location.y; last_cell.x = location.x; last_cell.s = '.';
    enemy_location.y = last_cell.y; enemy_location.x = last_cell.x; enemy_location.s = '.';
    last_last_cell.y = enemy_location.y; last_last_cell.x = enemy_location.x; last_last_cell.s = '.';
    while (1)
    {
        ch = getch();
        int direction;

        if (ch == 'q') break;

        else if (ch == ' ') {
            if (strcmp(player.current_weapon,"Mace") == 0)
            {
                switch (enemy_that_were_close_hit(location.x,location.y))
                {
                case 'D':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Deamon'!                                              ");
                    deamon.health -= 5;
                    if (deamon.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Deamon'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        deamon.woke = 0;
                    }
                    break;
                case 'F':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'FBM'!                                                 ");
                    fire.health -= 5;
                    if (fire.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'FBM'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        fire.woke = 0;
                    }
                    break;
                case 'G':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Giant'!                                               ");
                    giant.health -= 5;
                    if (giant.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Giant'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        giant.woke = 0;
                    }
                    break;
                case 'M':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Snake'!                                              ");
                    snake.health -= 5;
                    if (snake.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Snake'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        snake.woke = 0;
                    }
                    break;
                case 'U':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Undeed'!                                             ");
                    undeed.health -= 5;
                    if (undeed.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Undeed'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        undeed.woke = 0;
                    }
                    break;
                }
            }
            if (strcmp(player.current_weapon,"Sword") == 0)
            {
                switch (enemy_that_were_close_hit(location.x,location.y))
                {
                case 'D':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Deamon'!                                              ");
                    deamon.health -= 10;
                    if (deamon.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Deamon'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        deamon.woke = 0;
                    }
                    break;
                case 'F':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'FBM'!                                                 ");
                    fire.health -= 10;
                    if (fire.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'FBM'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        fire.woke = 0;
                    }
                    break;
                case 'G':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Giant'!                                               ");
                    giant.health -= 10;
                    if (giant.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Giant'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        giant.woke = 0;
                    }
                    break;
                case 'M':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Snake'!                                              ");
                    snake.health -= 10;
                    if (snake.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Snake'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        snake.woke = 0;
                    }
                    break;
                case 'U':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Undeed'!                                             ");
                    undeed.health -= 10;
                    if (undeed.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Undeed'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        undeed.woke = 0;
                    }
                    break;
                }
            }
            if (strcmp(player.current_weapon,"Arrow") == 0)
            {
                direction = getch();
                if (player.normal_arrow == 0)
                {
                    mvprintw(0,1,"You don't have any 'Arrow'!                                    ");
                    continue;
                }
                player.normal_arrow--;
                switch (direction)
                {
                case KEY_UP:
                    enemy_hit_long_range(location.x,location.y,1,5,5);
                    break;
                case KEY_RIGHT:
                    enemy_hit_long_range(location.x,location.y,2,5,5);
                    break;
                case KEY_DOWN:
                    enemy_hit_long_range(location.x,location.y,3,5,5);
                    break;
                case KEY_LEFT:
                    enemy_hit_long_range(location.x,location.y,4,5,5);
                    break;
                }
            }
            if (strcmp(player.current_weapon,"Wand") == 0)
            {
                direction = getch();
                if (player.magic_wand == 0)
                {
                    mvprintw(0,1,"You don't have any 'Wand'!                                    ");
                    continue;
                }
                player.magic_wand--;
                switch (direction)
                {
                case KEY_UP:
                    enemy_hit_long_range(location.x,location.y,1,10,15);
                    break;
                case KEY_RIGHT:
                    enemy_hit_long_range(location.x,location.y,2,10,15);
                    break;
                case KEY_DOWN:
                    enemy_hit_long_range(location.x,location.y,3,10,15);
                    break;
                case KEY_LEFT:
                    enemy_hit_long_range(location.x,location.y,4,10,15);
                    break;
                }
            }
            if (strcmp(player.current_weapon,"Dagger") == 0)
            {
                direction = getch();
                if (player.dagger == 0)
                {
                    mvprintw(0,1,"You don't have any 'Dagger'!                                  ");
                    continue;
                }
                player.dagger--;
                switch (direction)
                {
                case KEY_UP:
                    enemy_hit_long_range(location.x,location.y,1,5,12);
                    break;
                case KEY_RIGHT:
                    enemy_hit_long_range(location.x,location.y,2,5,12);
                    break;
                case KEY_DOWN:
                    enemy_hit_long_range(location.x,location.y,3,5,12);
                    break;
                case KEY_LEFT:
                    enemy_hit_long_range(location.x,location.y,4,5,12);
                    break;
                }
            }
        }

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
                if (player.health <= 0) {
                    game_over();
                }
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case 's':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Speed Potion'!                        ");
                player.potion_speed--;
                break;
            case 'h':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Health Potion'!                        ");
                player.potion_health--;
                break;
            case 'd':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Damage Potion'!                        ");
                player.potion_damage--;
                break;
            case 'A':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Normal Arrow'!                         ");
                player.normal_arrow--;
                break;
            case 'S':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Sword'!                                ");
                if (player.sword == 1) player.sword = 0;
                break;
            case 'W':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Magic Wand'!                          ");
                player.magic_wand--;
                break;
            case 'K':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Dagger'!                              ");
                player.dagger--;
                break;
            case 'f':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Food'!                                 ");
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
                    if (player.health <= 0) {
                        game_over();
                    }
                    attroff(COLOR_PAIR(1));
                    attron(COLOR_PAIR(player.color));
                    break;
                case 's':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Speed Potion' to your backpack!            ");
                last_cell.s = '.';
                break;
                case 'h':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Health Potion' to your backpack!            ");
                    last_cell.s = '.';
                    break;
                case 'd':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Damage Potion' to your backpack!            ");
                    last_cell.s = '.';
                    break;
                case 'A':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Normal Arrow' to your backpack!             ");
                    last_cell.s = '.';
                    break;
                case 'S':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 'Sword' to your backpack!                     ");
                    last_cell.s = '.';
                    break;
                case 'W':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Magic Wand' to your backpack!              ");
                    last_cell.s = '.';
                    break;
                case 'K':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Dagger' to your backpack!                 ");
                    last_cell.s = '.';
                    break;
                case 'f':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Food' to your backpack!                   ");
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
            clean_right_side();
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
            clean_right_side();
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
            clean_right_side();
            mvprintw(0,87,"Weapon Menu");
            mvprintw(4,80," weapon       dmg  rng  lft   ");
            mvprintw(5,80,"------------------------------");
            attron(COLOR_PAIR(1));
            mvprintw(6,80,"        (short ranges)        ");
            attron(COLOR_PAIR(2));
            mvprintw(7,80," Mace:(M)     5    1    inf   ");
            mvprintw(9,80,"------------------------------");
            attron(COLOR_PAIR(1));
            mvprintw(10,80,"        (long ranges)        ");
            attron(COLOR_PAIR(2));
            mvprintw(11,80," Dagger(K):   12   5    %d   ",player.dagger);
            mvprintw(12,80," Wand(W):     15   10   %d   ",player.magic_wand);
            mvprintw(13,80," Arrow(A):    5    5    %d   ",player.normal_arrow);
            if (player.sword == 1)
            mvprintw(8,80," Sword(S):    10   1    Yes   ");
            else
            mvprintw(8,80," Sword(S):    10   1    No    ");
            mvprintw(14,80,"------------------------------");
            attron(COLOR_PAIR(1));
            mvprintw(15,80,"       (current weapon)       ");
            attron(COLOR_PAIR(2));
            mvprintw(16,80," %s",player.current_weapon);
            char com; com = getch();

            if (com == 'z') {
                if (strcmp(player.current_weapon,"Mace") == 0)
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You put your 'Mace' in your backpack!         ");
                }
                else if (strcmp(player.current_weapon,"Dagger") == 0)
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You put a 'Dagger' in your backpack!          ");
                    player.dagger++;
                    attron(COLOR_PAIR(2));
                    mvprintw(11,80," Dagger(K):   12   5    %d   ",player.dagger);
                }
                else if (strcmp(player.current_weapon,"Sword") == 0)
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You put your 'Sword' in your backpack!        ");
                    attron(COLOR_PAIR(2));
                    mvprintw(8,80," Sword(S):    10   1    Yes   ");
                }
                else if (strcmp(player.current_weapon,"Arrow") == 0)
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You put an 'Arrow' in your backpack!         ");
                    player.normal_arrow++;
                    attron(COLOR_PAIR(2));
                    mvprintw(13,80," Arrow(A):    5    5    %d   ",player.normal_arrow);
                }
                else if (strcmp(player.current_weapon,"Wand") == 0)
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You put a 'Wand' in your backpack!           ");
                    player.magic_wand++;
                    attron(COLOR_PAIR(2));
                    mvprintw(12,80," Wand(W):     15   10   %d   ",player.magic_wand);
                }
                
                mvprintw(16,80,"press one of these {m,k,s,a,w}");
                char next_weapon; int valid = 1;
                while (valid)
                {
                    next_weapon = getch();
                    switch (next_weapon)
                    {
                        case 'm':
                            attron(COLOR_PAIR(1));
                            mvprintw(0,1,"You took your 'Mace'!                        ");
                            strcpy(player.current_weapon,"Mace");
                            attron(COLOR_PAIR(7));
                            mvprintw(7,80," Mace:(M)     5    1    inf   ");
                            attron(COLOR_PAIR(2));
                            mvprintw(16,80," Mace                         ");
                            valid = 0;
                            break;
                        case 's':
                            attron(COLOR_PAIR(1));
                            if (player.sword == 1) {
                                mvprintw(0,1,"You took your 'Sword'!                       ");
                                strcpy(player.current_weapon,"Sword");
                                valid = 0;
                                attron(COLOR_PAIR(7));
                                mvprintw(8,80," Sword(S):    10   1    Yes   ");
                                attron(COLOR_PAIR(2));
                                mvprintw(16,80," Sword                        ");
                            }
                            if (player.sword == 0) {
                                mvprintw(0,1,"You don't have 'Sword'!  pick another!       ");
                            }
                            break;
                        case 'k':
                            attron(COLOR_PAIR(1));
                            if (player.dagger == 0)
                            {
                                mvprintw(0,1,"You don't have any 'Dagger'!  pick another!  ");
                            }
                            else {
                                mvprintw(0,1,"You took a 'Dagger'!                        ");
                                attron(COLOR_PAIR(2));
                                mvprintw(16,80," Dagger                       ");
                                strcpy(player.current_weapon,"Dagger");
                                player.dagger--;
                                attron(COLOR_PAIR(7));
                                mvprintw(11,80," Dagger(K):   12   5    %d   ",player.dagger);
                                valid = 0;
                            }
                            break;
                        case 'a':
                            attron(COLOR_PAIR(1));
                            if (player.normal_arrow == 0)
                            {
                                mvprintw(0,1,"You don't have any 'Arrow'!  pick another!  ");
                            }
                            else {
                                mvprintw(0,1,"You took an 'Arrow'!                        ");
                                attron(COLOR_PAIR(2));
                                mvprintw(16,80," Arrow                        ");
                                strcpy(player.current_weapon,"Arrow");
                                player.normal_arrow--;
                                attron(COLOR_PAIR(7));
                                mvprintw(13,80," Arrow(A):    5    5    %d   ",player.normal_arrow);
                                valid = 0;
                            }
                            break;
                        case 'w':
                            attron(COLOR_PAIR(1));
                            if (player.magic_wand == 0)  {
                                mvprintw(0,1,"You don't have any 'Wand'!  pick another!  ");
                            }
                            else {
                                mvprintw(0,1,"You took a 'Wand'!                          ");
                                attron(COLOR_PAIR(2));
                                mvprintw(16,80," Wand                         ");
                                strcpy(player.current_weapon,"Wand");
                                player.magic_wand--;
                                attron(COLOR_PAIR(7));
                                mvprintw(12,80," Wand(W):     15   10   %d   ",player.magic_wand);
                                valid = 0;
                            }
                            break;
                    }
                }
            }
        }

        else {
            clean_right_side();


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
                if (player.health <= 0) {
                    game_over();
                }
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case 's':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Speed Potion' to your backpack!            ");
                last_cell.s = '.';
                break;
            case 'h':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Health Potion' to your backpack!            ");
                last_cell.s = '.';
                break;
            case 'd':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Damage Potion' to your backpack!            ");
                last_cell.s = '.';
                break;
            case 'A':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Normal Arrow' to your backpack!             ");
                last_cell.s = '.';
                break;
            case 'S':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 'Sword' to your backpack!                      ");
                last_cell.s = '.';
                break;
            case 'W':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Magic Wand' to your backpack!              ");
                last_cell.s = '.';
                break;
            case 'K':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Dagger' to your backpack!                 ");
                last_cell.s = '.';
                break;
            case 'f':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Food' to your backpack!                   ");
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

            if (deamon.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 1) {
                if (count_d == 6)
                {
                    count_d = 0;
                }
                else {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You woke up 'Deamon'!                           ");
                    mvaddch(deamon.y,deamon.x,'.');
                    deamon.woke = 1;
                }
            }

            if (deamon.woke == 1) {
                if (count_d == 6)
                {
                    attron(COLOR_PAIR(2));
                    mvaddch(enemy_location.y,enemy_location.x,'D');
                    deamon.woke = 0; deamon.x = enemy_location.x; deamon.y = enemy_location.y;
                    
                    last_cell.y = location.y; last_cell.x = location.x;
                    attron(COLOR_PAIR(player.color));
                    mvaddch(location.y,location.x,'@');

                    continue;
                }
                
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'D');
                count_d++;
                attroff(COLOR_PAIR(2));
                
            }

            if (fire.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 2) {
                if (count_f == 6)
                {
                    count_f = 0;
                }
                else {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You woke up 'Fire Breathing Monster'!           ");
                    mvaddch(fire.y,fire.x,'.');
                    fire.woke = 1;
                }
            }

            if (fire.woke == 1) {
                if (count_f == 6)
                {
                    attron(COLOR_PAIR(2));
                    mvaddch(enemy_location.y,enemy_location.x,'F');
                    fire.woke = 0; fire.x = enemy_location.x; fire.y = enemy_location.y;
                    
                    last_cell.y = location.y; last_cell.x = location.x;
                    attron(COLOR_PAIR(player.color));
                    mvaddch(location.y,location.x,'@');

                    continue;
                }
                
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'F');
                count_f++;
                attroff(COLOR_PAIR(2));
            }

            if (giant.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 3) {
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You woke up 'Giant'!                         ");
                mvaddch(giant.y,giant.x,'.');
                giant.woke = 1;
            }

            if (giant.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'G');
                attroff(COLOR_PAIR(2));
            }

            if (snake.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 4) {
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You woke up 'Snake'!                         ");
                mvaddch(snake.y,snake.x,'.');
                snake.woke = 1;
            }

            if (snake.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'M');
                attroff(COLOR_PAIR(2));
            }

            if (undeed.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 5) {
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You woke up 'Undeed'!                         ");
                mvaddch(undeed.y,undeed.x,'.');
                undeed.woke = 1;
            }

            if (undeed.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'U');
                attroff(COLOR_PAIR(2));
            }
            

            enemy_hit_us(location.y,location.x);

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

void enemy_hit_long_range(int x, int y, int direction, int range, int hit) {
    switch (direction)
    {
    case 1:
        for (int i = 1; i < range; i++)
        {
            char ch = mvinch(y-i, x) & A_CHARTEXT;
            if (ch == 'D')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Deamon'!                                             ");
                deamon.health -= hit;
                if (deamon.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Deamon'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    deamon.woke = 0;
                }
                break;
            }
            if (ch == 'F') {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'FBM'!                                                 ");
                fire.health -= hit;
                if (fire.health <= 0)
                {
                    mvprintw(0,1,"You killed 'FMB'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    fire.woke = 0;
                }
                break;
            }
            if (ch == 'G')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Giant'!                                               ");
                giant.health -= hit;
                if (giant.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Giant'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    giant.woke = 0;
                }
                break;
            }
            if (ch == 'U')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Undeed'!                                             ");
                undeed.health -= hit;
                if (undeed.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Undeed'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    undeed.woke = 0;
                }
                break;
            }
            if (ch == 'M')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Snake'!                                             ");
                snake.health -= hit;
                if (snake.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Snake'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    snake.woke = 0;
                }
                break;
            }
        }
        break;
    case 2:
        for (int i = 1; i < range; i++)
        {
            char ch = mvinch(y, x+i) & A_CHARTEXT;
            if (ch == 'D')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Deamon'!                                             ");
                deamon.health -= hit;
                if (deamon.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Deamon'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    deamon.woke = 0;
                }
                break;
            }
            if (ch == 'F') {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'FBM'!                                                 ");
                fire.health -= hit;
                if (fire.health <= 0)
                {
                    mvprintw(0,1,"You killed 'FMB'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    fire.woke = 0;
                }
                break;
            }
            if (ch == 'G')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Giant'!                                               ");
                giant.health -= hit;
                if (giant.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Giant'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    giant.woke = 0;
                }
                break;
            }
            if (ch == 'U')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Undeed'!                                             ");
                undeed.health -= hit;
                if (undeed.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Undeed'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    undeed.woke = 0;
                }
                break;
            }
            if (ch == 'M')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Snake'!                                             ");
                snake.health -= hit;
                if (snake.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Snake'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    snake.woke = 0;
                }
                break;
            }
        }
        break;
    case 3:
        for (int i = 1; i < range; i++)
        {
            char ch = mvinch(y+i, x) & A_CHARTEXT;
            if (ch == 'D')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Deamon'!                                             ");
                deamon.health -= hit;
                if (deamon.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Deamon'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    deamon.woke = 0;
                }
                break;
            }
            if (ch == 'F') {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'FBM'!                                                 ");
                fire.health -= hit;
                if (fire.health <= 0)
                {
                    mvprintw(0,1,"You killed 'FMB'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    fire.woke = 0;
                }
                break;
            }
            if (ch == 'G')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Giant'!                                               ");
                giant.health -= hit;
                if (giant.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Giant'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    giant.woke = 0;
                }
                break;
            }
            if (ch == 'U')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Undeed'!                                             ");
                undeed.health -= hit;
                if (undeed.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Undeed'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    undeed.woke = 0;
                }
                break;
            }
            if (ch == 'M')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Snake'!                                             ");
                snake.health -= hit;
                if (snake.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Snake'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    snake.woke = 0;
                }
                break;
            }
        }
        break;
    case 4:
        for (int i = 1; i < range; i++)
        {
            char ch = mvinch(y, x-i) & A_CHARTEXT;
            if (ch == 'D')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Deamon'!                                             ");
                deamon.health -= hit;
                if (deamon.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Deamon'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    deamon.woke = 0;
                }
                break;
            }
            if (ch == 'F') {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'FBM'!                                                 ");
                fire.health -= hit;
                if (fire.health <= 0)
                {
                    mvprintw(0,1,"You killed 'FMB'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    fire.woke = 0;
                }
                break;
            }
            if (ch == 'G')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Giant'!                                               ");
                giant.health -= hit;
                if (giant.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Giant'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    giant.woke = 0;
                }
                break;
            }
            if (ch == 'U')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Undeed'!                                             ");
                undeed.health -= hit;
                if (undeed.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Undeed'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    undeed.woke = 0;
                }
                break;
            }
            if (ch == 'M')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Snake'!                                             ");
                snake.health -= hit;
                if (snake.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Snake'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    snake.woke = 0;
                }
                break;
            }
        }
        break;
    }
    
}

char enemy_that_were_close_hit(int x, int y) {
    char ch1 = mvinch(y-1, x-1) & A_CHARTEXT;
    char ch2 = mvinch(y-1, x) & A_CHARTEXT;
    char ch3 = mvinch(y-1, x+1) & A_CHARTEXT;
    char ch4 = mvinch(y, x-1) & A_CHARTEXT;
    char ch5 = mvinch(y, x+1) & A_CHARTEXT;
    char ch6 = mvinch(y+1, x-1) & A_CHARTEXT;
    char ch7 = mvinch(y+1, x) & A_CHARTEXT;
    char ch8 = mvinch(y+1, x+1) & A_CHARTEXT;

    if (ch1 == 'D' || ch2 == 'D' || ch3 == 'D' || ch4 == 'D' || ch5 == 'D' || ch6 == 'D' || ch7 == 'D' || ch8 == 'D')
        return 'D';

    if (ch1 == 'F' || ch2 == 'F' || ch3 == 'F' || ch4 == 'F' || ch5 == 'F' || ch6 == 'F' || ch7 == 'F' || ch8 == 'F')
        return 'F';

    if (ch1 == 'G' || ch2 == 'G' || ch3 == 'G' || ch4 == 'G' || ch5 == 'G' || ch6 == 'G' || ch7 == 'G' || ch8 == 'G')
        return 'G';

    if (ch1 == 'M' || ch2 == 'M' || ch3 == 'M' || ch4 == 'M' || ch5 == 'M' || ch6 == 'M' || ch7 == 'M' || ch8 == 'M')
        return 'M';

    if (ch1 == 'U' || ch2 == 'U' || ch3 == 'U' || ch4 == 'U' || ch5 == 'U' || ch6 == 'U' || ch7 == 'U' || ch8 == 'U')
        return 'U';
}

void enemy_hit_us(int y, int x) {
    char ch1 = mvinch(y-1, x) & A_CHARTEXT;
    char ch2 = mvinch(y+1, x) & A_CHARTEXT;
    char ch3 = mvinch(y, x+1) & A_CHARTEXT;
    char ch4 = mvinch(y, x-1) & A_CHARTEXT;
    char ch5 = mvinch(y, x) & A_CHARTEXT;
    if (ch1 == 'D' || ch2 == 'D' || ch3 == 'D' || ch4 == 'D' || ch5 == 'D') {
        player.health -= 1;
        attron(COLOR_PAIR(2));
        mvprintw(0,1,"You were hit by Deamon! You lost 1 health!           ");
        attron(COLOR_PAIR(1));
        mvprintw(1,21,"Health: %d ",player.health);
        if (player.health <= 0)
            game_over();
    }
    if (ch1 == 'F' || ch2 == 'F' || ch3 == 'F' || ch4 == 'F' || ch5 == 'F') {
        player.health -= 2;
        attron(COLOR_PAIR(2));
        mvprintw(0,1,"You were hit by FBD! You lost 2 health!           ");
        attron(COLOR_PAIR(1));
        mvprintw(1,21,"Health: %d ",player.health);
        if (player.health <= 0)
            game_over();
    }
    if (ch1 == 'G' || ch2 == 'G' || ch3 == 'G' || ch4 == 'G' || ch5 == 'G') {
            player.health -= 3;
            attron(COLOR_PAIR(2));
            mvprintw(0,1,"You were hit by Giant! You lost 3 health!           ");
            attron(COLOR_PAIR(1));
            mvprintw(1,21,"Health: %d ",player.health);
            if (player.health <= 0)
                game_over();
    }
    if (ch1 == 'M' || ch2 == 'M' || ch3 == 'M' || ch4 == 'M' || ch5 == 'M') {
        player.health -= 4;
        attron(COLOR_PAIR(2));
        mvprintw(0,1,"You were hit by Snake! You lost 4 health!           ");
        attron(COLOR_PAIR(1));
        mvprintw(1,21,"Health: %d ",player.health);
        if (player.health <= 0)
            game_over();
    }
    if (ch1 == 'U' || ch2 == 'U' || ch3 == 'U' || ch4 == 'U' || ch5 == 'U') {
        player.health -= 5;
        attron(COLOR_PAIR(2));
        mvprintw(0,1,"You were hit by Unded! You lost 5 health!           ");
        attron(COLOR_PAIR(1));
        mvprintw(1,21,"Health: %d ",player.health);
        if (player.health <= 0)
            game_over();
    }
}

void clean_right_side() {
    for (int i = 3; i <= 19; i++)
    {
        mvprintw(i,80,"                              ");
    }
    mvprintw(0,80,"                              ");
    mvprintw(1,80,"                              ");
}

void initialize_player() {
    player.gold = 0; player.point = 0; player.health = 100; player.color = 2;
    player.dagger = 0; player.potion_damage = 0;
    player.magic_wand = 0; player.normal_arrow = 0;
    player.sword = 0; player.potion_speed = 0; player.potion_health = 0; player.food = 0;
    strcpy(player.current_weapon,"Mace");
}

void initialize_enemy() {
    deamon.woke = 0; snake.woke = 0; undeed.woke = 0; fire.woke = 0; giant.woke = 0;
    deamon.health = 5; snake.health = 20; undeed.health = 30; fire.health = 10; giant.health = 15;
    deamon.x = 0; deamon.y = 0; snake.y = 0; snake.x = 0;
    undeed.x = 0; undeed.y = 0; fire.x = 0; fire.y = 0; giant.x = 0; giant.y = 0;
}

void game_over() {
    mvprintw(1,21,"Health: %d ",0);
    mvprintw(0,1,"Game Over!!                                      ");
    //save_game(0);
    refresh();
    napms(5000);
    refresh();
    clear();
    endwin();
    //main_menu();
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

int main() {
    srand(time(0));
    initialize_player();
    make_random_map();
    endwin();
    return 0;
}