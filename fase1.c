//403105771
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>

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
int tabaghe = 1;
struct player_hlth_mny {
    char username[30];
    int color;
    int gold;
    int point;
    int health;
    char current_weapon;
    int mace;
    int dagger;
    int magic_wand;
    int normal_arrow;
    int sword;
    int potion_health;
    int potion_speed;
    int potion_damage;
    int food;
};
struct player_hlth_mny player;


int save_game(int won);
void make_random_map();
int compare(const void *a, const void *b);
void scoreboard2(char name[30]);
void scoreboard1(char name[30]);
void game_menu_user(char name[30]);
void hero_color(char name[30]);
void difficulty(char name[30]);
void setting(char name[30]);
int check_login(char username[30], char password[30]);
void login_menu();
void new_user();
int correct_password(char str[30]);
int correct_email(char str[30]);
void main_menu();
int random_number(int a, int b);
int valid_move(int x, int y);
void move_character(int i);

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
    player.normal_arrow++;
        return 1;
        break;
    case 'K':
    player.dagger++;
        return 1;
        break;
    case 'W':
    player.magic_wand++;
        return 1;
        break;
    case 'M':
    player.mace++;
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
    for (int i = 0; i < 105; i++)
    {
        mvaddch(2,i,'-');
        mvaddch(27,i,'_');
    }
    for (int i = 0; i < 28; i++)
    {
        mvaddch(i,0,'|');
        mvaddch(i,79,'|');
        mvaddch(i,105,'|');
    }

    int pos[15][2];

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
    int foodx, foody; int finishx, finishy;
    

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
                    else if ( (current_room == ganj_room) && ((i == telesm1y) && (j == telesm1x)) ) {
                        attron(COLOR_PAIR(3));
                        addch('h');
                        attroff(COLOR_PAIR(3));
                    }
                    else if ( (current_room == ganj_room) && ((i == telesm2y) && (j == telesm2x)) ) {
                        attron(COLOR_PAIR(3));
                        addch('d');
                        attroff(COLOR_PAIR(3));
                    }
                    else if ( (current_room == ganj_room) && ((i == telesm3y) && (j == telesm3x)) ) {
                        attron(COLOR_PAIR(3));
                        addch('s');
                        attroff(COLOR_PAIR(3));
                    }
                    else if ( (current_room != ganj_room) && ((i == telesm1y) && (j == telesm1x)) ) {
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
                        int a = random_number(1,5);
                        attron(COLOR_PAIR(6));
                        switch (a)
                        {
                        case 1:
                            addch('M');
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
                        case 5:
                            addch('S');
                            break;
                        }
                        attroff(COLOR_PAIR(6));
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
    
    int ch, ch2;
    attron(COLOR_PAIR(player.color));
    mvaddch(location.y,location.x,'@');
    last_cell.y = location.y; last_cell.x = location.x; last_cell.s = '.';
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
                    save_game(0);
                    refresh();
                    napms(5000);
                    refresh();
                    clear();
                    main_menu();
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
                mvprintw(0,1,"You ignored the 'damage sword'!                         ");
                player.sword--;
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
            case 'M':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'mace'!                                 ");
                player.mace--;
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
                        save_game(0);
                        refresh();
                        napms(5000);
                        refresh();
                        clear();
                        main_menu();
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
                    mvprintw(0,1,"You add 1 'sword' to your backpack!                    ");
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
                case 'M':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'mace' to your backpack!                   ");
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
                    save_game(1);
                    refresh();
                    napms(5000);
                    refresh();
                    clear();
                    main_menu();
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
            mvprintw(4,83,"Health Potion:    %d   ",player.mace);
            mvprintw(5,83,"Speed Potion:     %d   ",player.dagger);
            mvprintw(6,83,"Damage Potion:    %d   ",player.magic_wand);
            mvprintw(7,81,"                      ");
            mvprintw(8,81,"                      ");
            mvprintw(9,81,"                      ");
            mvprintw(10,81,"                      ");
        }

        else if (ch == 'i') {
            attron(COLOR_PAIR(2));
            mvprintw(0,87,"Weapon Menu");
            mvprintw(4,83,"Mace:          %d   ",player.mace);
            mvprintw(5,83,"Dagger:        %d   ",player.dagger);
            mvprintw(6,83,"Magic Wand:    %d   ",player.magic_wand);
            mvprintw(7,83,"Normal Arrow:  %d   ",player.normal_arrow);
            mvprintw(8,83,"Sword:         %d   ",player.sword);
            mvprintw(9,81,"                      ");
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
                    mvprintw(0,1,"Game Over!!                               ");
                    save_game(0);
                    refresh();
                    napms(5000);
                    refresh();
                    clear();
                    main_menu();
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
                mvprintw(0,1,"You add 1 'sword' to your backpack!                    ");
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
            case 'M':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'mace' to your backpack!                   ");
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
                save_game(1);
                refresh();
                napms(5000);
                refresh();
                clear();
                main_menu();
                break;
            }
            last_cell.y = location.y; last_cell.x = location.x;
            mvaddch(location.y,location.x,'@');
        }
    }
    attroff(COLOR_PAIR(player.color));
    clear();
    refresh();
}

