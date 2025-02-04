//403105771
//Rogue Game Project
//last update: 2/4/2025


#define _GNU_SOURCE
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
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
struct enemy {
    int health;
    int woke;
    int y;
    int x;
    int live;
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

    int ph_on;
    int ps_on;
    int pd_on;

    int food;
    int damage_food;
    int speed_food;
    int energy;

    int floor;
    int x; int y;
    int can_load;
};
struct room {
    int xs;
    int ys;
    int xf;
    int yf;
    int visited;
};


struct room room1; struct room room2; struct room room3; struct room room4; struct room room5; struct room room6;
struct position location; struct position2 last_cell; struct position2 last_last_cell; struct position2 enemy_location;
struct player_health_money_wapon player;
struct enemy deamon; struct enemy snake; struct enemy undeed; struct enemy fire; struct enemy giant; struct enemy boss;


int tabaghe = 1; int count_hungry = 0, count_hpotion = 0,count_spotion = 0,count_dpotion = 0, food_change = 0, sfood_change = 0, dfood_change = 0;
chtype last_map[27][110]; char last_map_simple[27][110]; chtype played_map[27][110];
int pos[30][10] = {{0}}; int window[4][2] = {{0}};
char string[20] = ""; int max_health = 100;
const char* musics[] = {"1.mp3","2.mp3","3.mp3"}; int current_music = 1;


int save_game(int won);
void clean_right_side();
void make_random_map();
int random_number(int a, int b);
int valid_move(int x, int y);
void move_character(int i);
void move_character_2x(int i);
void move_and_message();
void enemy_hit_us(int y, int x);
void initialize_enemy();
int wake_enemy(int x, int y);
void check_game_over();
int enemy_hit_long_range(int x, int y, int direction, int range, int hit);
char enemy_that_were_close_hit(int x, int y);
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
int main_menu();
int initSDL();
int music(char name[30]);
Mix_Music* playmusic(const char* musicFile);
void initialize_player();
void hallway_rd(int y1, int x1,int y2, int x2);
void hallway_ru(int y1, int x1,int y2, int x2);
void hallway_ur(int y1, int x1,int y2, int x2);
void hallway_ul(int y1, int x1,int y2, int x2);
void pass_gen();
void clean_map();
void save_map_before_quit();
void save_map();
void print_map_loading();
void print_hallway();
void profile_menu(char name[30]);
void print_rooms();
void print_room1(); void print_room2(); void print_room3(); void print_room4(); void print_room5(); void print_room6();



