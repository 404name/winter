/*--------------------------------------------------------------------
程序： C语言 漂流瓶
作者： 404name | 绝伦N
说明:  功能预览版 | 可自定义数据
自定义方法：
    ·增添数据：
        往结构体18行结构体后面添加相同规格数据即可
        之后系统会自动汇总数据处理页面，并进行分页。
        无需进行更多操作
    ·更改界面：
        自己读懂逻辑后更改相对应的属性即可
    ·属性介绍：
        User 仅限10个以内字符(5个汉字/10英文字符)
        Datetime 2020-03-05 04:11:12日期的长度
                 并且C语言版本不支持自动设置时间
                 有能力可以自己调用函数试试
        Message  108个汉字的文字内容，超出范围系统会默认在后面打上省略号
版本及时间： ver 1.0.0  2020-06-10 20:20:12
--------------------------------------------------------------------*/
#include <conio.h> //用于getch（）
#include <stdio.h>
#include <stdlib.h> //用于rand
#include <string.h>
#include <windows.h>
#define width 64
#define height 22
#define page_include 6
const char block1[3] = "█";
const char block2[3] = "▓";
const char point[4][3] = {{"/"}, {"¤"}, {" "}, {"  "}};

struct bottle
{
    char User[11];
    char Datetime[30];
    char Message[500];
} bottles[200] = {
    {"讨厌这世界", "2020-06-08 17:39:43", "活着，就是选择一种死去的旅途。"},
    {"镜心女神", "2020-05-31 02:32:56", "今年15了，还是有很多事达不到。不会聊天、不会道歉、不够勇敢、不够努力、还是那样自卑"},
    {"嗨皮农夫", "2020-05-20 19:53:16", "我想学好英语，我想出去看一看，我想去看看国外是不是真的有大教堂？那有多大？国外是不是真的有那么美的大草原？国外是不是真的像电影那样？都是一个一个静谧的小镇，所以加油，希望七八年后的我会在国外发一张帖子，在树洞网！加油！为了自己的愿望，为了那么美的风景，为了我喜欢的外国影视及明星（奥黛丽-赫本与凯特-温斯莱特）！"},
    {"虞姬", "2020-05-06 01:32:07", "因为想忘记他，所以找了个替代品，但终究只是替代品，没有可比性，算是精神上的寄托和慰籍，爱而不得，真的很无奈也很煎熬！…"},
    {"嫦娥", "2020-04-13 14:26:05", "自愈是一件很困难的事…"},
    {"嫦娥", "2020-04-13 14:27:05", "我喜欢你…"},
    {"花花好好的", "2020-04-14 15:24:05", "希望大家都能好好的或者，毕竟这个世界很美好！抱着沙发 睡眼昏花 凌乱头发渴望像电影主角一样潇洒"},
    {"88888", "2020-04-15 14:27:05", "2020-04-15 14:27:05"},
    {"10086", "2020-05-15 12:27:05", "尊敬的客户，您好,5月账单已经出来了,请及时缴费喔"},
    {"电竞食堂", "2020-02-15 15:27:03", "每一张卡牌，都是对你的一份思念，我赌赢了所有，却赢不回你的爱"},
    {"刀锋意志", "2020-03-15 14:27:05", "均衡存乎万物之间。 我的剑刃愿为你效劳。 "},
    {"思念", "2020-02-25 02:24:25", "啊……夜空绽放的烟花 点燃每一个盛夏 总让我莫名伤感 时光像风儿一样 悄悄地消失流淌 晨光映彩霞 夕阳茜风中 你牵着我 走在熟悉的街旁 直到尽头 依依不舍地紧紧相拥 与你在这最后的夏天 抹不去的思念 斜阳里的微笑 渐行渐远 六月的微风吹散你的泪光 深深地铭刻心间 难以忘却 与你许下这最后诺言微笑着离 "},
    {"我好想你", "2020-01-03 05:21:05", "在秒速5厘米间，他们发现了彼此。也在秒速5厘米之间，他们错过了彼此。"},
    {"111dw11", "2020-03-05 04:11:12", "希望在明年"},
    {"树洞先生", "2020-06-09 23:51:12", "作为一个山东人，我特别讨厌山东的官僚作风。大学毕业坚决不回山东。农民掏钱才能买的起质量不高的楼房，与民争利。自小接触的基层干部，干实事的少。山东，现在可能在我内心就是出生地了吧，没有多少意义，可能以后也很少回去了。带着家人好好的在别处生活吧。 "},
    {"时光", "2020-06-09 20:02:25", "人间烟火是什么的呢？ 市集、小巷、美食.....香喷喷的烧烤，麻辣烫，凉皮凉面，卤味"},
    {"14449", "2020-06-08 16:00:11", "我想好好地活下去，因为我现在过得不好，所以我想活下去，我想把生活过得好一点，只是一点就好。 "},
    {"小先生", "2020-06-08 14:05:21", "也许，这就是人生吧"},
    {"一位朋友", "2012-01-22 23:41:48", "开开心心的吃完年夜饭 回家因为一个亲戚的拜年电话 家里吵了起来 一个人站在阳台上边看烟花边掉泪 冷到不行 最后被爸硬拉回房 到现在还不停的颤 想说 2012这样的日子 能不能一天都不要过 "},
    {"新浪匿名", "2012-01-22 22:21:45", "尼玛的补课喜欢上一个男的，靠，重点是他妹的我也是男的好嘛。看到他露出来的脚踝我要疯了好么。性格也很好，和老师开玩笑来着。声音也很好听。身高什么的更无话可说啊。烦死了。 "},
    {"????", "2015-03-25 22:20:15", "什么叫快乐?就是掩饰自己的悲伤对每个人微笑"},
    {"匿名网友", "2010-04-21 21:21:10", "你是gg还是mm啊"},
    {"卢本伟", "2019-02-15 20:05:05", "我！卢本伟！没有开挂!"},
    {"绝伦N", "2020-06-10 18:57:21", "啊，爬数据累坏了，早点爬完数据呀~~~"},
    {"漂流瓶", "2017-02-15 14:05:05", "感谢你收到了这个漂流瓶，现在大部分人都忘记了漂流瓶了，而屏幕前的你还在看着这个捞上来的消息，非常感谢你...希望这个漂流瓶能承载着过去的岁月一直漂流吧"},
    {"小迷妹", "2018-01-10 12:04:05", "我真的喜欢你，但我知道我配不上，看着你远去我又好心酸，真的，，真的。。。。。。。。。。。。。。。"},
    {"高考加油", "2020-06-08 14:50:21", "我要去你的大学和你相遇！！！！！！"},
    {"时光", "2017-05-07 12:35:21", "谁又记得曾经的我们呢，是这个漂流瓶？笑话，曾经的我们早就不在了！要谁去纪念呢？"},
    {"Java", "2018-06-05 13:25:51", "我是世界上最好的语言！"},
    {"awsl", "2019-06-08 21:25:51", "时间不会把我们冲淡，但距离会"},
    {"西瓜", "2017-11-18 01:25:05", "我看着你远去，不明白为什么你要去那遥远的未来呢？"},
    {"沙比", "2014-10-24 12:24:05", "我是sb，我是sb，我是sb，，我是sb，我是sb，我是sb，我是sb，我是sb，我是sb，我是sb，我是sb，我是sb，我是sb，"},
    {"锤子", "2020-05-01 15:20:51", "啊啊啊啊，我受不了了，辣鸡网课，学不下去了"},
    {"比利比利", "2020-06-10 14:22:20", "干杯"},
    {"啊啊啊啊", "2020-06-01 12:00:21", "儿童节快乐呀"},
}; //往后追加数据即可

