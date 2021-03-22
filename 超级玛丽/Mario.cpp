/*---------------------------------------------------------
程序： 超级玛丽经典版   （纯C语言，可直接编译器运行）
作者： 绝伦N / 404name
说明：
--1.关于操作---
ad移动
w原地跳
qe斜跳
斜跳同时按住移动键实现远距离跳跃
蹲下和发射子弹没有写)
---------------------------------------------------------*/
#include <conio.h>  //getch()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

const int allPages = 14;
const int WIDTH = 300;
const int HEIGHT = 98;
// debug模式，默认视角锁定值为1
const int ifCameraClock = 1;
int action_Cnt = 0;                             //记录行为index
int nextGo[4][2] = {0, -1, -1, 0, 0, 1, 1, 0};  // awds; 左右0 2 上下1 3
int if_GameOver = 0;
int event_Change = 0;  //外界事件是否改变，改变就进入判断函数
int oldMarioX;
int oldMarioY;
int cameraY = 0;
struct player {
    int flag;  //分辨这是什么 0马里奥 1蘑菇 2星星 3敌人蘑菇 4敌人乌龟
    int turn;
    int leftGoCnt;
    int rightGocnt;
    int if_Falling;  //是否处于空中
    int if_Action;   // 是否在执行连续动作 并是对应动作的下标
    int X;
    int Y;
    int Height;
    int Width;
    char Image[20][40];
}
//马里奥
Mario = {0,
         1,
         0,
         0,
         0,
         -1,
         HEIGHT - 18,
         WIDTH / 3 - 50,
         8,
         18,
         {"  ###########     ", " ################ ", "    ##########    ",
          "    ##    #       ", "#### ### ######## ", "##################",
          "   ###     ###    ", "  ###       ###   "}},
    // 5个敌人蘑菇位置
    Enemy_Mushroom[5] =
        {{3, 0, 0, 0, 0, -1, HEIGHT - 18, WIDTH * 2 / 3, 8, 20, {""}},
         {3, 0, 0, 0, 0, -1, 0, 0, 8, 20, {""}},
         {3, 0, 0, 0, 0, -1, 0, 0, 8, 20, {""}},
         {3, 0, 0, 0, 0, -1, 0, 0, 8, 20, {""}},
         {3, 0, 0, 0, 0, -1, 0, 0, 8, 20, {""}}},
    // 3个乌龟位置
    Enemy_Tortoise[3] =
        {{4, 0, 0, 0, 0, -1, HEIGHT - 18, WIDTH * 2 / 3, 8, 20, {""}},
         {4, 0, 0, 0, 0, -1, 0, 0, 8, 20, {""}},
         {4, 0, 0, 0, 0, -1, 0, 0, 8, 20, {""}}},
    //变大蘑菇
    Mushroom = {1, 0, 0, 0, 0, -1, 0, 0, 8, 20, {""}},
    //变大星星
    Star = {2, 0, 0, 0, 0, -1, 0, 0, 8, 20, {""}};

int jump_Action[10][50] = {{16, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                           {30, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1,
                            1,  2, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2},
                           {30, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1,
                            1,  0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0},
                           {5, 1, 1, 1, 1, 1}};
char background[HEIGHT + 1][WIDTH * allPages + 1];
char enemy_Mushroom[9][21];         //蘑菇敌人
char alive_Tortoise_LeftGo[9][21];  //乌龟敌人
char alive_Tortoise_RightGo[9][21];
char dead_Tortoise[9][21];    // 龟壳
char star[9][21];             //星星
char to_Big_Mushroom[9][21];  //变大咕
char little_Mario_jump[9][19];
char little_Mario_leftGo1[9][19];
char little_Mario_leftGo2[9][19];
char little_Mario_rightGo1[9][19];
char little_Mario_rightGo2[9][19];
char little_Mario_stand[9][19];
char big_Mario_jump[16][21];
char big_Mario_leftGo1[16][21];
char big_Mario_leftGo2[16][21];
char big_Mario_rightGo1[16][21];
char big_Mario_rightGo2[16][21];
char big_Mario_stand[16][21];
void changePlayerTurn(int turn);
void printBackground();
void printPlayer();
void HideCursor()  //光标隐藏
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};  //后边的0代表光标不可见
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void gotoxy(int x, int y)  //让光标移动到坐标去打印内容函数
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
void initBackground() {
    for (int i = 0; i < HEIGHT; i++) {
        strcpy(background[i], "\0");
    }
}
void setColor(short x)  //自定义函根据参数改变颜色
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}
void getRoot(int pass, int page, char root[]) {
    char fileRoot[100] = "pass/";
    char tempStr[10];
    sprintf(tempStr, "%d", pass);
    strcat(fileRoot, tempStr);
    strcat(fileRoot, "/page");
    sprintf(tempStr, "%d", page);
    strcat(fileRoot, tempStr);
    strcat(fileRoot, ".txt");
    strcpy(root, fileRoot);
}