void scoreboard1(char name[30]) {
    setlocale(LC_ALL, "");
    char top_user[10][30]; int top_point[10], top_gold[10], top_fg[10]; int p[10], g[10], fg1[10];
    FILE* user = fopen("username.txt","r");
    FILE* points = fopen("points.txt","r");
    FILE* gold = fopen("gold.txt","r");
    FILE* fg = fopen("finished_game_number.txt","r");
    char str[30]; int p1[100], p2[100]; int x = 0; char u[50][30];
    while (fscanf(points, "%d", &p1[x]) != EOF) {
        p2[x] = p1[x];
        x++;
    }
    int r = 0;
    while (fscanf(gold, "%d", &g[r]) != EOF) r++;
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
                strcpy(top_user[i],u[j]);
            }
        }
    }
    fclose(user); fclose(gold); fclose(fg); fclose(points);

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
    printw("|rank| username                               |point|gold|exp|\n");

    if (strcmp(name,top_user[0]) == 0) attron(A_BOLD | A_UNDERLINE);
    attron(COLOR_PAIR(2) | A_ITALIC);
    printw("|1   |                                        |     |    |   |\n");
    mvaddwstr(1, 3, gold_medal);
    int q = 0;
    mvprintw((q+1),7,"%-25s",top_user[q]); mvprintw((q+1),31,"%-10s","(The Legend)");
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-3d",top_fg[q]);
    attroff(COLOR_PAIR(2) | A_ITALIC);
    if (strcmp(name,top_user[0]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[1]) == 0) attron(A_BOLD | A_UNDERLINE);
    attron(COLOR_PAIR(3) | A_ITALIC);
    move(2,0);
    printw("|2   |                                        |     |    |   |\n");
    mvaddwstr(2, 3, silver_medal);
    q++;
    mvprintw((q+1),7,"%-25s",top_user[q]); mvprintw((q+1),31,"%-10s","(The Goat)");
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-3d",top_fg[q]);
    attroff(COLOR_PAIR(2) | A_ITALIC);
    if (strcmp(name,top_user[1]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[2]) == 0) attron(A_BOLD | A_UNDERLINE);
    attron(COLOR_PAIR(4) | A_ITALIC);
    move(3,0);
    printw("|3   |                                        |     |    |   |\n");
    mvaddwstr(3, 3, bronze_medal);
    q++;
    mvprintw((q+1),7,"%-25s",top_user[q]); mvprintw((q+1),31,"%-10s","(The Champion)");
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-3d",top_fg[q]);
    attroff(COLOR_PAIR(4) | A_ITALIC);
    if (strcmp(name,top_user[2]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[3]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(4,0);
    printw("|4   |                                        |     |    |   |\n");
    q++;
    mvprintw((q+1),7,"%-25s",top_user[q]);
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-3d",top_fg[q]);
    if (strcmp(name,top_user[3]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[4]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(5,0);
    printw("|5   |                                        |     |    |   |\n");
    q++;
    mvprintw((q+1),7,"%-25s",top_user[q]);
    mvprintw((q+1),47,"%-5d",top_point[q]); mvprintw((q+1),53,"%-4d",top_gold[q]);
    mvprintw((q+1),58,"%-3d",top_fg[q]);
    if (strcmp(name,top_user[4]) == 0) attroff(A_BOLD | A_UNDERLINE);

    attron(COLOR_PAIR(4));
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
    char top_user[10][30]; int top_point[10], top_gold[10], top_fg[10]; int p[10], g[10], fg1[10];
    FILE* user = fopen("username.txt","r");
    FILE* points = fopen("points.txt","r");
    FILE* gold = fopen("gold.txt","r");
    FILE* fg = fopen("finished_game_number.txt","r");
    char str[30]; int p1[100], p2[100]; int x = 0; char u[50][30];
    while (fscanf(points, "%d", &p1[x]) != EOF) {
        p2[x] = p1[x];
        x++;
    }
    int r = 0;
    while (fscanf(gold, "%d", &g[r]) != EOF) r++;
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
                strcpy(top_user[i],u[j]);
            }
        }
    }
    fclose(user); fclose(gold); fclose(fg); fclose(points);

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
    move(0,0);
    printw("|rank| username                               |point|gold|exp|\n");

    if (strcmp(name,top_user[5]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(1,0);
    printw("|6   |                                        |     |    |   |\n");
    int q = 5;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-3d",top_fg[q]);
    if (strcmp(name,top_user[5]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[6]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(2,0);
    printw("|7   |                                        |     |    |   |\n");
    q++;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-3d",top_fg[q]);
    if (strcmp(name,top_user[6]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[7]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(3,0);
    printw("|8   |                                        |     |    |   |\n");
    q++;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-3d",top_fg[q]);
    if (strcmp(name,top_user[7]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[8]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(4,0);
    printw("|9   |                                        |     |    |   |\n");
    q++;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-3d",top_fg[q]);
    if (strcmp(name,top_user[8]) == 0) attroff(A_BOLD | A_UNDERLINE);

    if (strcmp(name,top_user[9]) == 0) attron(A_BOLD | A_UNDERLINE);
    move(5,0);
    printw("|10  |                                        |     |    |   |\n");
    q++;
    mvprintw((q-4),7,"%-25s",top_user[q]);
    mvprintw((q-4),47,"%-5d",top_point[q]); mvprintw((q-4),53,"%-4d",top_gold[q]);
    mvprintw((q-4),58,"%-3d",top_fg[q]);
    if (strcmp(name,top_user[9]) == 0) attroff(A_BOLD | A_UNDERLINE);

    attron(COLOR_PAIR(4));
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
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    attron(COLOR_PAIR(1));
    const char *menu[] = {"New Game", "Load Game","Settings","Scoreboard","Profile","Back"};
    int choice = 0;
    while (1)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(4 + i, 3, "%s", menu[i]);
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP)
            choice = (choice == 0) ? 5 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 5) ? 0 : choice + 1;
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
        if (player.can_load == 1) {
            print_map_loading();
            move_and_message();
        }
        else {
            game_menu_user(player.username);
        }
        break;
    case 2:
        setting(name);
        break;
    case 3:
        scoreboard1(name);
        break;
    case 4:
        profile_menu(name);
        break;
    case 5:
        main_menu();
        break;
    }
    echo();
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
}

void pass_gen() {
    int len = random_number(8,12); int a=0, a1=0,a2=0,a3=0;
    for (int i = 0; i < len; i++)
    {
        a = random_number(1,3);
        switch (a)
        {
        case 1:
            a1 = random_number(48,57);
            string[i] = (char)(a1);
            break;
        case 2:
            a2 = random_number(65,90);
            string[i] = (char)(a2);
            break;
        case 3:
            a3 = random_number(97,122);
            string[i] = (char)(a3);
            break;
        }
    }
    string[len] = '\0';
}

void hero_color(char name[30]) {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
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
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
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
        max_health = 150;
        break;
    case 1:
        player.health = 100;
        break;
    case 2:
        player.health = 50;
        max_health = 50;
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
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    attron(COLOR_PAIR(1));
    const char *menu[] = {"Difficulty", "Color of Hero", "Music", "Back"};
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
    switch (choice)
    {
    case 0:
        difficulty(name);
        break;
    case 1:
        hero_color(name);
        break;
    case 2:
        music(name);
        break;
    case 3:
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
        printw("Press x to see your password!\n");
        char x = getch();
        if (x == 'x')
        {
            printw("[%s]\n",pass2);
        }
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
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
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
    
    FILE *gold = fopen("gold.txt", "a+");
    FILE *point = fopen("points.txt", "a+");
    FILE *fg = fopen("finished_game_number.txt", "a+");
    fprintf(gold,"0\n");
    fprintf(point,"0\n");
    fprintf(fg,"0\n");
    fclose(gold); fclose(point); fclose(fg);
    
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
        if (!correct_password(password) || strlen(password) < 7) {
            pass_gen();
            printw("Here is a suggested password which were created randomly: %s\n",string);
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

int main_menu() {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    init_pair(2,COLOR_WHITE,COLOR_BLACK);
    attron(COLOR_PAIR(2));
    mvprintw(1,20,"__        __   _                               _             ____                        ");
    mvprintw(2,20,"\\ \\      / /__| | ___ ___  _ __ ___   ___     | |_ ___      |  _ \\ ___   __ _ _   _  ___ ");
    mvprintw(3,20," \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\    | __/ _ \\     | |_) / _ \\ / _` | | | |/ _ \\");
    mvprintw(4,20,"  \\ V  V /  __/ | (_| (_) | | | | | |  __/    | || (_) |    |  _ < (_) | (_| | |_| |  __/");
    mvprintw(5,20,"   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|     \\__\\___/     |_| \\_\\___/ \\__, |\\__,_|\\___|");
    mvprintw(6,20,"                                                                        |___/            ");
    attron(COLOR_PAIR(1));
    const char *menu[] = {"New User", "Login","Exit"};
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
        new_user();
        break;
    case 1:
        login_menu();
        break;
    case 2:
        Mix_CloseAudio();
        SDL_Quit();
        endwin();
        exit(0);
        return 0;
        break;
    }
    echo();
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
}

int initSDL() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        return 0;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        return 0;
    }
    return 1;
}

Mix_Music* playmusic(const char* music_file) {
    Mix_Music* start_music = Mix_LoadMUS(music_file);
    if (start_music == NULL) {
        return NULL;
    }
    Mix_PlayMusic(start_music, 0);
    return start_music;
}

int music(char name[30]) {
    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    attron(COLOR_PAIR(1));
    if (!initSDL()) return -1;
    const char *menu[] = {"   Hedwig's Theme      -         John Williams         ",
                          "     Sur le fil        -          Yann Tiersen         ", 
                          " Mission Impossible    -   Adam Clayton & Larry Mullen ",
                          "                  Turn off the music                   "};
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
    switch (choice)
    {
    case 0:
        current_music = 0;
        break;
    case 1:
        current_music = 1;
        break;
    case 2:
        current_music = 2;
        break;
    case 3:
        Mix_HaltMusic();
        Mix_CloseAudio();
        SDL_Quit();
        setting(name);
        return 0;
        break;
    }
    playmusic(musics[current_music]);
    
    echo();
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
    setting(name);
}

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
    case '&':
        player.damage_food++;
        return 1;
        break;
    case '%':
        player.speed_food++;
        return 1;
        break;
    case '=':
        return 1;
        break;
    case '*':
        player.normal_arrow++;
        return 1;
        break;
    case '!':
        if (boss.live == 1)
        {
            mvprintw(0,1,"The 'Boss' is still alive!                                            ");
        }
        else return 1;
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

    if (ch1 == 'B' || ch2 == 'B' || ch3 == 'B' || ch4 == 'B' || ch5 == 'B' || ch6 == 'B' || ch7 == 'B' || ch8 == 'B')
        return 6;
    
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

void move_character_2x(int i) {
    switch (i) {
        case KEY_HOME: // 7
            if (valid_move(location.x - 2, location.y - 2)) {
                location.y--;
                location.x--;
                location.y--;
                location.x--;
            }
            else if (valid_move(location.x - 1, location.y - 1)) {
                location.y--;
                location.x--;
            }
            break;
        case KEY_UP: // 8
            if (valid_move(location.x, location.y - 2)) {
                location.y--; location.y--;
            }
            else if (valid_move(location.x, location.y - 1)) {
                location.y--;
            }
            break;
        case KEY_PPAGE: // 9
            if (valid_move(location.x + 2, location.y - 2)) {
                location.y--;
                location.x++;
                location.y--;
                location.x++;
            }
            else if (valid_move(location.x + 1, location.y - 1)) {
                location.y--;
                location.x++;
            }
            break;
        case KEY_LEFT: // 4
            if (valid_move(location.x - 2, location.y)) {
                location.x--; location.x--; 
            }
            else if (valid_move(location.x - 1, location.y)) {
                location.x--;
            }
            break;
        case KEY_RIGHT: // 6
            if (valid_move(location.x + 2, location.y)) {
                location.x++; location.x++;
            }
            else if(valid_move(location.x + 1, location.y)) {
                location.x++;
            }
            break;
        case KEY_END: // 1
            if (valid_move(location.x - 2, location.y + 2)) {
                location.y++;
                location.x--;
                location.y++;
                location.x--;
            }
            else if (valid_move(location.x - 1, location.y + 1)) {
                location.y++;
                location.x--;
            }
            break;
        case KEY_DOWN: // 2
            if (valid_move(location.x, location.y + 2)) {
                location.y++; location.y++;
            }
            else if (valid_move(location.x, location.y + 1)) {
                location.y++;
            }
            break;
        case KEY_NPAGE: // 3
            if (valid_move(location.x + 2, location.y + 2)) {
                location.y++;
                location.x++;
                location.y++;
                location.x++;
            }
            else if (valid_move(location.x + 1, location.y + 1)) {
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
    mvprintw(0,1,"Welcome to floor %d!                                                  ",tabaghe);
    mvprintw(1,1,"Gold: %d ",player.gold);
    mvprintw(1,10,"Point: %d ",player.point);
    mvprintw(1,21,"Health: %d ",player.health);
    mvprintw(1,33,"Energy: %d ",player.energy);
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
    int panjare = 0;

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
                        int frand = random_number(1,3);
                        switch (frand)
                        {
                        case 1:
                            addch('f');
                            break;
                        case 2:
                            addch('&');
                            break;
                        case 3:
                            addch('%');
                            break;
                        }
                        attroff(COLOR_PAIR(7));
                    }
                    else if (tabaghe == 4 && current_room == ganj_room && (i == enemyy) && (j == enemyx)) {
                        attron(COLOR_PAIR(4));
                        if ((i == (arz-1)) && (j == (tool-2))) {
                            mvaddch(i-2,j-2,'B');
                            move(i,j);
                            boss.x = x+j-2; boss.y = y+i-2;
                        }
                        else {
                            addch('B');
                            boss.x = x + j; boss.y = y + i;
                        }
                        
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
                panjare = random_number(1,tool);
                if (panjare == door2) panjare--;
                
                pos[15][0] = (y + door1); pos[15][1] = (x + tool+1);
                pos[16][0] = (y + arz + 1); pos[16][1] = (x + door2);
                window[1][0] = (y + arz + 1); window[1][1] = (x + panjare);
                room1.xs = x; room1.xf = (x+tool+1); room1.ys = y; room1.yf = (y+arz+1); room1.visited = 0;
            }
            if (k == 0 && l == 1) {
                door1 = random_number(1,arz); door2 = random_number(1,tool);
                pos[3][0] = (y + door1); pos[3][1] = (x + tool+1);
                pos[4][0] = (y); pos[4][1] = (x + door2);
                room2.xs = x; room2.xf = (x+tool+1); room2.ys = y; room2.yf = (y+arz+1); room2.visited = 0;
            }
            if (k == 1 && l == 0) {
                door1 = random_number(1,arz); door2 = random_number(1,arz); door3 = random_number(1,tool);
                panjare = random_number(1,arz);
                if (panjare == door2) panjare--;
                pos[5][0] = (y + door1); pos[5][1] = (x);
                pos[6][0] = (y + door2); pos[6][1] = (x + tool + 1);
                pos[13][0] = (y + arz + 1); pos[13][1] = (x + door3);
                window[3][0] = (y + panjare); window[3][1] = (x + tool + 1);
                room3.xs = x; room3.xf = (x+tool+1); room3.ys = y; room3.yf = (y+arz+1); room3.visited = 0;
            }
            if (k == 1 && l == 1) {
                door1 = random_number(1,arz); door2 = random_number(1,arz); door3 = random_number(1,tool);
                panjare = random_number(1,tool);
                if (panjare == door3) panjare--;
                pos[7][0] = (y + door1); pos[7][1] = (x);
                pos[8][0] = (y + door2); pos[8][1] = (x + tool + 1);
                pos[14][0] = (y); pos[14][1] = (x + door3);
                window[2][0] = (y); window[2][1] = (x + panjare);
                room4.xs = x; room4.xf = (x+tool+1); room4.ys = y; room4.yf = (y+arz+1); room4.visited = 0;
            }
            if (k == 2 && l == 0) {
                door1 = random_number(1,arz); door2 = random_number(1,tool);
                pos[9][0] = (y + door1); pos[9][1] = (x);
                pos[10][0] = (y + arz + 1); pos[10][1] = (x + door2);
                room5.xs = x; room5.xf = (x+tool+1); room5.ys = y; room5.yf = (y+arz+1); room5.visited = 0;
            }
            if (k == 2 && l == 1) {
                door1 = random_number(1,arz); door2 = random_number(1,tool);
                panjare = random_number(1,tool);
                if (panjare == door2) panjare--;
                pos[11][0] = (y + door1); pos[11][1] = (x);
                pos[12][0] = (y); pos[12][1] = (x + door2);
                window[4][0] = (y); window[4][1] = (x + panjare);
                room6.xs = x; room6.xf = (x+tool+1); room6.ys = y; room6.yf = (y+arz+1); room6.visited = 1;
            }
            attron(COLOR_PAIR(1));
            current_room++;
        }
    }
    location.x = x+1; location.y = y+1;
    for (int i = 3; i <= 16; i++)
    {
        mvaddch(pos[i][0],pos[i][1],'+');
    }
    for (int i = 1; i <= 4; i++)
    {
        mvaddch(window[i][0],window[i][1],'=');
    }
    
    if (pos[15][0] > pos[5][0]) hallway_ru(pos[15][0],pos[15][1],pos[5][0],pos[5][1]);
    else hallway_rd(pos[15][0],pos[15][1],pos[5][0],pos[5][1]);

    if (pos[6][0] > pos[9][0]) hallway_ru(pos[6][0],pos[6][1],pos[9][0],pos[9][1]);
    else hallway_rd(pos[6][0],pos[6][1],pos[9][0],pos[9][1]);

    if (pos[3][0] > pos[7][0]) hallway_ru(pos[3][0],pos[3][1],pos[7][0],pos[7][1]);
    else hallway_rd(pos[3][0],pos[3][1],pos[7][0],pos[7][1]);

    if (pos[8][0] > pos[11][0]) hallway_ru(pos[8][0],pos[8][1],pos[11][0],pos[11][1]);
    else hallway_rd(pos[8][0],pos[8][1],pos[11][0],pos[11][1]);

    if (pos[16][1] > pos[4][1]) hallway_ur(pos[4][0],pos[4][1],pos[16][0],pos[16][1]);
    else hallway_ul(pos[4][0],pos[4][1],pos[16][0],pos[16][1]);

    if (pos[10][1] > pos[12][1]) hallway_ur(pos[12][0],pos[12][1],pos[10][0],pos[10][1]);
    else hallway_ul(pos[12][0],pos[12][1],pos[10][0],pos[10][1]);

    if (pos[13][1] > pos[14][1]) hallway_ur(pos[14][0],pos[14][1],pos[13][0],pos[13][1]);
    else hallway_ul(pos[14][0],pos[14][1],pos[13][0],pos[13][1]);


    attroff(COLOR_PAIR(1));
    save_map();
    print_room6();
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

        if (ch == 'q') {
            if (strcmp(player.username,"guest") == 0)
            {
                main_menu();
            }
            else {
                player.can_load = 1;
                save_map_before_quit();
                napms(3000);
                game_menu_user(player.username);
                endwin();
            }
        }

        else if (ch == ' ') {
            if (player.pd_on == 0) {
            if (strcmp(player.current_weapon,"Mace") == 0)
            {
                switch (enemy_that_were_close_hit(location.x,location.y))
                {
                case 'D':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Deamon'!                                                    ");
                    deamon.health -= 5;
                    if (deamon.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Deamon'!                                              ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        deamon.woke = 0;
                    }
                    break;
                case 'F':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'FBM'!                                                      ");
                    fire.health -= 5;
                    if (fire.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'FBM'!                                                ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        fire.woke = 0;
                    }
                    break;
                case 'G':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Giant'!                                                    ");
                    giant.health -= 5;
                    if (giant.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Giant'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        giant.woke = 0;
                    }
                    break;
                case 'M':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Snake'!                                                    ");
                    snake.health -= 5;
                    if (snake.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Snake'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        snake.woke = 0;
                    }
                    break;
                case 'U':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Undeed'!                                                   ");
                    undeed.health -= 5;
                    if (undeed.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Undeed'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        undeed.woke = 0;
                    }
                    break;
                case 'B':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Boss'!                                                    ");
                    boss.health -= 5;
                    if (boss.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Boss'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        boss.woke = 0; boss.live = 0;
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
                case 'B':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Boss'!                                                    ");
                    boss.health -= 10;
                    if (boss.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Boss'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        boss.woke = 0; boss.live = 0;
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
                    mvprintw(0,1,"You don't have any 'Wand'!                                        ");
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
                    mvprintw(0,1,"You don't have any 'Dagger'!                                         ");
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

            if (player.pd_on == 1) {
            if (strcmp(player.current_weapon,"Mace") == 0)
            {
                switch (enemy_that_were_close_hit(location.x,location.y))
                {
                case 'D':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Deamon'!                                                    ");
                    deamon.health -= 10;
                    if (deamon.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Deamon'!                                              ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        deamon.woke = 0;
                    }
                    break;
                case 'F':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'FBM'!                                                      ");
                    fire.health -= 10;
                    if (fire.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'FBM'!                                                ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        fire.woke = 0;
                    }
                    break;
                case 'G':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Giant'!                                                    ");
                    giant.health -= 10;
                    if (giant.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Giant'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        giant.woke = 0;
                    }
                    break;
                case 'M':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Snake'!                                                    ");
                    snake.health -= 10;
                    if (snake.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Snake'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        snake.woke = 0;
                    }
                    break;
                case 'U':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Undeed'!                                                   ");
                    undeed.health -= 10;
                    if (undeed.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Undeed'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        undeed.woke = 0;
                    }
                    break;
                case 'B':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Boss'!                                                    ");
                    boss.health -= 10;
                    if (boss.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Boss'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        boss.woke = 0; boss.live = 0;
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
                    deamon.health -= 20;
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
                    fire.health -= 20;
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
                    giant.health -= 20;
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
                    snake.health -= 20;
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
                    undeed.health -= 20;
                    if (undeed.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Undeed'!                                            ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        undeed.woke = 0;
                    }
                    break;
                case 'B':
                    attron(COLOR_PAIR(6));
                    mvprintw(0,1,"You hit 'Boss'!                                                    ");
                    boss.health -= 10;
                    if (boss.health <= 0)
                    {
                        mvprintw(0,1,"You killed 'Boss'!                                             ");
                        attron(COLOR_PAIR(1));
                        mvaddch(last_last_cell.y,last_last_cell.x,'.');
                        boss.woke = 0; boss.live = 0;
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
                    enemy_hit_long_range(location.x,location.y,1,5,10);
                    break;
                case KEY_RIGHT:
                    enemy_hit_long_range(location.x,location.y,2,5,10);
                    break;
                case KEY_DOWN:
                    enemy_hit_long_range(location.x,location.y,3,5,10);
                    break;
                case KEY_LEFT:
                    enemy_hit_long_range(location.x,location.y,4,5,10);
                    break;
                }
            }
            if (strcmp(player.current_weapon,"Wand") == 0)
            {
                direction = getch();
                if (player.magic_wand == 0)
                {
                    mvprintw(0,1,"You don't have any 'Wand'!                                        ");
                    continue;
                }
                player.magic_wand--;
                switch (direction)
                {
                case KEY_UP:
                    enemy_hit_long_range(location.x,location.y,1,10,30);
                    break;
                case KEY_RIGHT:
                    enemy_hit_long_range(location.x,location.y,2,10,30);
                    break;
                case KEY_DOWN:
                    enemy_hit_long_range(location.x,location.y,3,10,30);
                    break;
                case KEY_LEFT:
                    enemy_hit_long_range(location.x,location.y,4,10,30);
                    break;
                }
            }
            if (strcmp(player.current_weapon,"Dagger") == 0)
            {
                direction = getch();
                if (player.dagger == 0)
                {
                    mvprintw(0,1,"You don't have any 'Dagger'!                                         ");
                    continue;
                }
                player.dagger--;
                switch (direction)
                {
                case KEY_UP:
                    enemy_hit_long_range(location.x,location.y,1,5,24);
                    break;
                case KEY_RIGHT:
                    enemy_hit_long_range(location.x,location.y,2,5,24);
                    break;
                case KEY_DOWN:
                    enemy_hit_long_range(location.x,location.y,3,5,24);
                    break;
                case KEY_LEFT:
                    enemy_hit_long_range(location.x,location.y,4,5,24);
                    break;
                }
            }
            }
        }

        else if (ch == 'r') {
            for (int i = 0; i < 28; i++) {
                for (int j = 0; j < 110; j++) {
                    played_map[i][j] = mvinch(i,j);
                }
            }
            player.x = location.x; player.y = location.y;
            for (int i = 0; i < 28; i++) {
                for (int j = 0; j < 110; j++) {
                    if (last_map_simple[i][j] == '#')
                    {
                        mvaddch(i,j,last_map[i][j]);
                    }
                }
            }
            if (room1.visited == 0)
            {
                print_room1();
                room1.visited = 1;
            }
            if (room2.visited == 0)
            {
                print_room2();
                room2.visited = 1;
            }
            if (room3.visited == 0)
            {
                print_room3();
                room3.visited = 1;
            }
            if (room4.visited == 0)
            {
                print_room4();
                room4.visited = 1;
            }
            if (room5.visited == 0)
            {
                print_room5();
                room5.visited = 1;
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
                mvprintw(0,1,"You ignored the 'normal gold'!                                        ");
                player.gold--; player.point-=10;
                mvprintw(1,1,"Gold: %d ",player.gold);
                mvprintw(1,10,"Point: %d ",player.point);
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case 'g':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'black gold'!                                         ");
                player.gold--; player.point-=50;
                mvprintw(1,1,"Gold: %d ",player.gold);
                mvprintw(1,10,"Point: %d ",player.point);
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case '^':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You were Trapped! You lost 5 points!                                  ");
                mvprintw(1,21,"Health: %d ",player.health);
                check_game_over();
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case 's':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Speed Potion'!                                       ");
                player.potion_speed--;
                break;
            case 'h':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Health Potion'!                                      ");
                player.potion_health--;
                break;
            case 'd':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Damage Potion'!                                      ");
                player.potion_damage--;
                break;
            case 'A':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Normal Arrow'!                                      ");
                player.normal_arrow--;
                break;
            case 'S':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Sword'!                                            ");
                if (player.sword == 1) player.sword = 0;
                break;
            case 'W':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Magic Wand'!                                        ");
                player.magic_wand--;
                break;
            case 'K':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Dagger'!                                            ");
                player.dagger--;
                break;
            case 'f':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Food'!                                              ");
                player.food--;
                break;
            case '%':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Speed Food'!                                        ");
                player.food--;
                break;
            case '&':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You ignored the 'Power Food'!                                        ");
                player.food--;
                break;
            case '+':
                print_hallway();
                print_rooms();
                break;
            case '#':
                print_hallway();
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
                    mvprintw(0,1,"You've got a normal gold with 10 points!                                ");
                    mvprintw(1,1,"Gold: %d ",player.gold);
                    mvprintw(1,10,"Point: %d ",player.point);
                    attroff(COLOR_PAIR(1));
                    attron(COLOR_PAIR(player.color));
                    last_cell.s = '.';
                    break;
                case 'g':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You've got a black gold with 50 points!                                 ");
                    mvprintw(1,1,"Gold: %d ",player.gold);
                    mvprintw(1,10,"Point: %d ",player.point);
                    attroff(COLOR_PAIR(1));
                    attron(COLOR_PAIR(player.color));
                    last_cell.s = '.';
                    break;
                case '^':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You were Trapped! You lost 5 points!                                    ");
                    mvprintw(1,21,"Health: %d ",player.health);
                    check_game_over();
                    attroff(COLOR_PAIR(1));
                    attron(COLOR_PAIR(player.color));
                    break;
                case 's':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Speed Potion' to your backpack!                                  ");
                    last_cell.s = '.';
                    break;
                case 'h':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Health Potion' to your backpack!                             ");
                    last_cell.s = '.';
                    break;
                case 'd':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Damage Potion' to your backpack!                             ");
                    last_cell.s = '.';
                    break;
                case 'A':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Normal Arrow' to your backpack!                              ");
                    last_cell.s = '.';
                    break;
                case 'S':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 'Sword' to your backpack!                                       ");
                    last_cell.s = '.';
                    break;
                case 'W':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Magic Wand' to your backpack!                                ");
                    last_cell.s = '.';
                    break;
                case 'K':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Dagger' to your backpack!                                    ");
                    last_cell.s = '.';
                    break;
                case 'f':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Food' to your backpack!                                      ");
                    last_cell.s = '.';
                    break;
                case '%':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Speed Food' to your backpack!                              ");
                    sfood_change = 0;
                    last_cell.s = '.';
                    break;
                case '&':
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You add 1 'Power Food' to your backpack!                              ");
                    dfood_change = 0;
                    last_cell.s = '.';
                    break;
                case '+':
                    print_hallway();
                    print_rooms();
                    break;
                case '#':
                    print_hallway();
                    break;
                case '!':
                    attron(COLOR_PAIR(2));
                    mvprintw(0,1,"Congratulations!! You've finished Rouge successfully!!                  ");
                    save_game(1);
                    refresh();
                    napms(5000);
                    refresh();
                    clear();
                    main_menu();
                    break;
                }
                last_cell.y = location.y; last_cell.x = location.x;
                attron(COLOR_PAIR(player.color));
                mvaddch(location.y,location.x,'@');
                }
        }
        
        else if (ch == 'e') {
            attron(COLOR_PAIR(2));
            clean_right_side();
            mvprintw(0,87,"Food Menu");
            mvprintw(4,83,"Normal Food(f):    %d   ",player.food);
            mvprintw(5,83,"Power Food(p):     %d   ",player.damage_food);
            mvprintw(6,83,"Speed Food(s):     %d   ",player.speed_food);
            mvprintw(8,81,"press one of these {f,p,s}");

            char com; com = getch();

            if (com == 'f') {
                attron(COLOR_PAIR(1));
                if (player.food == 0)
                {
                    mvprintw(0,1,"You don't have any Normal Food!                                          ");
                    continue;
                }
                player.food--;
                player.energy = 5; player.health += 3;
                attron(COLOR_PAIR(1));
                mvprintw(1,33,"Energy: %d ",player.energy);
                mvprintw(1,21,"Health: %d ",player.health);
                mvprintw(0,1,"You ate a Normal Food and your energy was refreshed!                      ");
                attron(COLOR_PAIR(2));
                mvprintw(4,83,"Normal Food(f):    %d   ",player.food);
            }

            if (com == 's') {
                attron(COLOR_PAIR(1));
                if (player.speed_food == 0)
                {
                    mvprintw(0,1,"You don't have any Speed Food!                                          ");
                    continue;
                }
                mvprintw(0,1,"You ate a Speed Food and you can walk fast!                           ");
                player.speed_food--;
                count_spotion = 0; player.ps_on = 1;
                player.energy = 5; player.health += 3;
                mvprintw(1,33,"Energy: %d ",player.energy);
                mvprintw(1,21,"Health: %d ",player.health);
                attron(COLOR_PAIR(2));
                mvprintw(6,83,"Speed Food(s):     %d   ",player.speed_food);
            }

            if (com == 'p') {
                attron(COLOR_PAIR(1));
                if (player.damage_food == 0)
                {
                    mvprintw(0,1,"You don't have any Power Food!                                          ");
                    continue;
                }
                mvprintw(0,1,"You ate a Power Food! weapon's power has been boosted!                ");
                player.damage_food--;
                count_dpotion = 0; player.pd_on = 1;
                player.energy = 5; player.health += 3;
                mvprintw(1,33,"Energy: %d ",player.energy);
                mvprintw(1,21,"Health: %d ",player.health);
                attron(COLOR_PAIR(2));
                mvprintw(5,83,"Power Food(p):     %d   ",player.damage_food);
            }
        }

        else if (ch == 'p') {
            attron(COLOR_PAIR(2));
            clean_right_side();
            mvprintw(0,87,"Potion Menu");
            mvprintw(4,83,"Health Potion(h):    %d   ",player.potion_health);
            mvprintw(5,83,"Speed Potion(s):     %d   ",player.potion_speed);
            mvprintw(6,83,"Damage Potion(d):    %d   ",player.potion_damage);
            mvprintw(8,81,"press one of these {h,s,d}");

            char com; com = getch();

            if (com == 'h') {
                attron(COLOR_PAIR(1));
                if (player.potion_health == 0)
                {
                    mvprintw(0,1,"You don't have any Health Potion!                                          ");

                    continue;
                }
                mvprintw(0,1,"You used Health Potion!                                                ");
                attron(COLOR_PAIR(2));
                player.potion_health--;
                mvprintw(4,83,"Health Potion(h):    %d   ",player.potion_health);
                count_hpotion = 0;
                player.ph_on = 1;
            }

            if (com == 's') {
                attron(COLOR_PAIR(1));
                if (player.potion_speed == 0)
                {
                    mvprintw(0,1,"You don't have any Speed Potion!                                          ");
                    continue;
                }
                mvprintw(0,1,"You used Speed Potion!                                                ");
                attron(COLOR_PAIR(2));
                player.potion_speed--;
                mvprintw(5,83,"Speed Potion(s):     %d   ",player.potion_speed);
                count_spotion = 0;
                player.ps_on = 1;
            }

            if (com == 'd') {
                attron(COLOR_PAIR(1));
                if (player.potion_damage == 0)
                {
                    mvprintw(0,1,"You don't have any Damage Potion!                                          ");
                    continue;
                }
                mvprintw(0,1,"You used Damage Potion!                                                ");
                attron(COLOR_PAIR(2));
                player.potion_damage--;
                mvprintw(6,83,"Damage Potion(d):    %d   ",player.potion_damage);
                count_dpotion = 0;
                player.pd_on = 1;
            }
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
                    mvprintw(0,1,"You put your 'Mace' in your backpack!                                 ");
                }
                else if (strcmp(player.current_weapon,"Dagger") == 0)
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You put a 'Dagger' in your backpack!                                 ");
                    player.dagger++;
                    attron(COLOR_PAIR(2));
                    mvprintw(11,80," Dagger(K):   12   5    %d   ",player.dagger);
                }
                else if (strcmp(player.current_weapon,"Sword") == 0)
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You put your 'Sword' in your backpack!                               ");
                    attron(COLOR_PAIR(2));
                    mvprintw(8,80," Sword(S):    10   1    Yes   ");
                }
                else if (strcmp(player.current_weapon,"Arrow") == 0)
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You put an 'Arrow' in your backpack!                                ");
                    player.normal_arrow++;
                    attron(COLOR_PAIR(2));
                    mvprintw(13,80," Arrow(A):    5    5    %d   ",player.normal_arrow);
                }
                else if (strcmp(player.current_weapon,"Wand") == 0)
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You put a 'Wand' in your backpack!                                  ");
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
                            mvprintw(0,1,"You took your 'Mace'!                                                 ");
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
                                mvprintw(0,1,"You took your 'Sword'!                                                ");
                                strcpy(player.current_weapon,"Sword");
                                valid = 0;
                                attron(COLOR_PAIR(7));
                                mvprintw(8,80," Sword(S):    10   1    Yes   ");
                                attron(COLOR_PAIR(2));
                                mvprintw(16,80," Sword                        ");
                            }
                            if (player.sword == 0) {
                                mvprintw(0,1,"You don't have 'Sword'!  pick another!                                ");
                            }
                            break;
                        case 'k':
                            attron(COLOR_PAIR(1));
                            if (player.dagger == 0)
                            {
                                mvprintw(0,1,"You don't have any 'Dagger'!  pick another!                           ");
                            }
                            else {
                                mvprintw(0,1,"You took a 'Dagger'!                                                  ");
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
                                mvprintw(0,1,"You don't have any 'Arrow'!  pick another!                           ");
                            }
                            else {
                                mvprintw(0,1,"You took an 'Arrow'!                                                 ");
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
                                mvprintw(0,1,"You don't have any 'Wand'!  pick another!                             ");
                            }
                            else {
                                mvprintw(0,1,"You took a 'Wand'!                                                    ");
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

            if (player.ps_on == 1) move_character_2x(ch);
            if (player.ps_on == 0) move_character(ch);

            attroff(COLOR_PAIR(player.color));
            attron(COLOR_PAIR(1));

            mvaddch(last_cell.y,last_cell.x,last_cell.s);
            mvaddch(last_last_cell.y,last_last_cell.x,last_last_cell.s);

            attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(player.color));

            last_cell.s = mvinch(location.y,location.x) & A_CHARTEXT;
            last_last_cell.s = mvinch(enemy_location.y,enemy_location.x) & A_CHARTEXT;

            



            switch (last_cell.s)
            {
            case '$':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You've got a normal gold with 10 points!                             ");
                mvprintw(1,1,"Gold: %d ",player.gold);
                mvprintw(1,10,"Point: %d ",player.point);
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                last_cell.s = '.';
                break;
            case 'g':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You've got a black gold with 50 points!                              ");
                mvprintw(1,1,"Gold: %d ",player.gold);
                mvprintw(1,10,"Point: %d ",player.point);
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                last_cell.s = '.';
                break;
            case '^':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You were Trapped! You lost 5 points!                                  ");
                mvprintw(1,21,"Health: %d ",player.health);
                check_game_over();
                attroff(COLOR_PAIR(1));
                attron(COLOR_PAIR(player.color));
                break;
            case 's':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Speed Potion' to your backpack!                             ");
                last_cell.s = '.';
                break;
            case 'h':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Health Potion' to your backpack!                             ");
                last_cell.s = '.';
                break;
            case 'd':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Damage Potion' to your backpack!                             ");
                last_cell.s = '.';
                break;
            case 'A':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Normal Arrow' to your backpack!                              ");
                last_cell.s = '.';
                break;
            case 'S':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 'Sword' to your backpack!                                       ");
                last_cell.s = '.';
                break;
            case 'W':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Magic Wand' to your backpack!                                ");
                last_cell.s = '.';
                break;
            case 'K':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Dagger' to your backpack!                                    ");
                last_cell.s = '.';
                break;
            case 'f':
                attron(COLOR_PAIR(1));
                int currupt = random_number(1,3);
                if (currupt == 3)
                {
                    mvprintw(0,1,"You ate a 'Currupted Food' and lost 5 health!                           ");
                    player.food--;
                    player.health-=5;
                    mvprintw(1,21,"Health: %d ",player.health);
                    check_game_over();
                }
                else {
                    mvprintw(0,1,"You add 1 'Food' to your backpack!                                      ");
                    food_change = 0;
                }
                last_cell.s = '.';
                break;
            case '&':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Power Food' to your backpack!                              ");
                dfood_change = 0;
                last_cell.s = '.';
                break;
            case '%':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You add 1 'Speed Food' to your backpack!                              ");
                sfood_change = 0;
                last_cell.s = '.';
                break;
            case '*':
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"You took a 'wasted Arrow'!                                            ");
                last_cell.s = '.';
                break;
            case '+':
                print_hallway();
                print_rooms();
                chtype wall1 = mvinch(location.y+1,location.x);
                chtype wall2 = mvinch(location.y,location.x+1);
                chtype wall3 = mvinch(location.y-1,location.x);
                chtype wall4 = mvinch(location.y,location.x-1);
                if ( (PAIR_NUMBER(wall1 & A_COLOR) == 3 && ((wall1 & A_CHARTEXT) == '_' || (wall1 & A_CHARTEXT) == '|')) ||
                (PAIR_NUMBER(wall2 & A_COLOR) == 3 && ((wall2 & A_CHARTEXT) == '_' || (wall2 & A_CHARTEXT) == '|')) ||
                (PAIR_NUMBER(wall3 & A_COLOR) == 3 && ((wall3 & A_CHARTEXT) == '_' || (wall3 & A_CHARTEXT) == '|')) ||
                (PAIR_NUMBER(wall4 & A_COLOR) == 3 && ((wall4 & A_CHARTEXT) == '_' || (wall4 & A_CHARTEXT) == '|') ) )
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You are at the entry of a 'Potion' room!                              ");
                    if (current_music != 0) {
                        playmusic(musics[0]);
                        current_music = 0;
                    }
                }
                else if ( (PAIR_NUMBER(wall1 & A_COLOR) == 4 && ((wall1 & A_CHARTEXT) == '_' || (wall1 & A_CHARTEXT) == '|')) ||
                (PAIR_NUMBER(wall2 & A_COLOR) == 4 && ((wall2 & A_CHARTEXT) == '_' || (wall2 & A_CHARTEXT) == '|')) ||
                (PAIR_NUMBER(wall3 & A_COLOR) == 4 && ((wall3 & A_CHARTEXT) == '_' || (wall3 & A_CHARTEXT) == '|')) ||
                (PAIR_NUMBER(wall4 & A_COLOR) == 4 && ((wall4 & A_CHARTEXT) == '_' || (wall4 & A_CHARTEXT) == '|')))
                {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You are at the entry of the 'Treasure' room!                          ");
                    if (current_music != 2) {
                        playmusic(musics[2]);
                        current_music = 2;
                    }
                }
                else {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You are at the entry of a normal room!                                ");
                    if (current_music != 1) {
                        playmusic(musics[1]);
                        current_music = 1;
                    }
                }
                break;
            case '=':
                if (location.y == window[1][0] && location.x == window[1][1]) {
                    print_room2();
                    room2.visited = 1;
                }
                if (location.y == window[2][0] && location.x == window[2][1]) {
                    print_room3();
                    room3.visited = 1;
                }
                if (location.y == window[3][0] && location.x == window[3][1]) {
                    print_room5();
                    room5.visited = 1;
                }
                if (location.y == window[4][0] && location.x == window[4][1]) {
                    print_room5();
                    room5.visited = 1;
                }
                break;
            case '#':
                print_hallway();
                break;
            case '!':
                attron(COLOR_PAIR(2));
                mvprintw(0,1,"Congratulations!! You've finished Rouge successfully!!                  ");
                save_game(1);
                refresh();
                napms(5000);
                refresh();
                clear();
                main_menu();
                break;
            }

            if (deamon.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 1) {
                if (count_d == 6)
                {
                    count_d = 0;
                }
                else {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You woke up 'Deamon'!                                              ");
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
                    mvprintw(0,1,"You woke up 'Fire Breathing Monster'!                              ");
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
                mvprintw(0,1,"You woke up 'Giant'!                                                   ");
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
                mvprintw(0,1,"You woke up 'Snake'!                                                   ");
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
                mvprintw(0,1,"You woke up 'Undeed'!                                                   ");
                mvaddch(undeed.y,undeed.x,'.');
                undeed.woke = 1;
            }

            if (undeed.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'U');
                attroff(COLOR_PAIR(2));
            }

            if (boss.woke == 0 && wake_enemy(last_last_cell.x,last_last_cell.y) == 6) {
                    attron(COLOR_PAIR(1));
                    mvprintw(0,1,"You woke up 'Boss'!                                                   ");
                    mvaddch(boss.y,boss.x,'.');
                    boss.woke = 1;
            }

            if (boss.woke == 1) {
                attron(COLOR_PAIR(2));
                mvaddch(enemy_location.y,enemy_location.x,'B');
                attroff(COLOR_PAIR(2));
            }

            enemy_hit_us(location.y,location.x);

            last_last_cell.y = enemy_location.y; last_last_cell.x = enemy_location.x;
            enemy_location.y = last_cell.y; enemy_location.x = last_cell.x;
            last_cell.y = location.y; last_cell.x = location.x;
            
            attron(COLOR_PAIR(player.color));
            mvaddch(location.y,location.x,'@');


            count_hungry++; count_hpotion++; count_spotion++; count_dpotion++; food_change++; sfood_change++; dfood_change++;
            if (count_hungry == 10)
            {
                count_hungry = 0;
                player.energy--;
                attron(COLOR_PAIR(1));
                if (player.energy == 0)
                {
                    player.health -= 5;
                    player.energy = 5;
                    check_game_over();
                }
                mvprintw(1,21,"Health: %d ",player.health);
                mvprintw(1,33,"Energy: %d ",player.energy);
            }
            if (player.energy == 5 && (count_hungry==4 || count_hungry==8) )
            {
                player.health += 4;
                if (player.health > max_health)
                    player.health = max_health;
                attron(COLOR_PAIR(1));
                mvprintw(1,21,"Health: %d ",player.health);
                mvprintw(0,1,"You were healed!                                                              ");
            }
            if ((player.ph_on == 1) && (player.energy == 4 || player.energy == 5))
            {
                if (count_hpotion == 2 || count_hpotion == 4 || count_hpotion == 6 || count_hpotion == 8)
                {
                    player.health += 4;
                    if (player.health > max_health)
                        player.health = max_health;
                    attron(COLOR_PAIR(1));
                    mvprintw(1,21,"Health: %d ",player.health);
                    mvprintw(0,1,"You were healed by Health Potion                                          ");
                }
            }
            if (count_hpotion == 10) player.ph_on = 0; if (count_spotion == 10) player.ps_on = 0; if (count_dpotion == 10) player.pd_on = 0;

            if (food_change == 20 && player.food > 0)
            {
                food_change == 0; player.food--;
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"1 of your Normal Food was currupted!                                  ");
            }

            if (sfood_change == 20 && player.speed_food > 0)
            {
                sfood_change == 0; player.speed_food--; player.food++;
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"1 of your Speed Food became Normal Food!                              ");
            }

            if (dfood_change == 20 && player.damage_food > 0)
            {
                dfood_change == 0; player.damage_food--; player.food++;
                attron(COLOR_PAIR(1));
                mvprintw(0,1,"1 of your Damage Food became Normal Food!                              ");
            }
        }
    }
    attroff(COLOR_PAIR(player.color));
    clear();
    refresh();
}