char keymap[10][7] = { //对应光标去去处
    {"xxxxxx"},
    {"x0000x"},
    {"x1111x"},
    {"x2222x"},
    {"x3333x"},
    {"x4444x"},
    {"x5555x"},
    {"x6789x"},
    {"xxxxxx"}};
int keymap_to_map0[10][2] = { //keymap映射路劲    +3对应输出文字的地方
    {14, 5},                  //0
    {14, 7},
    {14, 9}, //2
    {14, 11},
    {14, 13}, //4
    {14, 15},
    {8, 19}, //6
    {20, 19},
    {34, 19}, //8
    {47, 19}};
int tx, ty, now_x, now_y, function, point_flag;
int page, All_messages, All_pages;
int menu_flag;                // 0加载动画， 1主菜单  2留言面板  3留言面板查看信息   返回键依次回溯
const char mapMaker[8][3] = { //界面采用逻辑构建这里是材料 也能复制界面用数组存着打印
    {"/"},                    //0
    {"▔"},                    //1
    {"▏"},                    //2
    {"█"},                    //3
    {"\\"},                   //4
    {"▁"}};                   //5
const char title[10][50] = {  //偶数主菜单，奇数附页面
    {"|信息|User:            Datetime:           "},
    {"|瓶子|404name          Datetime:           "},
    {"      这是首页喔~ 欢迎你的使用             "},
    {"                                           "},
    {"      漂流瓶  ver 1.0.0                    "},
    {"                                           "},
    {"      采用C语言搭建                        "},
    {"                                           "},
    {"      欢迎你的使用                         "}};
