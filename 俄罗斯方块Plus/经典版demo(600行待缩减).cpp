#include <conio.h> //用于getch（）
#include <math.h>
#include <stdio.h>
#include <stdlib.h> //用于rand
#include <string.h>
#include <time.h>
#include <windows.h>
//■□
/*-----
日志：
·人物移动格外算时间，有下降加快更新，上升同样，这时候不算随机数，平常走动随机数
·win画面更新。开头介绍，胜利。下降时间控制 



--------*/

int map[500][500] = {0};
int color_map[500][500] = {0};
int check_sum[500][2] = {0};
int if_full = 0;
//下标按顺序为  总分  应该下降的层数
//逻辑顺序是谁  check_sun储存对应map层数的和，当满的时候，他向上的有值的层数就要增加相应的该降层数。
int height, width, top;
int start_x = 6, start_y = 10, tx, ty, x, y, times, cnt_time, old_index0, index0, cnt = 0;
int if_change = 0, if_death = 0, reduce_time = 0;
int now_turn = 4, next_turn = 4, next_next_turn = 4;
int next_position_x[2] = {1, 1}, next_position_y[2] = {10, 2}, show_next = 0, show_first = 0;
int now_color = 3, next_color = 2, next_next_color = 3;

int death_line = 8;
char message[7][20] = { //自定义logo
    {"[][][][][]"},
    {"[Tetris  ]"},
    {"[    plus]"},
    {"[俄罗斯  ]"},
    {"[方块plus]"},
    {"[][][][][]"}};
struct Pass
{
    char ss[10][11];
} pass[20] = {
    {{{"0000000000"}, //1可以自己设置基础关卡 0空气 1砖块
      {"0000000000"},
      {"0000000000"},
      {"0000000000"},
      {"0000000020"},
      {"0000000000"},
      {"0000000000"},
      {"0000000000"}}}};
int now_pass = 1; //多关卡，现在默认第一关

int T[30][4] = {              //7种4*4状态，对应的数据是4位数对应4种情况（有效数前面不能为零不然会认为其他进制）
    {0000, 1011, 0000, 0000}, //T
    {1101, 1111, 111, 0000},
    {0000, 1110, 0000, 0000},
    {0000, 0000, 0000, 0000},

    {0000, 0000, 0000, 0000}, //田
    {0000, 1111, 1111, 0000},
    {0000, 1111, 1111, 0000},
    {0000, 0000, 0000, 0000},

    {1111, 1010, 1010, 1010}, //l
    {101, 0000, 0000, 0000},
    {101, 0000, 0000, 0000},
    {101, 0000, 0000, 0000},

    {1000, 1010, 1, 0000}, //右L
    {101, 1111, 101, 0000},
    {100, 1010, 10, 0000},
    {0000, 0000, 0000, 0000},

    {0001, 1010, 10, 0000}, //左L
    {101, 1111, 101, 0000},
    {1000, 1010, 100, 0000},
    {0000, 0000, 0000, 0000},

    {0000, 1111, 101, 0000}, //右与
    {101, 1111, 1010, 0000},
    {0000, 0000, 1010, 0000},
    {0000, 0000, 0000, 0000},

    {101, 1111, 0000, 0000}, //左与
    {1010, 1111, 101, 0000},
    {1010, 0000, 0000, 0000},
    {0000, 0000, 0000, 0000}};

void HideCursor();
int show(int out, int in, int if_auto, int class0);
void init_brick();
void death_line_drop(int turn);
void check();
void HideCursor() //光标隐藏
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0}; //后边的0代表光标不可见
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoxy(int x, int y) //坐标函数
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}

void init_brick()
{
    now_color = next_color;
    next_color = next_next_color;
    next_next_color = rand() % 9;
    if (next_next_color == 0)
        next_next_color = 1;
    x = start_x;
    y = start_y;
    tx = x, ty = y;
    now_turn = next_turn;
    next_turn = next_next_turn;
    next_next_turn = rand() % 7;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), next_color);
    show_next = 1; //打印下下个
    show(1, 1, 1, 1);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), next_next_color);
    show_next = 2; //打印下个
    show(1, 1, 1, 1);
    show_next = 0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), now_color);
    show_first = 1;
    show(1, 1, 1, 1);
    show_first = 0;
}