int enemy_hit_long_range(int x, int y, int direction, int range, int hit) {
    switch (direction)
    {
    case 1:
        for (int i = 1; i < range; i++)
        {
            char ch = mvinch(y-i, x) & A_CHARTEXT;
            if (ch == 'D')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Deamon'!                                                      ");
                deamon.health -= hit;
                if (deamon.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Deamon'!                                               ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    deamon.woke = 0;
                }
                return 1;
                break;
            }
            if (ch == 'F') {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'FBM'!                                                          ");
                fire.health -= hit;
                if (fire.health <= 0)
                {
                    mvprintw(0,1,"You killed 'FMB'!                                                         ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    fire.woke = 0;
                }
                return 1;
                break;
            }
            if (ch == 'G')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Giant'!                                                        ");
                giant.health -= hit;
                if (giant.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Giant'!                                                     ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    giant.woke = 0;
                }
                return 1;
                break;
            }
            if (ch == 'U')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Undeed'!                                                       ");
                undeed.health -= hit;
                if (undeed.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Undeed'!                                                    ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    undeed.woke = 0;
                }
                return 1;
                break;
            }
            if (ch == 'M')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Snake'!                                                       ");
                snake.health -= hit;
                if (snake.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Snake'!                                                      ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    snake.woke = 0;
                }
                return 1;
                break;
            }
            if (ch == 'B')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Boss'!                                                       ");
                boss.health -= hit;
                if (boss.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Boss'! now you can finish the game and win!                 ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    boss.woke = 0; boss.live = 0;
                }
                return 1;
                break;
            }
            if (ch == 'O' || ch == '_' || ch == '|' || ch == '=' || ch == '+' || ch == '#')
            {
                mvaddch(y-i+1,x,'*');
                return 0;
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
                return 1;
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
                return 1;
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
                return 1;
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
                return 1;
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
                return 1;
                break;
            }
            if (ch == 'B')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Boss'!                                                       ");
                boss.health -= hit;
                if (boss.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Boss'! now you can finish the game and win!                 ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    boss.woke = 0; boss.live = 0;
                }
                return 1;
                break;
            }
            if (ch == 'O' || ch == '_' || ch == '|' || ch == '=' || ch == '+' || ch == '#')
            {
                mvaddch(y,x+i-1,'*');
                return 0;
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
                mvprintw(0,1,"You hit 'Deamon'!                                                  ");
                deamon.health -= hit;
                if (deamon.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Deamon'!                                            ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    deamon.woke = 0;
                }
                return 1;
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
                return 1;
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
                return 1;
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
                return 1;
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
                return 1;
                break;
            }
            if (ch == 'B')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Boss'!                                                       ");
                boss.health -= hit;
                if (boss.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Boss'! now you can finish the game and win!                 ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    boss.woke = 0; boss.live = 0;
                }
                return 1;
                break;
            }
            if (ch == 'O' || ch == '_' || ch == '|' || ch == '=' || ch == '+' || ch == '#')
            {
                mvaddch(y+i-1,x,'*');
                return 0;
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
                return 1;
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
                return 1;
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
                return 1;
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
                return 1;
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
                return 1;
                break;
            }
            if (ch == 'B')
            {
                attron(COLOR_PAIR(6));
                mvprintw(0,1,"You hit 'Boss'!                                                       ");
                boss.health -= hit;
                if (boss.health <= 0)
                {
                    mvprintw(0,1,"You killed 'Boss'! now you can finish the game and win!                 ");
                    attron(COLOR_PAIR(1));
                    mvaddch(last_last_cell.y,last_last_cell.x,'.');
                    boss.woke = 0; boss.live = 0;
                }
                return 1;
                break;
            }
            if (ch == 'O' || ch == '_' || ch == '|' || ch == '=' || ch == '+' || ch == '#')
            {
                mvaddch(y,x-i+1,'*');
                return 0;
                break;
            }
        }
        break;
    }
    switch (direction)
    {
    case 1:
        attron(COLOR_PAIR(5));
        mvaddch(y-range,x,'*');
        break;
    case 2:
        attron(COLOR_PAIR(5));
        mvaddch(y,x+range,'*');
        break;
    case 3:
        attron(COLOR_PAIR(5));
        mvaddch(y+range,x,'*');
        break;
    case 4:
        attron(COLOR_PAIR(5));
        mvaddch(y,x-range,'*');
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

    if (ch1 == 'B' || ch2 == 'B' || ch3 == 'B' || ch4 == 'B' || ch5 == 'B' || ch6 == 'B' || ch7 == 'B' || ch8 == 'B')
        return 'B';
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
        check_game_over();
    }
    if (ch1 == 'F' || ch2 == 'F' || ch3 == 'F' || ch4 == 'F' || ch5 == 'F') {
        player.health -= 2;
        attron(COLOR_PAIR(2));
        mvprintw(0,1,"You were hit by FBD! You lost 2 health!           ");
        attron(COLOR_PAIR(1));
        mvprintw(1,21,"Health: %d ",player.health);
        check_game_over();
    }
    if (ch1 == 'G' || ch2 == 'G' || ch3 == 'G' || ch4 == 'G' || ch5 == 'G') {
            player.health -= 3;
            attron(COLOR_PAIR(2));
            mvprintw(0,1,"You were hit by Giant! You lost 3 health!                 ");
            attron(COLOR_PAIR(1));
            mvprintw(1,21,"Health: %d ",player.health);
            check_game_over();
    }
    if (ch1 == 'M' || ch2 == 'M' || ch3 == 'M' || ch4 == 'M' || ch5 == 'M') {
        player.health -= 4;
        attron(COLOR_PAIR(2));
        mvprintw(0,1,"You were hit by Snake! You lost 4 health!                 ");
        attron(COLOR_PAIR(1));
        mvprintw(1,21,"Health: %d ",player.health);
        check_game_over();
    }
    if (ch1 == 'U' || ch2 == 'U' || ch3 == 'U' || ch4 == 'U' || ch5 == 'U') {
        player.health -= 5;
        attron(COLOR_PAIR(2));
        mvprintw(0,1,"You were hit by Unded! You lost 5 health!                  ");
        attron(COLOR_PAIR(1));
        mvprintw(1,21,"Health: %d ",player.health);
        check_game_over();
    }
    if (ch1 == 'B' || ch2 == 'B' || ch3 == 'B' || ch4 == 'B' || ch5 == 'B') {
        player.health -= 10;
        attron(COLOR_PAIR(2));
        mvprintw(0,1,"You were hit by Boss! You lost 10 health!                  ");
        attron(COLOR_PAIR(1));
        mvprintw(1,21,"Health: %d ",player.health);
        check_game_over();
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
    player.dagger = 0; player.potion_damage = 0; player.energy = 5;
    player.magic_wand = 0; player.normal_arrow = 0;
    player.sword = 0; player.potion_speed = 0; player.potion_health = 0; player.food = 0;
    player.ps_on = 0; player.pd_on = 0; player.ph_on = 0;
    player.x = 0; player.y = 0; player.floor = 1; player.can_load = 0;
    strcpy(player.current_weapon,"Mace");
}