const char menu[8][12] = { //偶数主菜单，奇数附页面
    {"Θ===="},
    {"Θ返回"},
    {"¤扔一个"},
    {"←上一页"},
    {"◎捞捞看"},
    {"→下一页"},
    {"⊕我的瓶子"},
    {"⊕页码：01"}};
void HideCursor(int x) //光标隐藏
{
    CONSOLE_CURSOR_INFO cursor_info = {1, x}; //后边的0代表光标不可见
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

void change_menu(int turn) //更改菜单的功能按钮显示  进入不同页面显示不同功能
{
    int flag = 0, i;
    if (turn == 1)
        flag = 0;
    else
        flag = 1;
    for (i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            gotoxy(11, 2);
            printf("%s", title[i]);
        }
        else
        {
            if (i > 1)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 5);
            gotoxy(11, 1 + 2 * i);
            printf("%s", title[i]);
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 0);
    for (i = 6; i <= 9; i++)
    {
        tx = keymap_to_map0[i][0];
        ty = keymap_to_map0[i][1];
        gotoxy(tx, ty - 1);
        printf("%s", menu[(i - 6) * 2 + flag]);
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 1);
}

void init() //总init
{
    //初始化消息数目与页数处理
    int i, j;
    for (i = 0;; i++)
    {
        if (strlen(bottles[i].Datetime) == 0)
            break;
    }
    All_messages = i;
    page = 0;
    All_pages = (All_messages % page_include == 0 ? All_messages / page_include : All_messages / page_include + 1);
    //处理光标显示
    HideCursor(0);
    point_flag = 0; //光标形态
    now_x = 7, now_y = 1;
    //HideCursor(); //隐藏光标
    menu_flag = 1;
    //逻辑构建地图    也可以复制下来存字符串去打印但需要先for后面打个断点再复制页面
    for (i = 0; i < height; i++)
    {
        for (j = 0; j <= width; j++)
        {
            if ((i > 0 && i < height - 1 && (j == 0 || j == width)) || (i > 1 && i < height - 2 && (j == 2 || j == width - 2)))
            {
                gotoxy(j, i);
                printf("%s", mapMaker[2]);
            }
            if ((i == 0 && j == 0) || (i == 1 && j == 2) || (i == height - 1 && j == width - 1) || (i == height - 2 && j == width - 3))
            {
                gotoxy(j, i);
                printf("%s", mapMaker[0]);
            }
            if ((i == 0 && j == width - 1) || (i == 1 && j == width - 3) || (i == height - 1 && j == 0) || (i == height - 2 && j == 2))
            {
                gotoxy(j, i);
                printf("%s", mapMaker[4]);
            }
            if ((i == 0 && j < width - 1 && j % 2 == 1) || (i == 1 && j >= 3 && j < width - 3 && j % 2 == 1))
            {
                gotoxy(j, i);
                printf("%s", mapMaker[1]);
            }
            else if ((i == height - 1 && j < width - 1 && j % 2 == 1) || (i == height - 2 && j >= 3 && j < width - 3 && j % 2 == 1))
            {
                gotoxy(j, i);
                printf("%s", mapMaker[5]);
            }
            else if (i >= 2 && i <= 16)
            {
                if (j == 8 || j == width - 9)
                {
                    gotoxy(j, i);
                    printf("%s", mapMaker[3]);
                }
                else if (i > 2 && i % 2 == 0 && j >= 16 && j <= width - 12 && j % 2 == 1)
                {
                    gotoxy(j, i);
                    printf("%s", mapMaker[1]);
                }
            }
        }
        Sleep(50);
    }
    gotoxy(4, height - 3);
    printf("      ▔▔        ▔▔▔        ▔▔▔       ▔▔▔▔    ");
    //构建地图结束点

    //显示初始的界面选项3功能
    change_menu(menu_flag);

    //定位初始光标指针
    function = keymap[now_x][now_y] - '0';
    tx = keymap_to_map0[function][0];
    ty = keymap_to_map0[function][1];
    gotoxy(tx, ty);
    printf("%s", point[point_flag]);
    function = 0;
}