void initalize_player() {
    player.gold = 0; player.point = 0; player.health = 100; player.color = 2;
    player.mace = 1; player.dagger = 0; player.potion_damage = 0;
    player.magic_wand = 0; player.normal_arrow = 0;
    player.sword = 0; player.potion_speed = 0; player.potion_health = 0; player.food = 0;
}

int save_game(int won) {
    if (strcmp(player.username,"guest") == 0) return 0;
    FILE* user = fopen("username.txt","r");
    FILE* gold = fopen("gold.txt","r");
    FILE* point = fopen("points.txt","r");
    FILE* sg = fopen("started_game_number.txt","r");
    FILE* fg = fopen("finished_game_number.txt","r");
    char u[30]; int line = 0; int x = 0;
    char g[6], s[6], f[6], p[6]; int g2=0, s2=0, f2=0, p2=0;
    int g3[50], s3[50], f3[50], p3[50];
    while (fgets(u,30,user) != NULL)
    {
        u[strlen(u)-1] = '\0';
        if (strcmp(u,player.username) == 0) break;
        line++;
    }

    while (fgets(g,4,gold) != NULL)
    {
        g[strlen(g)-1] = '\0';
        sscanf(g,"%d",&g2);
        if (x == line) {
            g2 += player.gold;
        }
        g3[x] = g2;
        x++;
    }
    fclose(gold);
    gold = fopen("gold.txt","w");
    for (int i = 0; i < x; i++)
    {
        fprintf(gold,"%d\n",g3[i]);
    }
    fclose(gold); x = 0;

    while (fgets(p,4,point) != NULL)
    {
        p[strlen(p)-1] = '\0';
        sscanf(p,"%d",&p2);
        if (x == line) {
            p2 += player.point;
        }
        p3[x] = p2;
        x++;
    }
    fclose(point);
    point = fopen("points.txt","w");
    for (int i = 0; i < x; i++)
    {
        fprintf(point,"%d\n",p3[i]);
    }
    fclose(point); x = 0;

    while (fgets(s,4,sg) != NULL)
    {
        s[strlen(s)-1] = '\0';
        sscanf(s,"%d",&s2);
        if (x == line) {
            s2++;
        }
        s3[x] = s2;
        x++;
    }
    fclose(sg);
    sg = fopen("started_game_number.txt","w");
    for (int i = 0; i < x; i++)
    {
        fprintf(sg,"%d\n",s3[i]);
    }
    fclose(sg); x = 0;
    if (won != 1)
        return 0;
    
    while (fgets(f,4,fg) != NULL)
    {
        f[strlen(f)-1] = '\0';
        sscanf(f,"%d",&f2);
        if (x == line) {
            f2++;
        }
        f3[x] = f2;
        x++;
    }
    fclose(fg);
    fg = fopen("finished_game_number.txt","w");
    for (int i = 0; i < x; i++)
    {
        fprintf(fg,"%d\n",f3[i]);
    }
    fclose(fg);
}