void initialize_enemy() {
    deamon.woke = 0; snake.woke = 0; undeed.woke = 0; fire.woke = 0; giant.woke = 0; boss.woke = 0;
    deamon.health = 5; snake.health = 20; undeed.health = 30; fire.health = 10; giant.health = 15; boss.health = 50;
    deamon.x = 0; deamon.y = 0; snake.y = 0; snake.x = 0; boss.x = 0;
    undeed.x = 0; undeed.y = 0; fire.x = 0; fire.y = 0; giant.x = 0; giant.y = 0; boss.y = 0;
    boss.live = 1;
}

void check_game_over() {
    if (player.health <= 0) {
        mvprintw(1,21,"Health: %d ",0);
        mvprintw(0,1,"Game Over!!                                                        ");
        save_game(0);
        refresh();
        napms(5000);
        refresh();
        clear();
        endwin();
        main_menu();
    }
}

int save_game(int won) {
    if (strcmp(player.username,"guest") == 0) return 0;
    FILE* user = fopen("username.txt","r");
    FILE* gold = fopen("gold.txt","r");
    FILE* point = fopen("points.txt","r");
    FILE* fg = fopen("finished_game_number.txt","r");
    char u[30]; int line = 0; int x = 0;
    char g[10], f[4], p[10]; int g2=0, f2=0, p2=0;
    int g3[50], f3[50], p3[50];
    if (won == 1)
    {
        player.point += 50;
    }
    
    while (fgets(u,30,user) != NULL)
    {
        u[strlen(u)-1] = '\0';
        if (strcmp(u,player.username) == 0) break;
        line++;
    }
    while (fgets(g,8,gold) != NULL)
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
    while (fgets(p,8,point) != NULL)
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

void clean_map() {
    for (int i = 3; i <= 26; i++)
    {
        for (int j = 1; j <= 78; j++)
        {
            mvaddch(i,j,' ');
        }
    }
}

void save_map_before_quit() {
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 110; j++)
        {
            played_map[i][j] = mvinch(i,j);
        }
    }
    player.floor = tabaghe;
    player.x = location.x; player.y = location.y;
}