void init(int pass0)
{
    HideCursor(); //隐藏光标
    int i, j;
    system("cls");
    times = 500;
    cnt_time = 0;
    index0 = 1;
    x = start_x, y = start_y;
    tx = x, ty = y;
    top = 5;
    death_line = 8;
    height = 26;
    width = 23;
    //游戏开始样式
    for (i = 0; i <= height; i++)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rand() % 8 + 1);
        for (j = 0; j <= width; j = j + 2)
        {
            gotoxy(j, i);
            if (i == 0 || i == height || j == 0 || j == width - 1)
                printf("■");
            else
                printf("□");
        }
    }
    gotoxy(6, height / 2 - 2);
    printf("[Pass: No%2d]", pass0);
    gotoxy(6, height / 2);
    printf("[Start game]");
    getchar();
    //点击开始
    gotoxy(0, 0);
    //构建地图
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    for (i = 0; i <= height; i++)
    {
        check_sum[i][0] = 0;
        check_sum[i][1] = 0;
        for (j = 0; j <= width; j++)
        {
            if (i == 0 || j == 0 || j == 1 || i == height || j == width || j == width - 1 || i == top)
            {
                map[i][j] = 2;
            }
            else
                map[i][j] = 0;
            color_map[i][j] = 3;
        }
    }
    for (i = 0; i <= height; i++)
    {
        for (j = 0; j <= width; j = j + 2)
        {
            if (map[i][j] == 2)
            {
                printf("[]");
            }
            //这里是从17行开始打印8行关卡。
            else if (i >= (height - 8) && pass[pass0 - 1].ss[i - (height - 8)][j / 2 - 1] != '0' && pass0 != 0)
            {
                if (pass[pass0 - 1].ss[i - (height - 8)][j / 2 - 1] == '1')
                {
                    map[i][j] = 2;
                    check_sum[i][0] += 2;
                    printf("■");
                }
                else
                    printf("  ");
            }
            else
                printf("  ");
        }
        Sleep(50);
        printf("\n");
    }
    for (i = 1; i <= 4; i++)
    {
        gotoxy(8, i);
        printf("[]");
        gotoxy(16, i);
        printf("[]");
    }
    for (i = 0; i < 6; i++)
    {
        gotoxy(18, i);
        printf("%s", message[i]);
    }
    for (i = 6; i <= height; i++)
    {
        gotoxy(24, i);
        if (i != height)
            printf("  []");
        else
            printf("[][]");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    for (i = 6; i <= death_line; i++)
    {
        gotoxy(26, i);
        printf("[]");
    }
    death_line_drop(1);
    srand(time(NULL));
    next_turn = rand() % 7;
    next_next_turn = rand() % 7;
    init_brick();
    getch();
}

void death_line_drop(int turn) //0对应打印 1对应下降
{
    //清空死亡线
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    gotoxy(0, death_line);
    printf("[]");
    gotoxy(width - 1, death_line);
    printf("[]  ");
    for (int i = 2; i <= width - 2; i = i + 2)
    {
        gotoxy(i, death_line);
        if (map[death_line][i] != 1)
            printf("  ");
    }
    if (turn == 1) //是否下降
        death_line++;
    //打印新的死亡线
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    gotoxy(0, death_line);
    printf("{}");
    gotoxy(width - 1, death_line);
    printf("{}--[]");
    check_sum[death_line][0] = 0;
    for (int i = 2; i <= width - 2; i = i + 2)
    {
        if (map[death_line][i] == 2)
        {
            map[death_line][i] = 0;
        }
        gotoxy(i, death_line);
        if (map[death_line][i] != 1)
            printf("--");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), now_color);
    check();
}