void scoreboard1(char name[30]) {
    setlocale(LC_ALL, "");
    char top_user[10][30]; int top_point[10], top_gold[10], top_fg[10], top_sg[10]; int p[10], g[10], fg1[10], sg1[10];
    FILE* user = fopen("username.txt","r");
    FILE* points = fopen("points.txt","r");
    FILE* gold = fopen("gold.txt","r");
    FILE* sg = fopen("started_game_number.txt","r");
    FILE* fg = fopen("finished_game_number.txt","r");
    char str[30]; int p1[100], p2[100]; int x = 0; char u[50][30];
    while (fscanf(points, "%d", &p1[x]) != EOF) {
        p2[x] = p1[x];
        x++;
    }
    int r = 0;
    while (fscanf(gold, "%d", &g[r]) != EOF) r++;
    r = 0;
    while (fscanf(sg, "%d", &sg1[r]) != EOF) r++;
    r = 0;
    while (fscanf(fg, "%d", &fg1[r]) != EOF) r++;
    int n = x;
    qsort(p2,x,sizeof(int), compare);

    int y = 0;
    while (fgets(u[y],30,user) != NULL)
    {
        u[y][strlen(u[y])-1] = '\0';
        y++;
    }

    for (int i = 0; i < 10; i++)
    {
        --x;
        for (int j = 0; j < n; j++)
        {
            if (p1[j] == p2[x])
            {
                top_point[i] = p2[x];
                top_gold[i] = g[j];
                top_fg[i] = fg1[j];
                top_sg[i] = sg1[j];
                strcpy(top_user[i],u[j]);
            }
        }
    }
    fclose(user); fclose(gold); fclose(sg); fclose(fg); fclose(points);

    //-------------------------------------------------------------------------------------

    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    init_pair(2,COLOR_WHITE,COLOR_RED);
    init_pair(3,COLOR_WHITE,COLOR_YELLOW);
    init_pair(4,COLOR_WHITE,COLOR_BLUE);
    wchar_t gold_medal[] = L"🏅";
    wchar_t silver_medal[] = L"🥈";
    wchar_t bronze_medal[] = L"🥉";
    printw("|rank| username                               |point|gold|fgame|exp|\n");

    if (strcmp(name,top_user[0]) == 0) attron(A_BOLD | A_UNDERLINE);
    attron(COLOR_PAIR(2) | A_ITALIC);
    printw("|1   |                                        |     |    |     |   |\n");
    mvaddwstr(1, 3, gold_medal);
    int q = 0;
    mvprintw((q+1),7,"%-25s",top_user[q]); mvprintw((q+1),31,"%-10s","(The Legend)");
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-5d",top_fg[q]); mvprintw((q+1),64,"%-3d",top_sg[q]);
    attroff(COLOR_PAIR(2) | A_ITALIC);
    if (strcmp(name,top_user[0]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[1]) == 0) attron(A_BOLD | A_UNDERLINE);
    attron(COLOR_PAIR(3) | A_ITALIC);
    move(2,0);
    printw("|2   |                                        |     |    |     |   |\n");
    mvaddwstr(2, 3, silver_medal);
    q++;
    mvprintw((q+1),7,"%-25s",top_user[q]); mvprintw((q+1),31,"%-10s","(The Goat)");
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-5d",top_fg[q]); mvprintw((q+1),64,"%-3d",top_sg[q]);
    attroff(COLOR_PAIR(2) | A_ITALIC);
    if (strcmp(name,top_user[1]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[2]) == 0) attron(A_BOLD | A_UNDERLINE);
    attron(COLOR_PAIR(4) | A_ITALIC);
    move(3,0);
    printw("|3   |                                        |     |    |     |   |\n");
    mvaddwstr(3, 3, bronze_medal);
    q++;
    mvprintw((q+1),7,"%-25s",top_user[q]); mvprintw((q+1),31,"%-10s","(The Champion)");
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-5d",top_fg[q]); mvprintw((q+1),64,"%-3d",top_sg[q]);
    attroff(COLOR_PAIR(4) | A_ITALIC);
    if (strcmp(name,top_user[2]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[3]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(4,0);
    printw("|4   |                                        |     |    |     |   |\n");
    q++;
    mvprintw((q+1),7,"%-25s",top_user[q]);
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-5d",top_fg[q]); mvprintw((q+1),64,"%-3d",top_sg[q]);
    if (strcmp(name,top_user[3]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[4]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(5,0);
    printw("|5   |                                        |     |    |     |   |\n");
    q++;
    mvprintw((q+1),7,"%-25s",top_user[q]);
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-5d",top_fg[q]); mvprintw((q+1),64,"%-3d",top_sg[q]);
    if (strcmp(name,top_user[4]) == 0) attroff(A_BOLD | A_UNDERLINE);

    attron(COLOR_PAIR(1));
    const char *menu[] = {"next page", "back to the game menu"};
    int choice = 0;
    while (1)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(8 + i, 3, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 1 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 1) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    attroff(COLOR_PAIR(1));
    clear();
    switch (choice)
    {
    case 0:
        refresh();
        clear();
        scoreboard2(name);
        break;
    case 1:
        game_menu_user(name);
        break;
    }
    refresh();
}

void scoreboard2(char name[30]) {
    char top_user[10][30]; int top_point[10], top_gold[10], top_fg[10], top_sg[10]; int p[10], g[10], fg1[10], sg1[10];
    FILE* user = fopen("username.txt","r");
    FILE* points = fopen("points.txt","r");
    FILE* gold = fopen("gold.txt","r");
    FILE* sg = fopen("started_game_number.txt","r");
    FILE* fg = fopen("finished_game_number.txt","r");
    char str[30]; int p1[100], p2[100]; int x = 0; char u[50][30];
    while (fscanf(points, "%d", &p1[x]) != EOF) {
        p2[x] = p1[x];
        x++;
    }
    int r = 0;
    while (fscanf(gold, "%d", &g[r]) != EOF) r++;
    r = 0;
    while (fscanf(sg, "%d", &sg1[r]) != EOF) r++;
    r = 0;
    while (fscanf(fg, "%d", &fg1[r]) != EOF) r++;
    int n = x;
    qsort(p2,x,sizeof(int), compare);

    int y = 0;
    while (fgets(u[y],30,user) != NULL)
    {
        u[y][strlen(u[y])-1] = '\0';
        y++;
    }

    for (int i = 0; i < 10; i++)
    {
        --x;
        for (int j = 0; j < n; j++)
        {
            if (p1[j] == p2[x])
            {
                top_point[i] = p2[x];
                top_gold[i] = g[j];
                top_fg[i] = fg1[j];
                top_sg[i] = sg1[j];
                strcpy(top_user[i],u[j]);
            }
        }
    }
    fclose(user); fclose(gold); fclose(sg); fclose(fg); fclose(points);

    //-------------------------------------------------------------------------------------

    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    move(0,0);
    printw("|rank| username                               |point|gold|fgame|exp|\n");

    if (strcmp(name,top_user[5]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(1,0);
    printw("|6   |                                        |     |    |     |   |\n");
    int q = 5;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-5d",top_fg[q]); mvprintw((q-4),64,"%-3d",top_sg[q]);
    if (strcmp(name,top_user[5]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[6]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(2,0);
    printw("|7   |                                        |     |    |     |   |\n");
    q++;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-5d",top_fg[q]); mvprintw((q-4),64,"%-3d",top_sg[q]);
    if (strcmp(name,top_user[6]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[7]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(3,0);
    printw("|8   |                                        |     |    |     |   |\n");
    q++;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-5d",top_fg[q]); mvprintw((q-4),64,"%-3d",top_sg[q]);
    if (strcmp(name,top_user[7]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[8]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(4,0);
    printw("|9   |                                        |     |    |     |   |\n");
    q++;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-5d",top_fg[q]); mvprintw((q-4),64,"%-3d",top_sg[q]);
    if (strcmp(name,top_user[8]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[9]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(5,0);
    printw("|10  |                                        |     |    |     |   |\n");
    q++;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-5d",top_fg[q]); mvprintw((q-4),64,"%-3d",top_sg[q]);
    if (strcmp(name,top_user[9]) == 0) attroff(A_BOLD | A_UNDERLINE);

    attron(COLOR_PAIR(1));
    const char *menu[] = {"last page", "back to the game menu"};
    int choice = 0;
    while (1)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(8 + i, 3, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 1 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 1) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    attroff(COLOR_PAIR(1));
    clear();
    switch (choice)
    {
    case 0:
        refresh();
        clear();
        scoreboard1(name);
        break;
    case 1:
        game_menu_user(name);
        break;
    }
    refresh();
}

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void game_menu_user(char name[30]) {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    attron(COLOR_PAIR(1));
    const char *menu[] = {"New Game", "Load Game","Settings","Scoreboard","Back"};
    int choice = 0;
    while (1)
    {
        for (int i = 0; i < 5; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 4 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 4) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    switch (choice)
    {
    case 0:
        tabaghe = 1;
        make_random_map();
        break;
    case 1:
        main_menu();
        break;
    case 2:
        setting(name);
        break;
    case 3:
        scoreboard1(name);
        break;
    case 4:
        main_menu();
        break;
    }
    echo();
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
}

void hero_color(char name[30]) {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    attron(COLOR_PAIR(1));
    const char *menu[] = {"Red", "Green", "Purple","Yellow"};
    int choice = 0;
    while (1)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 3 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 3) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    echo();
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
    switch (choice)
    {
    case 0:
        player.color = 2;
        break;
    case 1:
        player.color = 6;
        break;
    case 2:
        player.color = 3;
        break;
    case 3:
        player.color = 4;
        break;
    }
    setting(name);
}

void difficulty(char name[30]) {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    attron(COLOR_PAIR(1));
    const char *menu[] = {"Amatur", "Regular", "Profesioanl"};
    int choice = 0;
    while (1)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 2 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    echo();
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
    switch (choice)
    {
    case 0:
        player.health = 150;
        break;
    case 1:
        player.health = 100;
        break;
    case 2:
        player.health = 50;
        break;
    }
    setting(name);
}

void setting(char name[30]) {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    attron(COLOR_PAIR(1));
    const char *menu[] = {"Difficulty", "Color of Hero", "Back"};
    int choice = 0;
    while (1)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 2 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    switch (choice)
    {
    case 0:
        difficulty(name);
        break;
    case 1:
        hero_color(name);
        break;
    case 2:
        game_menu_user(name);
        break;
    }
    echo();
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
}

int check_login(char username[30], char password[30]) {
    FILE* user = fopen("username.txt","r");
    FILE* pass = fopen("password.txt","r");
    char user2[30], pass2[30]; int u = 0, p = 0, line = 0;
    while (fgets(user2,30,user) != NULL)
    {
        line++;
        user2[strlen(user2)-1] = '\0';
        if (strcmp(user2,username) == 0)
        {
            u = 1;
            break;
        }
    }
    fclose(user);
    if (u == 0) {
        printw("username wan not found!\n");
        return 0;
    }
    for (int i = 0; i < line; i++) fgets(pass2,30,pass);
    pass2[strlen(pass2)-1] = '\0';
    if (strcmp(pass2,password) == 0) p = 1;
    fclose(pass);
    if (p == 0)
    {
        printw("password is incorrect!\n");
        return 0;
    }
    if (p && u)
    {
        return 1;
    }
    return 0;
}

void login_menu() {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    attron(COLOR_PAIR(1));
    const char *menu[] = {"login to your account", "login as guest", "Back"};
    int choice = 0;
    while (1)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 2 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    echo();
    switch (choice)
    {
    case 0:
        clear();
        char username[30], password[30];
        printw("Enter Username: ");
        scanw("%s",username);
        printw("Enter Password: ");
        scanw("%s",password);
        while (!check_login(username,password))
        {
            printw("Enter Username: ");
            scanw("%s",username);
            printw("Enter Password: ");
            scanw("%s",password);
        }
        strcpy(player.username,username);
        attroff(COLOR_PAIR(1));
        clear();
        printw("you are logged in successfully!\n");
        printw("press any key to continue...\n");
        getch();
        game_menu_user(username);
        refresh();
    case 1:
        refresh();
        clear();
        strcpy(player.username,"guest");
        make_random_map();
        break;
    case 2:
        refresh();
        clear();
        main_menu();
        break;
    }
    clear();
    refresh();
}

void new_user() {
    initscr();
    clear();
    echo();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    init_pair(1,COLOR_WHITE,COLOR_BLACK);
    attron(COLOR_PAIR(1));
    char username[30], username2[30];
    char password[30];
    char email[30];
    FILE *user = fopen("username.txt", "a+");
    FILE *pass = fopen("password.txt", "a+");
    FILE *em = fopen("email.txt", "a+");
    
    while (1)
    {
        int is_tekrari = 0;
        printw("Enter a Username: ");
        scanw("%s", username);
        while (fgets(username2,30,user) != NULL)
        {
            username2[strlen(username2)-1] = '\0';
            if (strcmp(username2,username) == 0)
            {
                printw("this username has been taken! please choose something else.\n");
                is_tekrari = 1;
                break;
            }
        }
        if (is_tekrari == 0)
        {
            break;
        }
    }

    while (1)
    {
        printw("Enter a Password: ");
        scanw("%s", password);
        if (!correct_password(password))
        {
            printw("password must have at least 1 uppercase and 1 lowercase and 1 digit!\n");
        }
        if (strlen(password) < 7)
        {
            printw("it must be at least 7 charachters!\n");
        }
        if (correct_password(password) && (strlen(password) >= 7))
        {
            break;
        }
    }

    while (1)
    {
        printw("Enter your email: ");
        scanw("%s", email);
        if (!correct_email(email)) {
            printw("incorrect format!\n");
        }
        else break;
    }
    fprintf(user,"%s\n",username);
    fprintf(pass,"%s\n",password);
    fprintf(em,"%s\n",email);
    fclose(user); fclose(pass); fclose(em);
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
    printw("a new user was made!\n");
    printw("press any key to continue...\n");
    getch();
    main_menu();
}

int correct_password(char str[30]) {
    int big = 0, small = 0, number = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if ((str[i] >= 'a') && (str[i] <= 'z')) small = 1;
        if ((str[i] >= 'A') && (str[i] <= 'Z')) big = 1;
        if ((str[i] >= '0') && (str[i] <= '9')) number = 1;
    }
    if (big && small && number) return 1;
    else return 0;
}

int correct_email(char str[30]) {
    int x = 0, y = 0, z = 0; int a;
    for (int i = 0; (i < strlen(str) && str[i] != '@'); i++)
    {
        if (isalpha(str[i]) || isdigit(str[i]))
        {
            x = 1;
        }
        a = i;
    }
    for (int i = (a+2); (i < strlen(str) && str[i] != '.'); i++)
    {
        if (isalpha(str[i]) || isdigit(str[i]))
        {
            y = 1;
        }
        a = i;
    }
    for (int i = (a+2); i < strlen(str); i++)
    {
        if (isalpha(str[i]) || isdigit(str[i]))
        {
            z = 1;
        }
    }
    if (x && y && z) return 1;
    else return 0;
}

void main_menu() {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);
    attron(COLOR_PAIR(1));
    const char *menu[] = {"New User", "Login","Exit"};
    int choice = 0;
    while (1)
    {
        mvprintw(1,1,"Welcome to Rouge! ;)");
        for (int i = 0; i < 3; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 2 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
        else if (ch == 10)
            break;
    }
    switch (choice)
    {
    case 0:
        new_user();
        break;
    case 1:
        login_menu();
        break;
    case 2:
        endwin();
        break;
    }
    echo();
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
}

int main() {
    srand(time(0));
    initalize_player();
    main_menu();
    return 0;
}