void save_map() {
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 110; j++)
        {
            last_map[i][j] = mvinch(i,j);
            last_map_simple[i][j] = (mvinch(i,j) & A_CHARTEXT);
        }
    }
    clean_map();
}

void print_map_loading() {
    location.x = player.x; location.y = player.y;
    tabaghe = player.floor;
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 110; j++)
        {
            mvaddch(i,j,played_map[i][j]);
        }
    }
}

void print_room1() {
    for (int i = room1.ys; i <= room1.yf; i++) {
        for (int j = room1.xs; j <= room1.xf; j++) {
            mvaddch(i,j,last_map[i][j]);
        }
    }
    room1.visited = 1;
}
void print_room2() {
    for (int i = room2.ys; i <= room2.yf; i++) {
        for (int j = room2.xs; j <= room2.xf; j++) {
            mvaddch(i,j,last_map[i][j]);
        }
    }
    room2.visited = 1;
}
void print_room3() {
    for (int i = room3.ys; i <= room3.yf; i++) {
        for (int j = room3.xs; j <= room3.xf; j++) {
            mvaddch(i,j,last_map[i][j]);
        }
    }
    room3.visited = 1;
}
void print_room4() {
    for (int i = room4.ys; i <= room4.yf; i++) {
        for (int j = room4.xs; j <= room4.xf; j++) {
            mvaddch(i,j,last_map[i][j]);
        }
    }
    room4.visited = 1;
}
void print_room5() {
    for (int i = room5.ys; i <= room5.yf; i++) {
        for (int j = room5.xs; j <= room5.xf; j++) {
            mvaddch(i,j,last_map[i][j]);
        }
    }
    room5.visited = 1;
}
void print_room6() {
    for (int i = room6.ys; i <= room6.yf; i++) {
        for (int j = room6.xs; j <= room6.xf; j++) {
            mvaddch(i,j,last_map[i][j]);
        }
    }
    room6.visited = 1;
}