void delete_line()
{
    HideCursor(); //隐藏光标
    int i, j, temp = 0;
    for (i = height - 1; i >= death_line; i--)
    {
        if (check_sum[i][0] == 20)
        {
            check_sum[i][0] = 0;
            for (j = i - 1; j >= death_line; j--)
            {

                check_sum[j][1]++;
            }
        }
    }
    //下降
    for (i = height - 1; i >= death_line; i--)
    {
        if (check_sum[i][1])
        {
            check_sum[i + check_sum[i][1]][0] = check_sum[i][0];
            check_sum[i][0] = 0;
            gotoxy(2, i + check_sum[i][1]);
            for (j = 2; j <= width - 2; j = j + 2)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_map[i][j]);
                map[i][j] == 2 ? printf("■") : printf("  ");
                map[i + check_sum[i][1]][j] = map[i][j];
            }
            gotoxy(2, i);
            for (j = 2; j <= width - 2; j = j + 2)
            {
                map[i][j] = 0;
                printf("  ");
            }
            check_sum[i][1] = 0;
        }
    }
    check();
    Sleep(50);
    //刷新死亡线
    death_line_drop(0);
}

int get_index0_num(int res, int index0)
{
    int ans = 0;
    while (res && index0)
    {
        ans = res % 10;
        index0--;
        res /= 10;
    }
    if (index0)
        return 0;
    return ans;
}

int show(int out, int in, int if_auto, int class0) //是否清除 是否加载 是否自动下落  赋值类型（1在移动 2定位的）
{
    int i, j;
    int temp_x;
    int temp_y;
    int temp_tx;
    int temp_ty;
    int temp_now_turn;
    if (show_next == 1 || show_next == 2)
    {
        temp_now_turn = now_turn;
        temp_x = x;
        temp_y = y;
        temp_tx = tx;
        temp_ty = ty;
        x = next_position_x[show_next - 1];
        y = next_position_y[show_next - 1];
        now_turn = (show_next == 2 ? next_next_turn : next_turn);
        tx = x, ty = y;
    }
    if (out == 1) //清除
    {
        for (i = tx; i < tx + 4; i++)
        {
            for (j = ty; j < ty + 4; j++)
            {

                if (i <= height && 2 * j - ty <= width && map[i][2 * j - ty] == 1)
                {
                    gotoxy(2 * j - ty, i);
                    map[i][2 * j - ty] = 0;
                    if (i == death_line)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                        printf("--");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), now_color);
                    }
                    else
                        printf("  ");
                }
            }
        }
    }
    if (in == 1) //打印
    {
        int now_turn_index = now_turn * 4; //对应到的初始下标
        for (i = x; i < x + 4; i++)
        {
            for (j = y; j < y + 4; j++)
            {
                if (i <= height && 2 * j - y <= width && get_index0_num(T[i - x + now_turn_index][j - y], index0) == 1 && map[i][2 * j - y] == 0)
                {
                    gotoxy(2 * j - y, i);
                    if (!(class0 == 2 && i <= death_line))
                        map[i][2 * j - y] = class0;
                    if (class0 == 2 && i > death_line)
                    {
                        check_sum[i][0] += 2;
                        if (check_sum[i][0] == 20)
                        {
                            if_full = 1;
                        }
                        color_map[i][2 * j - y] = now_color;
                    }
                    if (!(class0 == 2 && i <= death_line))
                        printf("■");
                }
                else if (i <= height && 2 * j - y <= width && get_index0_num(T[i - x + now_turn_index][j - y], index0) == 1 && map[i][2 * j - y] == 2 && if_auto == 1) //失败则回退
                {
                    if (show_first)
                    {
                        if_death = 1;
                    }
                    x = tx;
                    y = ty;
                    show(1, 1, 1, 2);
                    Sleep(300);
                    init_brick();
                    show(1, 1, 1, 1);
                    return 0;
                }
                else if (i <= height && 2 * j - y <= width && get_index0_num(T[i - x + now_turn_index][j - y], index0) == 1 && map[i][2 * j - y] == 2) //失败则回退
                {
                    int ttx = tx;
                    int tty = ty;
                    tx = x;
                    ty = y;
                    show(1, 0, 0, 0);
                    x = ttx;
                    y = tty;
                    if (if_change == 1)
                    {
                        index0 = old_index0;
                        show(1, 1, 0, 1);
                    }
                    else if (if_auto == 0)
                    {
                        show(1, 1, 0, 1);
                    }
                    return 0;
                }

                if (get_index0_num(T[i - x + now_turn_index][j - y], index0) == 1 && map[i + 1][2 * j - y] == 2)
                    reduce_time = 1;
            }
        }
        check();
        tx = x;
        ty = y;
    }
    if (show_next == 1 || show_next == 2)
    {
        x = temp_x;
        y = temp_y;
        tx = temp_tx;
        ty = temp_ty;
        now_turn = temp_now_turn;
    }
    return 1;
}