int move(int x, int y, int limit_x1, int limit_x2, int limit_y1, int limit_y2, int xx, int yy)
{
    //参数：开始坐标x y  限制活动空间 x左右  y上下 移动距离 x y
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 0);
    int last_x = x, last_y = y;
    function = keymap[x][y] - '0';
    ;
    tx = keymap_to_map0[function][0];
    ty = keymap_to_map0[function][1];
    gotoxy(tx, ty);
    //printf("%s",point);
    printf("%s", point[point_flag]);
    char ch = '\0', ch1;
    char res[20];
    while (ch != ' ')
    {
        ch = getch();
        gotoxy(tx, ty);
        printf("%s", point[point_flag + 2]); //输出空格消除
        if (ch == -32)
        { //同时启用wasd  和上下左右键   //如果读取到第一个是-32那么 继续读取一个字符
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
        switch (ch)
        {
        case 'w':
            x = x - xx;
            break;
        case 's':
            x = x + xx;
            break;
        case 'a':
            y = y - yy;
            break;
        case 'd':
            y = y + yy;
            break;
        case ' ':
            break;
        }
        if (x < limit_x1 || x > limit_x2 || y < limit_y1 || y > limit_y2 || (menu_flag != 2 && (ch == 'w' || ch == 's')))
        {
            x = last_x;
            y = last_y;
        }
        last_y = y, last_x = x;
        if (x == 7) //在x == 7的时候光标显示 == /
            point_flag = 0;
        else // 不然在x < 7的时候光标变成 O
            point_flag = 1;
        function = keymap[x][y] - '0';
        tx = keymap_to_map0[function][0];
        ty = keymap_to_map0[function][1];
        gotoxy(tx, ty);
        printf("%s", point[point_flag]);
    }
    if (x < 7)
        x = 7, y = 1;
    now_x = x, now_y = y; //返回x y
    gotoxy(tx, ty);
    printf("%s", point[point_flag + 2]); //输出空格消除
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 1);
    return function;
}

void show_message(int id) //显示第id条消息   难在显示输出的处理
{
    int i;
    gotoxy(17, 3);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 4);
    printf("%-17s%s", bottles[id].User, bottles[id].Datetime);
    int index = 0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 5);
    for (i = 0; i <= 5; i++)
    {
        tx = keymap_to_map0[i][0];
        ty = keymap_to_map0[i][1];
        gotoxy(tx + 3, ty);
        printf("                                     ");
    }
    if (id == -1) //假装没捞到
    {
        int flag = rand() % 4;
        gotoxy(17, 3);
        if (flag == 0)
            printf("哇，好大一只螃蟹。放生 或 再捞捞看  ");
        else if (flag == 1)
            printf("鱼儿水中游，来去乐悠悠。再捞捞看    ");
        else if (flag == 2)
            printf("捞到个锤子。放生 或 再捞捞看        ");
        else if (flag == 3)
            printf("捞到一个破瓶子，里面什么都没有。    ");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 1);
        return;
    }
    for (i = 0; i <= 5; i++)
    {
        tx = keymap_to_map0[i][0];
        ty = keymap_to_map0[i][1];
        gotoxy(tx + 3, ty);
        int cnt = 18 * 2;
        while (cnt-- && bottles[id].Message[index] != '\0')
        {
            if (i == 5 && cnt == 1)
            {
                printf("…");
                cnt--;
                break;
            }
            printf("%c", bottles[id].Message[index]);
            index++;
        }
        if (bottles[id].Message[index] == '\0' && cnt)
        {
            while (cnt--)
            {
                printf(" ");
            }
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 1);
}

