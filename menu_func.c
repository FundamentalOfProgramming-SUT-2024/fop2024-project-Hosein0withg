#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>


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
            choice = (choice == 0) ? 2 : choice - 1;
        else if (ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
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
        //$
        break;
    case 1:
        //$
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
    const char *menu[] = {"Red", "Green", "Blue", "White","Yellow"};
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
    echo();
    attroff(COLOR_PAIR(1));
    clear();
    refresh();
    setting(name);
    //$
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
    //$
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
        //$
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