void loadAnimation() {
    FILE* fp1 = fopen("animation/little_Mario_stand.txt", "r");
    FILE* fp2 = fopen("animation/little_Mario_leftGo1.txt", "r");
    FILE* fp3 = fopen("animation/little_Mario_leftGo2.txt", "r");
    FILE* fp4 = fopen("animation/little_Mario_rightGo1.txt", "r");
    FILE* fp5 = fopen("animation/little_Mario_rightGo2.txt", "r");
    FILE* fp6 = fopen("animation/little_Mario_jump.txt", "r");
    for (int i = 0; i < 8; i++) {
        fgets(little_Mario_stand[i], sizeof(little_Mario_stand[i]) + 1, fp1);
        fgets(little_Mario_leftGo1[i], sizeof(little_Mario_leftGo1[i]) + 1,
              fp2);
        fgets(little_Mario_leftGo2[i], sizeof(little_Mario_leftGo2[i]) + 1,
              fp3);
        fgets(little_Mario_rightGo1[i], sizeof(little_Mario_rightGo1[i]) + 1,
              fp4);
        fgets(little_Mario_rightGo2[i], sizeof(little_Mario_rightGo2[i]) + 1,
              fp5);
        fgets(little_Mario_jump[i], sizeof(little_Mario_jump[i]) + 1, fp6);
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    fclose(fp6);
    FILE* Fp1 = fopen("animation/big_Mario_stand.txt", "r");
    FILE* Fp2 = fopen("animation/big_Mario_leftGo1.txt", "r");
    FILE* Fp3 = fopen("animation/big_Mario_leftGo2.txt", "r");
    FILE* Fp4 = fopen("animation/big_Mario_rightGo1.txt", "r");
    FILE* Fp5 = fopen("animation/big_Mario_rightGo2.txt", "r");
    FILE* Fp6 = fopen("animation/big_Mario_jump.txt", "r");
    for (int i = 0; i < 16; i++) {
        fgets(big_Mario_stand[i], sizeof(big_Mario_stand[i]) + 1, Fp1);
        fgets(big_Mario_leftGo1[i], sizeof(big_Mario_leftGo1[i]) + 1, Fp2);
        fgets(big_Mario_leftGo2[i], sizeof(big_Mario_leftGo2[i]) + 1, Fp3);
        fgets(big_Mario_rightGo1[i], sizeof(big_Mario_rightGo1[i]) + 1, Fp4);
        fgets(big_Mario_rightGo2[i], sizeof(big_Mario_rightGo2[i]) + 1, Fp5);
        fgets(big_Mario_jump[i], sizeof(big_Mario_jump[i]) + 1, Fp6);
    }
    getchar();
    fclose(Fp1);
    fclose(Fp2);
    fclose(Fp3);
    fclose(Fp4);
    fclose(Fp5);
    fclose(Fp6);
    FILE* Fpp1 = fopen("animation/enemy_Mushroom.txt", "r");
    FILE* Fpp2 = fopen("animation/alive_Tortoise_leftGo.txt", "r");
    FILE* Fpp6 = fopen("animation/alive_Tortoise_rightGo.txt", "r");
    FILE* Fpp3 = fopen("animation/dead_Tortoise.txt", "r");
    FILE* Fpp4 = fopen("animation/star.txt", "r");
    FILE* Fpp5 = fopen("animation/to_Big_Mushroom.txt", "r");
    for (int i = 0; i < 16; i++) {
        fgets(enemy_Mushroom[i], sizeof(enemy_Mushroom[i]) + 1, Fpp1);
        fgets(alive_Tortoise_LeftGo[i], sizeof(alive_Tortoise_LeftGo[i]) + 1,
              Fpp2);
        fgets(alive_Tortoise_RightGo[i], sizeof(alive_Tortoise_RightGo[i]) + 1,
              Fpp6);
        fgets(dead_Tortoise[i], sizeof(dead_Tortoise[i]) + 1, Fpp3);
        fgets(star[i], sizeof(star[i]) + 1, Fpp4);
        fgets(to_Big_Mushroom[i], sizeof(to_Big_Mushroom[i]) + 1, Fpp5);
    }
    fclose(Fpp1);
    fclose(Fpp2);
    fclose(Fpp3);
    fclose(Fpp4);
    fclose(Fpp5);
    fclose(Fpp6);
}

void loadFile(int pass, int pages) {
    initBackground();
    loadAnimation();
    for (int page = 0; page <= pages; page++) {
        FILE* fp = NULL;  //打开读写文件
        char fileRoot[100] = "\0";
        getRoot(pass, page, fileRoot);
        fp = fopen(fileRoot, "r");
        char buf[1024];
        for (int i = 0; i < HEIGHT; i++) {
            if (fgets(buf, sizeof(buf), fp) != NULL) {
                strncat(background[i], buf, WIDTH);
                // printf("%s\n",background[i]);
            } else
                break;
        }
        fclose(fp);
    }
}
void gameOver() {
    changePlayerTurn(0);
    Mario.turn = 1;
    Mario.X -= 30;
    printBackground();
    printPlayer();
    Sleep(200);
    while (Mario.X < HEIGHT) {
        Mario.X += 1;
        printBackground();
        printPlayer();
    }
    Sleep(1000);
    system("cls");
    printf("your are lose");
    getchar();
    getchar();
    Mario.turn = 1;
    Mario.X = HEIGHT * 2 / 3;
}
void changeTortoiseTurn(player* object, int turn) {
    if (turn == 1) {
        for (int i = 0; i < Star.Height; i++) {
            strcpy((*object).Image[i], alive_Tortoise_LeftGo[i]);
            (*object).Image[i][(*object).Width] = '\0';
        }
    } else if (turn == 2) {
        for (int i = 0; i < Star.Height; i++) {
            strcpy((*object).Image[i], alive_Tortoise_RightGo[i]);
            (*object).Image[i][(*object).Width] = '\0';
        }
    } else if (turn == 3) {
        for (int i = 0; i < Star.Height; i++) {
            strcpy((*object).Image[i], dead_Tortoise[i]);
            (*object).Image[i][(*object).Width] = '\0';
        }
    }
}
void printBackground() {
    int startPrintY = Mario.Y - WIDTH / 3;
    //视角固定
    if (ifCameraClock == 1 && startPrintY < cameraY) {
        startPrintY = cameraY;
    }
    //视角跟随马里奥前移  同时检测内容
    else {
        cameraY = startPrintY;
        for (int i = 0; i < HEIGHT; i++) {
            for (int k = 0; k < 4; k++) {
                if (background[i][cameraY + WIDTH + k] == '3') {  //生成敌人蘑菇
                    background[i][cameraY + WIDTH + k] = ' ';  //清空
                    for (int j = 0; j < 5; j++) {
                        if (Enemy_Mushroom[j].turn == 0) {
                            Enemy_Mushroom[j].turn = 1;
                            Enemy_Mushroom[j].X = i;

                            Enemy_Mushroom[j].Y = cameraY + WIDTH - 1 -
                                                  Enemy_Mushroom[j].Width + k;
                            break;
                        }
                    }
                } else if (background[i][cameraY + WIDTH + k] ==
                           '4') {  //生成敌人乌龟
                    background[i][cameraY + WIDTH + k] = ' ';
                    for (int j = 0; j < 3; j++) {
                        if (Enemy_Tortoise[j].turn == 0) {
                            Enemy_Tortoise[j].turn = 1;
                            changeTortoiseTurn(&Enemy_Tortoise[j], 1);
                            Enemy_Tortoise[j].X = i;

                            Enemy_Tortoise[j].Y = cameraY + WIDTH - 1 -
                                                  Enemy_Tortoise[j].Width + k;
                            break;
                        }
                    }
                }
            }
        }
    }
    gotoxy(0, 0);
    char tempStr[WIDTH + 1];
    for (int i = 0; i < HEIGHT; i++) {
        strncpy(tempStr, background[i] + startPrintY, WIDTH);
        tempStr[WIDTH] = '\0';
        printf("%s\n", tempStr);
    }
}

void changePlayerTurn(int flag) {
    if (flag < 6) {
        Mario.Height = 8;
        Mario.Width = 18;
        for (int i = 0; i < Mario.Height; i++) {
            if (flag == 0)
                strcpy(Mario.Image[i], little_Mario_stand[i]);
            else if (flag == 1)
                strcpy(Mario.Image[i], little_Mario_jump[i]);
            else if (flag == 2)
                strcpy(Mario.Image[i], little_Mario_rightGo1[i]);
            else if (flag == 3)
                strcpy(Mario.Image[i], little_Mario_rightGo2[i]);
            else if (flag == 4)
                strcpy(Mario.Image[i], little_Mario_leftGo1[i]);
            else if (flag == 5)
                strcpy(Mario.Image[i], little_Mario_leftGo2[i]);
            Mario.Image[i][Mario.Width] = '\0';
        }
    } else if (flag < 12) {
        Mario.Height = 16;
        Mario.Width = 20;
        for (int i = 0; i < Mario.Height; i++) {
            if (flag == 6)
                strcpy(Mario.Image[i], big_Mario_stand[i]);
            else if (flag == 7)
                strcpy(Mario.Image[i], big_Mario_jump[i]);
            else if (flag == 8)
                strcpy(Mario.Image[i], big_Mario_rightGo1[i]);
            else if (flag == 9)
                strcpy(Mario.Image[i], big_Mario_rightGo2[i]);
            else if (flag == 10)
                strcpy(Mario.Image[i], big_Mario_leftGo1[i]);
            else if (flag == 11)
                strcpy(Mario.Image[i], big_Mario_leftGo2[i]);
            Mario.Image[i][Mario.Width] = '\0';
        }
    }
}
void printObject(player object) {
    char tempStr[object.Width + 1];
    /*for(int i = 0; i < Mario.Height; i++){
            strncpy(tempStr,background[oldMarioX - Mario.Height + i] +
    oldMarioY,Mario.Width); gotoxy(oldMarioY - cameraY,oldMarioX - Mario.Height
    + i); tempStr[Mario.Width] = '\0'; printf("%s",tempStr);
    }*/
    setColor(3);
    //打印新的
    for (int i = 0; i < object.Height; i++) {
        if (object.X - object.Height + i > 0)
            gotoxy(object.Y - cameraY, object.X - object.Height + i);
        printf("%s", object.Image[i]);
        setColor(7);
        for (int j = 0; j < object.Width; j++) {
            if (object.Image[i][j] == ' ' && object.X - object.Height + i > 0) {
                gotoxy(object.Y - cameraY + j, object.X - object.Height + i);
                printf("%c",
                       background[object.X - object.Height + i][object.Y + j]);
            }
        }
        setColor(3);
    }
    setColor(7);
}
//查询所有active的player打印
void printPlayer() {
    if (Star.turn != 0) {
        printObject(Star);
    }
    if (Mushroom.turn != 0) {
        printObject(Mushroom);
    }
    //打印敌人蘑菇
    for (int j = 0; j < 5; j++) {
        if (Enemy_Mushroom[j].turn != 0) {
            printObject(Enemy_Mushroom[j]);
        }
    }
    //打印敌人乌龟
    for (int j = 0; j < 3; j++) {
        if (Enemy_Tortoise[j].turn != 0) {
            printObject(Enemy_Tortoise[j]);
        }
    }
    //打印马里奥
    if (Mario.turn != 0) {
        printObject(Mario);
        oldMarioX = Mario.X;
        oldMarioY = Mario.Y;
    }
}
void delete_Block(int x, int y) {
    int startx, starty, endx, endy;
    while (background[x - 1][y] != '#' || background[x - 1][y + 1] == '#') {
        y--;
    }
    endx = x, starty = y;
    x--;
    while (background[x][y + 1] != '#') {
        x--;
    }
    y++;
    while (background[x + 1][y] != '#') {
        y++;
    }
    startx = x, endy = y;
    char change_Block = ' ';
    //砖块围墙 ackground[startx+1][starty+1]
    //是M表示变成墙，是空格表示是变大，是O表示是顶完消失
    if (background[startx + 1][starty + 1] == 'M')
        change_Block = 'M';
    else if (background[startx + 1][starty + 1] == 'O' && Mario.turn == 1)
        return;  //小马里奥不能顶砖块 )
    else if (background[startx + 1][starty + 1] == '1') {
        Mushroom.turn = 2;
        Mushroom.X = startx - 1;
        Mushroom.Y = starty;
        change_Block = 'M';
    } else if (background[startx + 1][starty + 1] == '2') {
        Star.turn = 2;
        Star.X = startx - 1;
        Star.Y = starty;
        change_Block = 'M';
    }
    for (int i = startx; i <= endx; i++) {
        for (int j = starty; j <= endy; j++) {
            background[i][j] = change_Block;
            if (change_Block == ' ' &&
                (i == startx || i == endx || j == starty || j == endy) &&
                rand() % 4 == 0) {
                int index = rand() % 4;
                if (background[i + nextGo[index][0]][j + nextGo[index][1]] ==
                    ' ')
                    background[i + nextGo[index][0]][j + nextGo[index][1]] =
                        'o';
            }
        }
    }
}
int isInterset(player a, player b) {
    if (b.Y > a.Y + a.Width || a.X < b.X - b.Height || b.X < a.X - a.Height ||
        a.Y > b.Y + b.Width)
        return 0;
    else {
        if (a.flag == 0 &&
            (a.X == b.X - b.Height)) {  //马里奥碰撞且是往下压 则消灭
            return 2;
        }
        return 1;
    }
}
//检测状态
int checkInput(player* object, int turn) {
    if ((*object).flag != 0 && (*object).Y - cameraY <= 1) {
        (*object).turn = 0;
        printBackground();
        printPlayer();
        return 0;
    }
    if (turn == 'a' || turn == 0) {
        if (Mario.Y - cameraY <= 1) {
            (*object).if_Action = -1;
            return 0;
        }
        for (int i = 0; i < (*object).Height; i++) {
            if (background[(*object).X - (*object).Height + i]
                          [(*object).Y - 1] == 'M' ||
                background[(*object).X - (*object).Height + i]
                          [(*object).Y - 1] == '#') {
                (*object).if_Action = -1;
                if ((*object).flag != 0) {
                    if ((*object).turn == 1) {
                        (*object).turn = 2;
                        if ((*object).flag == 4) {
                            changeTortoiseTurn(object, 2);
                        }
                    } else {
                        (*object).turn = 1;
                        if ((*object).flag == 4) {
                            changeTortoiseTurn(object, 1);
                        }
                    }
                }
                return 0;
            }
        }
    } else if (turn == 'd' || turn == 2) {
        if ((*object).Y + (*object).Width - cameraY >= WIDTH) {
            if ((*object).flag != 0) {
                if ((*object).turn == 1)
                    (*object).turn = 2;
                else
                    (*object).turn = 1;
            }
            return 0;
        }
        for (int i = 0; i < (*object).Height; i++) {
            if (background[(*object).X - (*object).Height + i]
                          [(*object).Y + (*object).Width] == 'M' ||
                background[(*object).X - (*object).Height + i]
                          [(*object).Y + (*object).Width] == '#') {
                (*object).if_Action = -1;
                if ((*object).flag != 0) {
                    if ((*object).turn == 1) {
                        (*object).turn = 2;
                        if ((*object).flag == 4) {
                            changeTortoiseTurn(object, 2);
                        }
                    } else {
                        (*object).turn = 1;
                        if ((*object).flag == 4) {
                            changeTortoiseTurn(object, 1);
                        }
                    }
                }
                return 0;
            }
        }
    } else if (turn == 'w' || turn == 1) {
        if ((*object).if_Action == -1 && (*object).if_Falling == 1)
            return 0;
        if (turn == 'w' &&
            ((*object).if_Action != -1 || (*object).if_Falling == 1))
            return 0;
        if ((*object).X - (*object).Height < 1) {
            return 0;
        }
        for (int i = 0; i < (*object).Width; i++) {
            if ((*object).X - (*object).Height - 1 > 0 &&
                background[(*object).X - (*object).Height - 1]
                          [(*object).Y + i] == '#') {
                //删除顶撞砖块
                delete_Block((*object).X - (*object).Height - 1,
                             (*object).Y + i);
                return 0;
            } else if ((*object).X - (*object).Height - 1 > 0 &&
                       background[(*object).X - (*object).Height - 2]
                                 [(*object).Y + i] == '#') {
                //删除顶撞砖块
                delete_Block((*object).X - (*object).Height - 2,
                             (*object).Y + i);
                return 0;
            } else if ((*object).X - (*object).Height - 1 > 0 &&
                       background[(*object).X - (*object).Height - 1]
                                 [(*object).Y + i] == 'M') {
                (*object).if_Action = -1;
                return 0;
            }
        }
    } else if (turn == 'e' || turn == 'q') {
        if ((*object).if_Action != -1 || (*object).if_Falling == 1) {
            return 0;
        }
    }
    //非马里奥  且和物品碰撞
    int intersetFlag = isInterset(Mario, (*object));
    if ((*object).flag != 0 && intersetFlag) {
        if ((*object).flag == 1) {  //变大蘑菇
            (*object).turn = 0;
            Mario.turn = 2;
            changePlayerTurn(6);
            printBackground();
            printPlayer();
        } else if ((*object).flag == 2) {
            (*object).turn = 0;
            Mario.turn = 2;
            changePlayerTurn(6);
            printBackground();
            printPlayer();

        } else if ((*object).flag == 3 ||
                   (*object).flag == 4) {  //碰到敌人扣生命
            if (intersetFlag == 1) {
                Mario.turn--;
                if (Mario.turn == 1) {
                    changePlayerTurn(0);
                } else {
                    gameOver();
                }
            } else if (intersetFlag == 2) {
                if ((*object).flag == 4) {
                    if ((*object).turn != 3) {
                        (*object).turn = 3;
                        changeTortoiseTurn(object, 3);
                        Mario.X--;
                    } else {
                        (*object).turn = 0;
                    }
                } else
                    (*object).turn = 0;
                Mario.if_Action = 3;
                action_Cnt = 0;
                printBackground();
                printPlayer();
            }
        }
        return 0;
    } else if ((*object).flag == 0) {                             //马里奥动
        if (Mushroom.turn != 0 && isInterset(Mario, Mushroom)) {  //吃到蘑菇
            Mushroom.turn = 0;
            Mario.turn = 2;
            changePlayerTurn(6);
            return 0;
        }
        if (Star.turn != 0 && isInterset(Mario, Star)) {  //吃到星星
            Star.turn = 0;
            Mario.turn = 2;
            changePlayerTurn(6);
            return 0;
        }
        for (int i = 0; i < 5; i++) {
            if (Enemy_Mushroom[i].turn != 0 &&
                isInterset(Mario, Enemy_Mushroom[i])) {
                if (isInterset(Mario, Enemy_Mushroom[i]) == 1) {
                    Mario.turn--;
                    if (Mario.turn == 1) {
                        changePlayerTurn(0);
                    } else {
                        gameOver();
                    }
                } else if (isInterset(Mario, Enemy_Mushroom[i]) == 2) {
                    Enemy_Mushroom[i].turn = 0;
                    Mario.if_Action = 3;
                    action_Cnt = 0;
                    printBackground();
                    printPlayer();
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            if (Enemy_Tortoise[i].turn != 0 &&
                isInterset(Mario, Enemy_Tortoise[i])) {
                if (isInterset(Mario, Enemy_Tortoise[i]) == 1) {
                    Mario.turn--;
                    if (Mario.turn == 1) {
                        changePlayerTurn(0);
                    } else {
                        gameOver();
                    }
                } else if (isInterset(Mario, Enemy_Tortoise[i]) == 2) {
                    if (Enemy_Tortoise[i].turn != 3) {
                        Enemy_Tortoise[i].turn = 3;
                        changeTortoiseTurn(&Enemy_Tortoise[i], 3);
                        Mario.X--;
                    } else
                        Enemy_Tortoise[i].turn = 0;
                    Mario.if_Action = 3;
                    action_Cnt = 0;
                    printBackground();
                    printPlayer();
                }
            }
        }
    }
    return 1;
}
//判断下坠
void checkFalling(player* object) {
    if ((*object).flag != 0 && (*object).Y - cameraY <= 1) {
        (*object).turn = 0;
        return;
    }
    if ((*object).flag == 0)
        event_Change = 0;
    if (Mario.X == HEIGHT - 1) {
        gameOver();
        return;
    }
    if ((*object).X == HEIGHT - 1) {
        (*object).turn = 0;
        printBackground();
        printPlayer();
    }
    for (int i = 0; i < (*object).Width; i++) {
        if ((*object).X > 0 &&
                background[(*object).X][(*object).Y + i] == 'M' ||
            background[(*object).X][(*object).Y + i] == '#') {
            (*object).if_Falling = 0;
            return;
        }
    }
    (*object).if_Falling = 1;
    if ((*object).X - (*object).Height > 0) {
        gotoxy((*object).Y - cameraY, (*object).X - (*object).Height);
        for (int i = 0; i < (*object).Width; i++) {
            printf("%c",
                   background[(*object).X - (*object).Height][(*object).Y + i]);
        }
    }
    (*object).X++;
    printObject((*object));  //向下
    if ((*object).flag == 0) {
        oldMarioX = Mario.X;
        oldMarioY = Mario.Y;
    }
    checkInput(object, 'z');
}
void objectGo(player* object) {
    int forword = 0;
    if ((*object).turn == 0)
        return;
    if ((*object).turn == 1)
        forword = 0;
    else if ((*object).turn == 2)
        forword = 2;
    else if ((*object).turn == 3) {
        printObject((*object));
        return;
    }
    if (checkInput(object, forword)) {
        (*object).X += nextGo[forword][0];
        (*object).Y += nextGo[forword][1];
        // c擦除右边
        /*gotoxy((*object).Y - cameraY, (*object).X - (*object).Height);
        for (int i = 0; i < (*object).Width; i++) {
            printf("%c",
                   background[(*object).X - (*object).Height][(*object).Y + i]);
        }*/
        if (forword == 0) {
            for (int i = 0; i < (*object).Height; i++) {
                gotoxy((*object).Y - cameraY + (*object).Width,
                       (*object).X - (*object).Height + i);
                printf("%c", background[(*object).X - (*object).Height + i]
                                       [(*object).Y + (*object).Width]);
            }
        } else if (forword == 2) {  // c擦除左边
            for (int i = 0; i < (*object).Height; i++) {
                gotoxy((*object).Y - cameraY - 1,
                       (*object).X - (*object).Height + i);
                printf("%c", background[(*object).X - (*object).Height + i]
                                       [(*object).Y - 1]);
            }
        }
        printObject((*object));
    }
}
void checkObjectGo() {
    if (Mushroom.turn != 0) {
        checkFalling(&Mushroom);
        if (!Mushroom.if_Falling) {
            objectGo(&Mushroom);
        }
    }
    if (Star.turn != 0) {
        checkFalling(&Star);
        if (!Star.if_Falling) {
            objectGo(&Star);
        }
    }
    for (int i = 0; i < 5; i++) {
        if (Enemy_Mushroom[i].turn != 0) {
            checkFalling(&Enemy_Mushroom[i]);
            if (!Enemy_Mushroom[i].if_Falling) {
                objectGo(&Enemy_Mushroom[i]);
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        if (Enemy_Tortoise[i].turn != 0) {
            checkFalling(&Enemy_Tortoise[i]);
            if (!Enemy_Tortoise[i].if_Falling) {
                objectGo(&Enemy_Tortoise[i]);
            }
        }
    }
}
void autoChange() {
    static int delayTime = 1000;
    if (Mario.if_Action != -1) {  //执行跳跃动作的时候不自动下坠
        action_Cnt++;
        if (Mario.if_Action == 0) {
            while (checkInput(&Mario,
                              jump_Action[Mario.if_Action][action_Cnt]) != 0 &&
                   action_Cnt <= jump_Action[Mario.if_Action][0] &&
                   Mario.X - Mario.Height > 0) {
                oldMarioX = Mario.X;
                oldMarioY = Mario.Y;
                Mario.X +=
                    2 * nextGo[jump_Action[Mario.if_Action][action_Cnt]][0];
                Mario.Y += nextGo[jump_Action[Mario.if_Action][action_Cnt]][1];
                action_Cnt++;
            }
            printBackground();
            printPlayer();
            action_Cnt = 0;
            Mario.if_Action = -1;
            Mario.if_Falling = 1;
        } else {
            if (action_Cnt > jump_Action[Mario.if_Action][0]) {
                action_Cnt = 0;
                Mario.if_Action = -1;
                Mario.if_Falling = 1;
            } else if (checkInput(&Mario,
                                  jump_Action[Mario.if_Action][action_Cnt]) ==
                       0) {
                action_Cnt = 0;
                Mario.if_Action = -1;
                Mario.if_Falling = 1;
            } else {
                int cnt = 5;
                while (cnt-- &&
                       checkInput(&Mario,
                                  jump_Action[Mario.if_Action][action_Cnt]) !=
                           0 &&
                       action_Cnt <= jump_Action[Mario.if_Action][0] &&
                       Mario.X - Mario.Height > 0) {
                    oldMarioX = Mario.X;
                    oldMarioY = Mario.Y;
                    Mario.X +=
                        2 * nextGo[jump_Action[Mario.if_Action][action_Cnt]][0];
                    Mario.Y +=
                        2 * nextGo[jump_Action[Mario.if_Action][action_Cnt]][1];
                    action_Cnt++;
                }
                printBackground();
                printPlayer();
            }
        }
        checkObjectGo();
    } else if (event_Change || Mario.if_Falling) {
        checkFalling(&Mario);
        checkObjectGo();
        delayTime = 500;
    }
    delayTime--;
    if (!delayTime) {
        delayTime = 500;
        checkObjectGo();
    }
    //查看其他object状态
}
void manChange() {
    static int rightGo = 0;
    static int leftGo = 0;
    if (kbhit()) {
        HideCursor();
        char temp = getch();
        if (checkInput(&Mario, temp) == 0)
            return;
        event_Change = 1;
        if (temp == 'd' || temp == 'D') {
            Mario.rightGocnt++;
            if (Mario.turn == 1) {  //小形态
                if (Mario.rightGocnt % 2 == 0)
                    changePlayerTurn(2);
                else
                    changePlayerTurn(3);
            } else if (Mario.turn == 2) {  //大形态
                if (Mario.rightGocnt % 2 == 0)
                    changePlayerTurn(8);
                else
                    changePlayerTurn(9);
            }
            Mario.Y += 4;
        } else if (temp == 'a' || temp == 'A') {
            Mario.leftGoCnt++;
            if (Mario.turn == 1) {  //小形态
                if (Mario.leftGoCnt % 2 == 0)
                    changePlayerTurn(4);
                else
                    changePlayerTurn(5);
            } else if (Mario.turn == 2) {  //大形态
                if (Mario.leftGoCnt % 2 == 0)
                    changePlayerTurn(10);
                else
                    changePlayerTurn(11);
            }
            Mario.Y -= 4;
        } else if (temp == 'w') {
            Mario.if_Action = 0;
            /*if(Mario.X - 21 - Mario.Height > 0)
                    Mario.X -= 21;*/
        } else if (temp == 'e' || temp == 'E') {
            Mario.if_Action = 1;

        } else if (temp == 'q' || temp == 'Q') {
            Mario.if_Action = 2;

        } else if (temp == 'o') {
            Mario.turn = 1;
            changePlayerTurn(0);
        } else if (temp == 'p') {
            Mario.turn = 2;
            changePlayerTurn(6);
        } else if (temp == 'z') {
            changePlayerTurn(8);
        } else if (temp == 'x') {
            changePlayerTurn(9);
        } else if (temp == 'c') {
            changePlayerTurn(10);
        } else if (temp == 'v') {
            changePlayerTurn(11);
        }
        printBackground();
        printPlayer();
        checkObjectGo();
    }
}

void initGame() {
    HideCursor();
    oldMarioX = Mario.X;
    oldMarioY = Mario.Y;
    loadFile(1, 13);  //加载文件
                      //创建好敌人预制体5个+2个
    for (int i = 0; i < Star.Height; i++) {
        for (int j = 0; j < 5; j++) {
            strcpy(Enemy_Mushroom[j].Image[i], enemy_Mushroom[i]);
            Enemy_Mushroom[j].Image[i][Enemy_Mushroom[j].Width] = '\0';
        }
        for (int j = 0; j < 3; j++) {
            strcpy(Enemy_Tortoise[j].Image[i], alive_Tortoise_LeftGo[i]);
            Enemy_Tortoise[j].Image[i][Enemy_Tortoise[j].Width] = '\0';
        }
        strcpy(Star.Image[i], star[i]);
        Star.Image[i][Star.Width] = '\0';
        strcpy(Mushroom.Image[i], to_Big_Mushroom[i]);
        Mushroom.Image[i][Mushroom.Width] = '\0';
    }
    getchar();
    printBackground();
}
int main() {
    initGame();
    while (1) {
        manChange();
        autoChange();
    }
    return 0;
}