void print_hallway() {
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (last_map_simple[location.y + i][location.x + j] == '#' || last_map_simple[location.y + i][location.x + j] == '+') {
                mvaddch(location.y + i, location.x + j, last_map[location.y + i][location.x + j]);
            }
        }
    }
}

void print_rooms() {
    int x = location.x; int y = location.y;
        if (room1.visited == 0) {
            if ( ( (y == pos[15][0]) && (x == pos[15][1]) ) || ( (y == pos[16][0]) && (x == pos[16][1]) ) ) {
                print_room1();
            }
        }
        if (room2.visited == 0) {
            if ( ( (y == pos[3][0]) && (x == pos[3][1]) ) || ( (y == pos[4][0]) && (x == pos[4][1]) ) ) {
                print_room2();
            }
        }
        if (room3.visited == 0) {
            if ( ( (y == pos[5][0]) && (x == pos[5][1]) ) || ( (y == pos[6][0]) && (x == pos[6][1]) ) || ( (y == pos[13][0]) && (x == pos[13][1]) ) ) {
                print_room3();
            }
        }
        if (room4.visited == 0) {
            if ( ( (y == pos[7][0]) && (x == pos[7][1]) ) || ( (y == pos[8][0]) && (x == pos[8][1]) ) || ( (y == pos[14][0]) && (x == pos[14][1]) ) ) {
                print_room4();
            }
        }
        if (room5.visited == 0) {
            if ( ( (y == pos[9][0]) && (x == pos[9][1]) ) || ( (y == pos[10][0]) && (x == pos[10][1]) ) ) {
                print_room5();
            }
        }
}