void show_page(int page) //显示第几的页面
{
    int start = (page - 1) * page_include;
    int end = page * page_include;
    int i;
    if (end > All_messages)
        end = All_messages;
    int index = 0;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 5);
    for (i = 0; i <= 5, start < page * page_include; i++, start++)
    {
        tx = keymap_to_map0[i][0];
        ty = keymap_to_map0[i][1];
        gotoxy(tx + 3, ty);
        if (start < end)
            printf("%-17s%s", bottles[start].User, bottles[start].Datetime);
        else
            printf("                                     ");
    }
    tx = keymap_to_map0[9][0];
    ty = keymap_to_map0[9][1];
    gotoxy(tx + 8, ty - 1);
    printf("%02d", page);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 1);
}

void throw_bottle() //扔漂流瓶   == 增加功能   这里难在处理输入
{
    int i;
    HideCursor(1);
    for (i = 0; i <= 5; i++)
    {
        tx = keymap_to_map0[i][0];
        ty = keymap_to_map0[i][1];
        gotoxy(tx + 3, ty);
        printf("                                     ");
    }
    gotoxy(17, 3);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 4);
    printf("          ||在此输入不越线的用户名  ");
    char username[100];
    gotoxy(17, 3);
    scanf("%s", username);
    if (strlen(username) > 10)
    {
        gotoxy(17, 2);
        printf("用户名过长,错误输入，按回车退回       ");
        getchar();
        getchar();
        change_menu(menu_flag);
        return;
    }
    else
    {
        gotoxy(17, 2);
        printf("成功输入,%10s|                 ", username);
        gotoxy(17, 3);
        printf("文字输入请自行按回车换行             ");
    }
    int index = 0;
    char res[500] = "\0";
    char temp[50];
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 5);
    for (i = 0; i <= 5; i++)
    {
        getchar();
        tx = keymap_to_map0[i][0];
        ty = keymap_to_map0[i][1];
        gotoxy(tx + 3, ty);
        scanf("%[^\n]", &temp);
        strcat(res, temp);
    }
    strcpy(bottles[All_messages].User, username);
    strcpy(bottles[All_messages].Message, res);
    strcpy(bottles[All_messages].Datetime, "2020-06-08 17:39:43 ");
    All_messages++;
    All_pages = (All_messages % page_include == 0 ? All_messages / page_include : All_messages / page_include + 1);
    gotoxy(17, 2);
    printf("漂流瓶已扔入大海，回车返回");
    getchar();
    getchar();
    menu_flag = 1;
    change_menu(menu_flag);
    HideCursor(0);
}

void manchange() //人为输入更新
{
    char ch, ch1;
    if (kbhit())
    {
        function = move(now_x, now_y, 1, 7, 1, 4, 1, 1);

        if (function >= 0 && function <= 5) //查看留言面板第m页的第n+1条
        {
            menu_flag++;
            show_message((page - 1) * page_include + function); //取数据库第几条
        }
        if (function == 6) //返回
        {
            if (menu_flag <= 2)
            { //查看所有留言返回菜单
                menu_flag = 1;
                change_menu(menu_flag);
            }
            if (menu_flag == 3)
            { //所有留言查看详细  返回 留言
                menu_flag--;
                show_page(page);
            }
        }
        else if (function == 7)
        {
            if (menu_flag == 1)
            { //扔一个
                throw_bottle();
            }
            else
            { //上一页
                if (page > 1)
                {
                    menu_flag = 2;
                    show_page(--page);
                }
            }
        }
        else if (function == 8)
        {
            if (menu_flag == 1) //捞捞看
            {
                if (rand() % 3 == 1)
                    show_message(-1); //没捞到
                else
                    show_message(rand() % All_messages); //随机数
            }
            else
            { //下一页
                if (page < All_pages)
                {
                    menu_flag = 2;
                    show_page(++page);
                }
            }
        }
        else if (function == 9)
        {
            if (menu_flag == 1)
            {
                menu_flag++;
                change_menu(menu_flag);
                page = 1;
                show_page(1);
            }
        }
        function = keymap[now_x][now_y] - '0';
        tx = keymap_to_map0[function][0];
        ty = keymap_to_map0[function][1];
        gotoxy(tx, ty);
        printf("%s", point[point_flag]);
    }
}
void autochange() //自动更新
{
    //无
}
int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10 | 1);
    //SetConsoleTextAttribute
    //设置控制台窗口字体颜色和背景色的
    init();
    while (1)
    {
        autochange();
        manchange();
    }
}

