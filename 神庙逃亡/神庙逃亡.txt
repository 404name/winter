#include <conio.h> //用于getch（）
#include <stdio.h>
#include <string.h>
#include <windows.h>

/*--------------------------------
1.a方向测试
2.方向测试
3.刷新方面  转弯后没刷新
2.加入开头动画
3.设置迷宫
4.加载自动前进



加入路线系统


5.优化小人
6.设计地图（初始死亡地不允许再次出现）
7.设置死亡
8设计8对应正确路劲的颜色
-------------------------------*/
int x, y, lx, ly, rx, ry, old_lx, old_ly, old_rx, old_ry, end_rx, end_ry, end_lx, end_ly; //(奇数   偶数   负数（死亡）)
int time = 15000, change_time = 5000;
//下一步移动
int next[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //w 0  s 1  a 2  d 3
int next_turn = 0, if_die = 0, if_start = 0, if_cheat = 0, if_pass = 0;
int automove;
int step = 0;
struct step_move
{
    int x;
    int y;
} s[1000000];
//w a d s 对应出0 1 2 3数组
int ch_to_num[128] = {0};

//相应显示
char turn_to_show[11] = {" o#$%^&(*X"}; //0对应空 2对应前方不可通行 5对应地刺  8对应达到正确的路劲  9对应死亡
char people_mod[7][20] = {
    {"   0  "},
    {" +/+ /"},
    {"/| |  "},
    {" +-+  "},
    {" / /  "},
    {"o o   "}};
//9个区域信息
int turn[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    Ahead_on = 0, ahead_on = 0;
char mmap[1000][1000];
char end[200][800] = {
    {"##   ##   ##            ## #   "},
    {" ##   #   ##### ######  ## ##  "},
    {"   ##### ###### ######  ##  #  "},
    {"   #######          ##  ###### "},
    {"##  ##  # #####     ########## "},
    {" ## ####  #####  ## ######     "},
    {"    ####    ##   ## ##  ##  #  "},
    {"    ## # ######   ###   ## ##  "},
    {"  # ## # ######    ##   ## ##  "},
    {"  # ## #   ##     ####   ###   "},
    {" ## ## #   ##    ##  #   ##   #"},
    {" #  ## #   ##   ##      #### ##"},
    {"## ### #  ###   #     ###  ####"},
    {"## ## ##  ##          ##    ###"},
    {"                               "},
    {"  ##     ##            ##      "},
    {"  ##  ######### ###############"},
    {" ##   ######### ###############"},
    {"##  ##   ##            ##      "},
    {"###### #######    ############ "},
    {"  ##   #######    ############ "},
    {" ##               ##   ##   ## "},
    {"###### #######    ############ "},
    {"###### #######       #####     "},
    {"       ##   ##      ## ####    "},
    {"   ### ##   ##     ##  ## ##   "},
    {"###### #######   ###   ##  ### "},
    {"###    #######   ##    ##   ## "},
    {"       ##   ##         ##      "}};
int num_map_temp = 0;
int num_map[100][100] = {0};
int num_map1[100][100] = {
    {1, 2, 2, 1, 3, 3, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //0
    {1, 5, 5, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 3, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {2, 0, 0, 0, 0, 5, 0, 0, 5, 0, 1, 1, 1, 2, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 8, 1},
    {1, 0, 0, 1, 2, 1, 3, 1, 7, 0, 0, 3, 1, 2, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
    {3, 0, 0, 1, 2, 2, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 3, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 6},
    {1, 0, 5, 1, 0, 0, 6, 1, 3, 0, 0, 1, 1, 0, 0, 1, 2, 0, 0, 0, 0, 2, 2, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 7},
    {2, 0, 0, 1, 0, 0, 7, 1, 1, 5, 0, 3, 1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 4},
    {1, 0, 5, 2, 0, 0, 4, 1, 6, 0, 0, 1, 1, 0, 5, 1, 1, 1, 1, 1, 1, 0, 0, 4, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 2, 1, 1, 0, 5, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1, 5, 0, 1, 0, 5, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 5, 2},
    {2, 5, 0, 3, 0, 0, 2, 1, 3, 0, 5, 3, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 5, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 6},
    {1, 0, 0, 1, 0, 5, 1, 1, 1, 0, 5, 3, 1, 0, 0, 0, 0, 5, 0, 0, 0, 0, 2, 4, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 5, 0, 2}, //10
    {1, 0, 5, 3, 0, 0, 2, 1, 3, 0, 0, 1, 1, 2, 2, 0, 0, 0, 0, 5, 0, 0, 2, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4, 2, 2, 1, 0, 0, 3},
    {2, 0, 0, 1, 5, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 7, 1, 7, 1, 1, 2, 2, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 5, 0, 0, 0, 0, 1, 0, 0, 2},
    {1, 0, 0, 1, 0, 0, 3, 1, 6, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 2, 3, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 2},
    {2, 5, 0, 3, 0, 5, 1, 1, 6, 0, 0, 1, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 2, 3, 1, 1, 1, 1, 1, 1, 3, 3, 3, 1, 3, 1, 3, 3, 2, 2, 0, 0, 3},
    {1, 0, 0, 1, 0, 0, 4, 1, 1, 0, 0, 1, 3, 1, 3, 1, 3, 1, 1, 4, 4, 3, 1, 3, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3},
    {1, 0, 0, 3, 5, 0, 1, 1, 1, 5, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 3},
    {2, 0, 0, 2, 0, 0, 3, 1, 3, 5, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 5, 2},
    {5, 0, 5, 1, 0, 5, 2, 1, 1, 5, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 5, 0, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 6},
    {7, 0, 0, 2, 0, 0, 7, 1, 2, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 6},
    {6, 0, 0, 1, 5, 0, 6, 1, 1, 0, 5, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 5, 0, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 5, 0, 7}, //20
    {5, 0, 0, 1, 0, 0, 7, 1, 1, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 7},
    {4, 0, 0, 2, 0, 5, 6, 1, 3, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 5, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 6},
    {3, 0, 0, 1, 0, 0, 1, 1, 1, 0, 5, 3, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 5, 7},
    {1, 0, 0, 2, 0, 0, 5, 1, 2, 0, 0, 3, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 5, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 6},
    {1, 0, 0, 1, 0, 0, 1, 1, 6, 0, 0, 1, 3, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 3},
    {1, 0, 0, 2, 0, 0, 1, 3, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 5, 0, 3},
    {1, 2, 2, 1, 2, 0, 0, 0, 0, 0, 0, 1, 1, 5, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3},
    {1, 0, 0, 1, 2, 0, 0, 0, 0, 2, 2, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 2},
    {2, 0, 0, 3, 1, 2, 1, 1, 1, 2, 2, 1, 3, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 3, 1, 1, 1, 3, 1, 2, 2, 2, 2, 3, 4, 4, 1, 0, 0, 2},
    {2, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 2, 1}, //30
    {5, 0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 1, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 5, 0, 0, 2, 1},
    {7, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3, 6, 6, 1, 3, 6, 2, 2, 3, 3, 2, 2, 2, 1, 4, 4, 1, 0, 0, 2},
    {6, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 2},
    {6, 0, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 3},
    {6, 0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 5, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
    {5, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0, 1},
    {3, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 4},
    {2, 0, 0, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 5, 4},
    {5, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 0, 0, 6},
    {4, 0, 5, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 5, 0, 6}, //40
    {1, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 3, 0, 5, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2},
    {4, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 2, 1, 3, 1, 3, 2, 2, 1, 0, 0, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 5, 2},
    {6, 0, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 3},
    {5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0, 0, 6, 0, 0, 0, 0, 5, 0, 0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 3},
    {4, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 3, 3, 3, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 5, 0, 2, 2, 2},
    {3, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 4, 6, 1, 3, 3, 1, 1, 1},
    {2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
char map1[200][500] = {
    {"+-------------------------------------------------+"},   //0
    {"|\\           {}     神 庙 逃 亡    {}            /|"},  //1
    {"| \\          {}     ----- -----    {}           / |"},  //2   1
    {"|  \\         *-->----------------<--*          /  |"},  //3
    {"|   \\                                         /   |"},  //4
    {"|    \\       *-*  ！游 戏 开 始 ！*-*        /    |"},  //5
    {"|     \\                                     /     |"},  //6
    {"|      \\     *-->----------------<--*      /      |"},  //7
    {"|       \\                                 /       |"},  //8
    {"|        \\                               /        |"},  //9  8
    {"|         \\                             /         |"},  //10    *9个
    {"|         +\\---------------------------/+         |"}}; //11

char map[200][500] = {                                        //图形界面
    {"+-------------------------------------------------+"},  //0
    {"|\\           {}     菜 单 界 面    {}            /|"}, //1
    {"| \\          {}     ----- -----    {}           / |"}, //2   1
    {"|  \\               寻找神庙的终点              /  |"}, //3
    {"|   \\                                         /   |"}, //4
    {"|    \\       *-*   按空格开始游戏   *-*      /    |"}, //5
    {"|     \\                                     /     |"}, //6
    {"|      \\     *-->  左右<->控制游戏 <--*    /      |"}, //7
    {"|       \\                                 /       |"}, //8
    {"|        \\   *--> - + 调速：       <--*  /        |"}, //9  8
    {"|         \\  (菜单界面还不能控制游戏的) /         |"}, //10    *9个
    {"|         +\\---------------------------/+         |"}, //11
    {"|         | \\                         / |         |"}, //12
    {"|         |  \\                       /  |         |"}, //13
    {"|         |   \\                     /   |         |"}, //14
    {"|         |    \\                   /    |         |"}, //15
    {"|         |     \\                 /     |         |"}, //16
    {"|         |      +---------------+      |         |"},  //17
    {"|         |      |               |      |         |"},  //18
    {"|         |      |               |      |         |"},  //19
    {"|         |      |               |      |         |"},  //20
    {"|         |      |               |      |         |"},  //21
    {"|         |      +---------------+      |         |"},  //22
    {"|         |     /        |        \\     |         |"}, //23
    {"|         |    /         |         \\    |         |"}, //24
    {"|         |   /          |          \\   |         |"}, //25
    {"|         |  /           |           \\  |         |"}, //26
    {"|         | /            |            \\ |         |"}, //27
    {"|         +/-------------|-------------\\+         |"}, //28
    {"|         /              |              \\         |"}, //29  *9
    {"|        /               |               \\        |"}, //30
    {"|       /                |                \\       |"}, //31
    {"|      /                 |                 \\      |"}, //32
    {"|     /                  |                  \\     |"}, //33
    {"|    /                   |                   \\    |"}, //34
    {"|   /                    |                    \\   |"}, //35
    {"|  /                     |                     \\  |"}, //36
    {"| /                      |                      \\ |"}, //37
    {"|/                       |                       \\|"}, //38
    {"+-------------------------------------------------+"}}; //39
void gotoxy(int x, int y)                                     //坐标函数
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
void loading()
{ //按一下提示注意切换英文输入
    char ch;
    int n = 11, m;
    int i, j, k, min = 0, max, ti = 0, tj = 0;
    int step = 0;
    char messge[7][15] = {
        {"  3D神庙逃亡  "},
        {"  C语言精简版 "},
        {" made by 绝伦N"},
        {"              "},
        {" 加载菜单界面 "},
        {" 按任意键继续 "}};
    char messge1[7][15] = {
        {"  恭喜通关！！"},
        {"  恭喜通关！！"},
        {"  恭喜通关！！"},
        {"              "},
        {"  按空格重置  "},
        {" 按其他键结束 "}};
    system("color 05");
    system("mode con cols=52 lines=42");
    m = 5 + n * 4;
    max = m;
    while (m >= 5)
    {
        system("color 05");
        for (i = min; i < max; i++)
        {
            for (j = min; j < max; j++)
            {
                if (j >= min + 2 && j < max - 2 && (i == min || i == max - 1))
                    mmap[i][j] = step % 2 == 0 ? '#' : '+';
                else if ((j == min + 2 || j == max - 3) && (i == min + 1 || i == max - 2))
                    mmap[i][j] = step % 2 == 0 ? '#' : '+';
                else if (i >= min + 2 && i < max - 2 && (j == min || j == max - 1))
                    mmap[i][j] = step % 2 == 0 ? '#' : '+';
                else if ((i == min + 2 || i == max - 3) && (j == min + 1 || j == max - 2))
                    mmap[i][j] = step % 2 == 0 ? '#' : '+';
                else if ((i == min + 2 || i == max - 3) && (j == min + 2 || j == max - 3))
                    mmap[i][j] = step % 2 == 0 ? '#' : '+';
                else if (mmap[i][j] == 0)
                    mmap[i][j] = ' ';
            }
        }
        step++;
        min += 2;
        max -= 2;
        m -= 4;
        for (i = 9; i < 5 + n * 4; i++)
        {
            for (j = 0; j < 5 + n * 4; j++)
            {
                printf("%c", mmap[i][j]);
            }
            printf("\n");
        }
        Sleep(500);
        gotoxy(0, 0);
        if (step == 9 && if_pass == 0)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
            for (k = 0; k < 6; k++)
            {
                gotoxy(17, 10 + 2 * k);
                printf("%s", messge[k]);
            }
            getch();
        }
        else if (step == 9 && if_pass == 1)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
            for (k = 0; k < 6; k++)
            {
                gotoxy(17, 10 + 2 * k);
                printf("%s", messge1[k]);
            }
            ch = getch();
            if (ch == ' ')
                return;
            else
                exit(0);
        }
        if (step == 9)
        {
            system("cls");
        }
    }
    min -= 2;
    max += 2;
    m += 4;
    while (step--)
    {
        for (i = min; i < max; i++)
        {
            for (j = min; j < max; j++)
            {
                if (j >= min + 2 && j < max - 2 && (i == min || i == max - 1))
                    mmap[i][j] = ' ';
                else if ((j == min + 2 || j == max - 3) && (i == min + 1 || i == max - 2))
                    mmap[i][j] = ' ';
                else if (i >= min + 2 && i < max - 2 && (j == min || j == max - 1))
                    mmap[i][j] = ' ';
                else if ((i == min + 2 || i == max - 3) && (j == min + 1 || j == max - 2))
                    mmap[i][j] = ' ';
                else if ((i == min + 2 || i == max - 3) && (j == min + 2 || j == max - 3))
                    mmap[i][j] = ' ';
                else if (mmap[i][j] == 0)
                    mmap[i][j] = ' ';
            }
        }
        min -= 2;
        max += 2;
        m += 4;
        for (i = 9; i < 5 + n * 4; i++)
        {
            for (j = 0; j < 5 + n * 4; j++)
            {
                printf("%c", mmap[i][j]);
            }
            printf("\n");
        }
        Sleep(100);
        gotoxy(0, 0);
    }
    for (i = 0; i < 5 + n * 4; i++)
    {
        for (j = 0; j < 5 + n * 4; j++)
        {
            printf("%c", mmap[i][j]);
        }
        printf("\n");
    }
    return;
}
void init()
{
    int n = 40, i;
    system("color 07");
    system("mode con cols=52 lines=42");
    memcpy(num_map, num_map1, sizeof(num_map1)); //复制地图，用于重新开始。
    ch_to_num['w'] = 0;
    ch_to_num['s'] = 1;
    ch_to_num['a'] = 2;
    ch_to_num['d'] = 3;
    next_turn = 0, if_die = 0, if_pass = 0;
    step = 0;
    automove = time;
    Ahead_on = 0, ahead_on = 0;
    if (if_start == 1)
    { //正式开始游戏；
        1;
        x = 26;
        y = 1;
    }
    else
    {
        x = 41, y = 1;
    }
    lx = x, ly = y;
    old_lx = lx, old_ly = ly;
    rx = x, ry = y + 1;
    end_ly = 43, end_ry = 44;
    end_lx = 3, end_rx = 3;

    for (i = 0; i < 10; i++)
        turn[i] = 0;
    for (i = 0; i < n; i++)
    {
        if (i <= 11)
        {
            if (if_start == 1)
                printf("%s\n", map1[i]);
            else
                printf("%s\n", map[i]);
        }
        else
            printf("%s\n", map[i]);
        Sleep(50);
    }
    if (if_start == 0)
    {
        gotoxy(27, 9);
        printf("%6d", time);
    }
}
void Left1(int turn) //0
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);

    for (i = 2; i <= 37; i++)
    {
        gotoxy(1, i);
        if (i <= 9)
        {

            for (j = 2; j <= i; j++)
            {

                printf("%c", turn_to_show[turn]);
            }
        }
        else if (i <= 29)
        {
            for (j = 1; j <= 9; j++)
            {
                printf("%c", turn_to_show[turn]);
            }
        }
        else
        {
            for (j = i; j <= 37; j++)
            {
                printf("%c", turn_to_show[turn]);
            }
        }
        printf("\n");
    }
}
void Left2(int turn) //1
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
    for (i = 12; i <= 27; i++)
    {
        gotoxy(11, i);
        if (i <= 16)
        {

            for (j = 12; j <= i; j++)
            {

                printf("%c", turn_to_show[turn]);
            }
        }
        else if (i <= 22)
        {
            for (j = 0; j <= 5; j++)
            {
                printf("%c", turn_to_show[turn]);
            }
        }
        else
        {
            for (j = i; j <= 27; j++)
            {
                printf("%c", turn_to_show[turn]);
            }
        }
    }
}
void left1(int turn) //2
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
    if (turn == -2)
    {
        for (i = 29; i <= 34; i++)
        {
            gotoxy(15, i);
            printf("%s", people_mod[i - 29]);
        }
    }
    else
    {
        for (i = 29; i <= 38; i++)
        {
            gotoxy(2 + 38 - i, i);
            for (j = 1 + 38 - i; j <= 23; j++)
                printf("%c", turn_to_show[turn]);
        }
    }
}
void left2(int turn) //3
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
    for (i = 23; i <= 27; i++)
    {
        gotoxy(13 + 27 - i, i);
        for (j = 1 + 27 - i; j <= 12; j++)
            printf("%c", turn_to_show[turn]);
    }
}
void Right1(int turn) //4
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
    for (i = 2; i <= 37; i++)
    {
        if (i <= 9)
        {
            gotoxy(41 + (10 - i), i);
            for (j = 2; j <= i; j++)
            {

                printf("%c", turn_to_show[turn]);
            }
        }
        else if (i <= 29)
        {
            gotoxy(41, i);
            for (j = 1; j <= 9; j++)
            {
                printf("%c", turn_to_show[turn]);
            }
        }
        else
        {
            gotoxy(41 + i - 29, i);
            for (j = i; j <= 37; j++)
            {
                printf("%c", turn_to_show[turn]);
            }
        }
    }
}
void Right2(int turn) //5
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);

    for (i = 12; i <= 27; i++)
    {

        if (i <= 16)
        {
            gotoxy(34 + (17 - i), i);
            for (j = 12; j <= i; j++)
            {

                printf("%c", turn_to_show[turn]);
            }
        }
        else if (i <= 22)
        {
            gotoxy(34, i);
            for (j = 0; j <= 5; j++)
            {
                printf("%c", turn_to_show[turn]);
            }
        }
        else
        {
            gotoxy(34 + i - 22, i);
            for (j = i; j <= 27; j++)
            {
                printf("%c", turn_to_show[turn]);
            }
        }
    }
}
void right1(int turn) //6
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
    if (turn == -2)
    {
        for (i = 29; i <= 34; i++)
        {
            gotoxy(28, i);
            printf("%s", people_mod[i - 29]);
        }
    }
    else
    {
        for (i = 29; i <= 38; i++)
        {
            gotoxy(26, i);
            for (j = 1 + 38 - i; j <= 23; j++)
                printf("%c", turn_to_show[turn]);
        }
    }
}
void right2(int turn) //7
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
    for (i = 23; i <= 27; i++)
    {
        gotoxy(26, i);
        for (j = 1 + 27 - i; j <= 12; j++)
            printf("%c", turn_to_show[turn]);
    }
}
void ahead(int turn) //9
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);

    for (i = 18; i <= 21; i++)
    {
        gotoxy(18, i);
        for (j = 0; j < 15; j++)
            printf("%c", turn_to_show[turn]);
    }
}
void top(int turn)
{
    int i, j;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 01);
    if (turn == 9)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 04);
    if (turn == 8)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 02);
    for (i = 12; i <= 16; i++)
    {
        gotoxy(13 + i - 12, i);
        for (j = 0; j < 25 - 2 * (i - 12); j++)
            printf("%c", turn_to_show[turn]);
    }
}
void Ahead(int t) //8
{
    if (t == 0)
    {
        top(t);
        ahead(t);
    }
    else if (t == 9)
    {
        ahead(t);
        left2(t);
        right2(t);
        Left2(t);
        Right2(t);
        top(t);
    }
    else
    {
        ahead(t);
        if (turn[3] == 0)
            left2(t);
        if (turn[7] == 0)
            right2(t);
        if (turn[1] == 0)
            Left2(t);
        if (turn[5] == 0)
            Right2(t);
        top(t);
    }
}
void stop() //游戏结束加载
{
    char ch;
    int c, d, i;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);

    gotoxy(0, 0);
    for (c = 0; c < 40; c++)
    {
        for (d = 0; d < 50; d++)
            printf("0");
        printf("\n");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 05);
    for (i = 0; i < step; i++)
    {
        gotoxy(s[i].y, s[i].x);
        printf("#");
        Sleep(100);
    }
    ch = getch();
    while (ch != ' ')
    {
        ch = getch();
    }
}
void gamepass()
{
    int i;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
    for (i = 0; i <= 28; i++)
    {
        gotoxy(10, 6 + i);
        printf("%s\n", end[i]);
        Sleep(100);
    }
    Sleep(1000);
    loading();
}
void show()
{
    /*gotoxy(0, 40);
    printf("x:%d  y%d ", x, y);
    gotoxy(0, 41);
    printf("lx:%d  ly%d ", lx, ly);
    gotoxy(0, 42);
    printf("rx:%d  ry%d  ahead_on = %d  Ahead_on = %d", rx, ry, ahead_on, Ahead_on);*/
    int tx, ty;
    if (if_die)
    {
        lx = old_lx, ly = old_ly;
        rx = old_rx, ry = old_ry;
        num_map_temp = num_map[x][y];
        num_map[x][y] = 9;
        x = 0, y = 0;
    }
    //识别左边
    tx = (next_turn == 2 || next_turn == 3) ? lx - next[next_turn][1] : lx + next[next_turn][1];
    ty = (next_turn == 2 || next_turn == 3) ? ly - next[next_turn][0] : ly + next[next_turn][0];
    if (num_map[tx][ty] != turn[0]) //lx ly相对运动方向的左边
    {
        turn[0] = num_map[tx][ty];
        Left1(turn[0]);
    }
    /*gotoxy(0, 43);
    printf("%d", num_map[tx][ty]);*/
    tx = tx + next[next_turn][0],
    ty = ty + next[next_turn][1];
    if (num_map[tx][ty] != turn[1]) //左前方
    {
        turn[1] = num_map[tx][ty];
        Left2(turn[1]); //1
    }
    /* gotoxy(0, 44);
    printf("%d", num_map[tx][ty]);*/
    if (num_map[lx][ly] != turn[2] || (lx == x && ly == y)) //原地
    {
        if (lx == x && ly == y && turn[2] == -2)
            1;
        else if (lx == x && ly == y)
        {
            turn[2] = -2;
            left1(turn[2]); //2
        }
        else
        {
            turn[2] = num_map[lx][ly];
            left1(turn[2]); //2}
        }
    }
    if (num_map[lx + next[next_turn][0]][ly + next[next_turn][1]] != turn[3]) //相对运动方向的前边
    {
        turn[3] = num_map[lx + next[next_turn][0]][ly + next[next_turn][1]];
        if (turn[3] == 9 && num_map[rx + next[next_turn][0]][ry + next[next_turn][0]] != 0)
            turn[8] = 9;
        left2(turn[3]); //3
    }

    //识别右边
    tx = (next_turn == 2 || next_turn == 3) ? rx + next[next_turn][1] : rx - next[next_turn][1];
    ty = (next_turn == 2 || next_turn == 3) ? ry + next[next_turn][0] : ry - next[next_turn][0];
    if (num_map[rx + next[next_turn][1]][ry - next[next_turn][0]] != turn[4]) //相对运动方向的右边
    {
        turn[4] = num_map[rx + next[next_turn][1]][ry - next[next_turn][0]];
        Right1(turn[4]); //4
    }
    /*gotoxy(0, 45);
    printf("%d", num_map[tx][ty]);*/
    tx = tx + next[next_turn][0],
    ty = ty + next[next_turn][1];
    if (num_map[tx][ty] != turn[5])
    {
        turn[5] = num_map[tx][ty];
        Right2(turn[5]); //5
    }
    /* gotoxy(0, 46);
    printf("%d", num_map[tx][ty]);*/
    if (num_map[rx][ry] != turn[6] || (rx == x && ry == y))
    {
        if (rx == x && ry == y && turn[6] == -2)
            1;
        else if (rx == x && ry == y)
        {
            turn[6] = -2;
            right1(turn[6]);
        }
        else
        {
            turn[6] = num_map[rx][ry];
            right1(turn[6]);
        } //6
    }
    if (num_map[rx + next[next_turn][0]][ry + next[next_turn][1]] != turn[7])
    {
        turn[7] = num_map[rx + next[next_turn][0]][ry + next[next_turn][1]];
        if (turn[7] == 9 && num_map[lx + next[next_turn][0]][ly + next[next_turn][0]] != 0)
            turn[8] = 9;
        right2(turn[7]); //7
    }
    //由于识别前面造成的把之后的刷新；
    //识别前面

    if (num_map[lx + next[next_turn][0]][ly + next[next_turn][1]] && num_map[rx + next[next_turn][0]][ry + next[next_turn][1]])
    {
        if (Ahead_on == 0 || turn[8] == 9 || if_die)
        {
            turn[8] = num_map[lx + next[next_turn][0]][ly + next[next_turn][1]];
            if (if_die)
            {
                turn[8] = 9;
            }
            Ahead(turn[8]); //8
            Ahead_on = 1;
        }
    }
    else if (Ahead_on == 1)
    {
        Ahead_on = 0;
        turn[8] = 0;
        Ahead(turn[8]); //8
    }
    if (Ahead_on == 0)
    {
        if (num_map[lx + 2 * next[next_turn][0]][ly + 2 * next[next_turn][1]] && num_map[rx + 2 * next[next_turn][0]][ry + 2 * next[next_turn][1]])
        {
            if (ahead_on == 0)
            {
                turn[9] = num_map[lx + 2 * next[next_turn][0]][ly + 2 * next[next_turn][1]];
                if (if_die)
                {
                    turn[9] = 9;
                }
                ahead(turn[9]); //9
                ahead_on = 1;
            }
        }
        else if (ahead_on == 1)
        {
            ahead_on = 0;
            turn[9] = 0;
            ahead(turn[9]); //8
        }
    }
    if (if_die == 1 && if_start == 1)
    {
        if_start = 0;
        Sleep(2000);
        stop();
        init();
        return;
    }
    if ((x == end_lx && y == end_ly) || (x == end_rx && y == end_ry))
    {
        if_start = 0;
        if_pass = 1;
        stop();
        gamepass();
        init();
        return;
    }
    old_lx = lx;
    old_ly = ly;
    old_rx = rx;
    old_ry = ry;
}