void check() //统计每层的得分情况
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    for (int i = 6; i < height; i++)
    {
        gotoxy(24, i);
        if (i != death_line)
            printf("[%02d]", check_sum[i][0] / 2);
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), now_color);
}

void manchange() //人为输入更新
{
    char ch, ch1, ch2;
    if (kbhit())
    {
        ch = getch();
        if (ch != ' ' && ch != 'w' && ch != 's' && ch != 'a' && ch != 'd' && ch != -32 && ch != 'p' && ch != 'b')
            return;
        if (ch == -32)
        { //同时启用wasd  和上下左右键
            ch1 = getch();
            switch (ch1)
            {
            case 72:
                ch = 'w';
                break;
            case 75:
                ch = 'a';
                break;
            case 80:
                ch = 's';
                break;
            case 77:
                ch = 'd';
                break;
            }
        }
        if (ch == 'a' || ch == 's' || ch == 'd' || ch == 'w' || ch == ' ' || ch == 'p' || ch == 'b')
        {
            switch (ch)
            {
            case 'a':
                y = y - 2, show(1, 1, 0, 1);
                break;
            case 'd':
                y = y + 2, show(1, 1, 0, 1);
                break;
            case 's':
                x++, show(1, 1, 0, 1);
                break;
            case 'w':
            {
                if_change = 1;
                old_index0 = index0;
                index0 = (index0 + 1) % 5;
                if (index0 == 0)
                    index0 = 1;
                show(1, 1, 0, 1);
                if_change = 0;
                break;
            }
            }
        }
    }
}
void autochange() //自动更新
{
    times--; //模拟sleep函数
    if (times == 0)
    {
        cnt_time++;
        times = 15000;
        x++;
        show(1, 1, 1, 1);
        reduce_time = 0;
        if (if_full == 1)
        {
            if_full = 0;
            delete_line();
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), now_color);
        if (reduce_time == 1)
            times = 8000;
        if (cnt_time == 20 && now_pass != 0)
        {
            cnt_time = 0;
            //是否开启死亡线下降： 0否   1是
            death_line_drop(0);
        }
    }
}
int main()
{
    int temp_pass = 1;
    system("mode con cols=50 lines=28");
    HideCursor();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    printf("******   游戏名：  俄罗斯方块Tetris Plus   ******\n");
    printf("******                                     ******\n");
    printf("******   程序作者： 404name                ******\n");
    printf("******                                     ******\n");
    printf("******   操作方式： ←左移动               ******\n");
    printf("******   操作方式： →右移动               ******\n");
    printf("******   操作方式： ↑上旋转               ******\n");
    printf("******   操作方式： ↓加速下降             ******\n");
    printf("******   操作方式： 回车确认键             ******\n");
    printf("******   建议游戏字号大小为26号            ******\n");
    getchar();
    gotoxy(0, 0);
    system("mode con cols=28 lines=28");
    system("color 07");
    gotoxy(0, 0);
    init(now_pass);
    while (1)
    {
        autochange();
        manchange();
    }
}