void profile_menu(char name[30]) {
    FILE* user = fopen("username.txt","r");
    FILE* gold = fopen("gold.txt","r");
    FILE* point = fopen("points.txt","r");
    FILE* email = fopen("email.txt","r");
    FILE* fg = fopen("finished_game_number.txt","r");
    char user2[30], email2[50], p[10], g[10], f[10]; int line = 0; int p2=0, fg2=0, g2=0;

    while (fgets(user2,28,user) != NULL)
    {
        line++;
        user2[strlen(user2)-1] = '\0';
        if (strcmp(user2,player.username) == 0)
            break;
    }
    fclose(user);

    for (int i = 0; i < line; i++) fgets(email2,45,email);
    email2[strlen(email2)-1] = '\0';
    fclose(email);

    for (int i = 0; i < line; i++) fgets(p,8,point);
    p[strlen(p)-1] = '\0';
    sscanf(p,"%d",&p2);
    fclose(point);

    for (int i = 0; i < line; i++) fgets(g,8,gold);
    g[strlen(g)-1] = '\0';
    sscanf(g,"%d",&g2);
    fclose(gold);

    for (int i = 0; i < line; i++) fgets(f,8,fg);
    f[strlen(f)-1] = '\0';
    sscanf(f,"%d",&fg2);
    fclose(fg);

    initscr();
    clear();
    curs_set(FALSE);
    start_color();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    attron(COLOR_PAIR(1));

    mvprintw(2, 3, "Hello %s! Welcome to your profile!",player.username);
    mvprintw(4, 3, "Email: %s",email2);
    mvprintw(5, 3, "Points: %d",p2);
    mvprintw(6, 3, "Golds: %d",g2);
    mvprintw(7, 3, "Finished Games: %d",fg2);


    int choice = 0;
    while (1)
    {
        for (int i = 0; i < 1; ++i)
        {
            if (i == choice)
                attron(A_REVERSE);
            mvprintw(10 + i, 3, "Back");
            if (i == choice)
                attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == 10)
            break;
    }
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
    game_menu_user(name);
}


int main() {
    srand(time(0));
    initialize_player();
    main_menu();
    endwin();
    return 0;
}