void withoutinput()
{
    automove--;
    if (automove == 0)
    {
        automove = time;
        x = x + next[next_turn][0];
        y = y + next[next_turn][1];
        lx = lx + next[next_turn][0];
        ly = ly + next[next_turn][1];
        rx = rx + next[next_turn][0];
        ry = ry + next[next_turn][1];
        s[step].x = x;
        s[step].y = y;
        step++;
        if (num_map[x][y])
        {
            if_die = 1;
        }
        show();
    }
    gotoxy(0, 0);
}

void withinput()
{
    if (kbhit())
    {
        char ch = getch();
        if (if_start == 1 && (ch == 'a' || ch == 'd' || ch == 'w' || ch == -32)) //开始游戏了
        {
            if (ch == -32)
            { //同时启用wasd  和上下左右键   //如果读取到第一个是-32那么 继续读取一个字符
                char ch1 = getch();
                switch (ch1)
                {
                case 72:
                    ch = 'w';
                    break;
                case 75:
                    ch = 'a';
                    break;
                case 80:
                    return;
                case 77:
                    ch = 'd';
                    break;
                default:
                    return;
                }
            }
            switch (ch)
            {
            case 'a':
            {
                if (next_turn == 2 || next_turn == 3)
                {
                    x = x - next[next_turn][1];
                    y = y - next[next_turn][0];
                }
                else
                {
                    x = x + next[next_turn][1];
                    y = y + next[next_turn][0];
                }
                break;
            }
            case 'd':
            {
                if (next_turn == 2 || next_turn == 3)
                {
                    x = x + next[next_turn][1];
                    y = y + next[next_turn][0];
                }
                else
                {
                    x = x - next[next_turn][1];
                    y = y - next[next_turn][0];
                }
                break;
            }
            case 'w':
            {
                x = x + next[next_turn][0];
                y = y + next[next_turn][1];
                lx = lx + next[next_turn][0];
                ly = ly + next[next_turn][1];
                rx = rx + next[next_turn][0];
                ry = ry + next[next_turn][1];
                break;
            }
            case 's':
                break;
            case ' ':
                break;
            }
            s[step].x = x;
            s[step].y = y;
            step++;
            if (num_map[x][y] != 0)
            {
                if_die = 1;
                return;
            }
            if ((x == lx && y == ly) || (x == rx && y == ry))
                1;
            else
            {
                if (next_turn == 0)
                {
                    if (ch == 'a')
                        next_turn = 2;
                    else if (ch == 'd')
                        next_turn = 3;
                }
                else if (next_turn == 3)
                {
                    if (ch == 'a')
                        next_turn = 0;
                    else if (ch == 'd')
                        next_turn = 1;
                }
                else if (next_turn == 1)
                {
                    if (ch == 'a')
                        next_turn = 3;
                    else if (ch == 'd')
                        next_turn = 2;
                }
                else if (next_turn == 2)
                {
                    if (ch == 'a')
                        next_turn = 1;
                    else if (ch == 'd')
                        next_turn = 0;
                }
                if (ch == 'a')
                {
                    lx = x, ly = y;
                    rx = (next_turn == 2 || next_turn == 3) ? x + next[next_turn][1] : x - next[next_turn][1];
                    ry = (next_turn == 2 || next_turn == 3) ? y + next[next_turn][0] : y - next[next_turn][0];
                }
                else if (ch == 'd')
                {
                    rx = x, ry = y;
                    lx = (next_turn == 2 || next_turn == 3) ? x - next[next_turn][1] : x + next[next_turn][1];
                    ly = (next_turn == 2 || next_turn == 3) ? y - next[next_turn][0] : y + next[next_turn][0];
                }
            }
        }
        else if (if_start == 0 && ch == ' ') //空格开始游戏
        {
            if_start = 1;
            init();
            return;
        }
        else if (if_start == 0 && ch == '+') //按G一键开挂（浏览模式）
        {
            time = time + change_time;
            gotoxy(27, 9);
            printf("%6d", time);
        }
        else if (if_start == 0 && ch == '-')
        { //调节速度
            if (time >= change_time)
            {
                time = time - change_time;
                gotoxy(27, 9);
                printf("%6d", time);
            }
        }
    }
}

int main()
{
    loading();
    init();
    while (1)
    {

        withinput();
        if (if_die <= 1)
            show();
        if (if_cheat == 0 && if_die == 0)
            withoutinput();
        if (if_die == 1)
            if_die++;
    }
    getchar();
    getchar();
    return 0;
}