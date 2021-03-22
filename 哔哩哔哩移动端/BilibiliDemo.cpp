/*-----------------------------------
程序    ：   字符画开发小框架
作者    ：   404name / 绝伦N
完成时间：   2021.1.11
说明：这个程序不太好表达，后面考虑出一期教程，但他的功能十分强大，
	  就你不仅仅可以改写哔哩哔哩，其他软件你也只要写好组件然后用页面去调用就能显示。
	 个人改造可以只要 
	 数组介绍：
	 贯穿全代码的是一个index 的概念，就是第n个组件。
	 appMap负责存组件 取值就是一个index，用这个下标去appMapMessage里面拿这个组件的具体信息，位置
	 最后去appMapIron里面用index去取这个组件的字符画内容，有些是不能写死的就要格外设定需要函数去打印
	 
	  
	 appMap[20][12][7] : 存界面，表示有20个12*7的界面 这里设计很特别，需要格外说明
	 					可以先不懂动 
	 appMapMessage[100][5] 就是取到对应的内容的具体信息 
	 appMapIcon[100][1000]：存界面 
	  
	主要函数介绍 
	printDemo(index,bool);  html网页里面的鼠标移动到他上面和移开的概念，该函数会绘制两种状态。
	checkAction 就类似监听器，所以路由跳转全到那边设置 
	move函数就是移动 
-----------------------------------*/

#include <conio.h>  //getch()
#include <stdio.h>
#include <windows.h>
#define Is_Active_Color 115     //当前所在状态栏的颜色 浅蓝色灰底
#define Is_Selected1_Color 128  //被选中的颜色1 蓝色深灰底
#define Is_Selected2_Color 62   //被选中的颜色2 白色蓝底
#define Is_Selected3_Color \
    369  //被选中的颜色3 深蓝色灰色(解决一些字符不显示背景用)
#define Video_Color 3     //视频颜色 蓝色黑底
#define Normal_Color 112  //正常的颜色 灰底
#define Boder_Color 391   //边框的颜色 深灰底
const int WIDTH = 53;     //单位宽度
const int HEIGHT = 47;    //单位高度
const int L = 4;          //用户头像长宽
const int LL = 20;        //视频封面长度
const int N = 50;         //标题、用户名等最长长度
const int M = 1000000;    //视频收藏，发布，粉丝等上限
/*appMap是内容映射，取到内容数值对应下面的具体信息
0边界 1：头像(点击登录) 2搜索框 3信息
4 5 6 7 8 直播/推荐/热门/追番/影视
9-14 6个视频块
15-19 首页/推荐/动态/会员购/我的
20 404页面  */
/*全局变量
nowPage 0 1 2 3 404页面/首页/热搜/我的/搜索页面/待更新
*/
int nowPage = 1;
bool onPlay = false;   //关闭则是杀死
bool stopPlay = true;  //开启则是停止
int activeIndexBottom = 15;  //当前响应的页面(顶部
int activeIndexTop = 5;      //当前响应的页面(底部
char bacground[48][56] = {
    "                                                     ",
    "                                                     ",
    "              你 感 兴 趣 的 视 频                   ",
    "                                                     ",
    "                    都 在 B 站                       ",
    "                  r.                                 ",
    "                 vPvi                                ",
    "                   Qq                                ",
    "               .iirj77JYLr:                          ",
    "             iY1Lvrirrr77Ls1i        ::::.           ",
    "            5J7r7rrirrri7iir1v    ..iui.     7M:     ",
    "           Z7rrrrir7i:rivZuiiJv.....   ..::  BBBB:   ",
    "          2PrirrrrLb7iri:vMb7i1:    .   . .:v7 vu.   ",
    "          QjisrrrduidS72uvririrv  ..   . ...2r  . :  ",
    "          QUrLri7Br  :vEi:BBbisDL ..  :    ...U:  .: ",
    "          sQu7irr2 .v.    BZrr1..   .L2.:    rB    : ",
    "          .MP:irriuQi.      :bPu::::i  :v     i   .: ",
    "           BY7v:i25:   :r:  21.B5   UBBUr .. :v  .:  ",
    "          vQEMQPjKD5i  :r.iBPr      iBR7  .  :u. .:  ",
    "         :EBBgbMZ. :vJrMB7  rj:       ...  ri.7rrr.  ",
    "       is i.    jr  . ..i   r:7Xv..  .r:..sJ5Ksv17:  ",
    "     vSu.    ir1Bb77v::ru. .    :iUBBB  UZi..: i..   ",
    ".     Yv:. 7BQBBBXq2IUqSKdi:ir   :7PB7  qE.   :. ::  ",
    ":gBBBBQgE22QQQBBii. .Rir: :s.   Prsd.. r.     .:::.  ",
    "  uBBBBQQgQQBDP:     i.r.:  ....i  :   i.       .    ",
    "    .r7YKQBRs:i.    :  BBBB2. .... u   i.            ",
    "       sBBd:ii .ii:rL. 2uUui      .r.  :S.           ",
    "       :..  .  .i:r7:r:           :v7i :ZS.          ",
    "             r... .:ii:           r:.:..  :          ",
    "           .BBB.   :. .          .        ..         ",
    "           dBjBB5bBBgKq           Ji .r   .          ",
    "               qQBBBQBB          .BB:RIidB:          ",
    "                :BBBBQB          :BQBBBBBB           ",
    "                   rBBB           1bJ BBBi           ",
    "                    BBB               BBB            ",
    "                   :BMQ               QBQ            ",
    "                    BRB7             jBBJ            ",
    "                     QMB.            BBB             ",
    "                      QRBQg2      qQBBBr             ",
    "                      .s7:.        .:7i              ",
    "                ____  _ _ _ _     _ _ _ 	   	      ",
    "               |  _ \\(_) (_) |   (_) (_)		       ",
    "               | |_) |_| |_| |__  _| |_ 		      ",
    "               |  _ <| | | | '_ \\| | | |		       ",
    "               | |_) | | | | |_) | | | |		      ",
    "               |____/|_|_|_|_.__/|_|_|_|		      ",
    "                                                     ",
};
//三维依次是   哪个界面 界面哪行  某行第几个模块
// appMap[x][0][0]表示该页面有几行
int appMap[20][12][7] = {{{5, 0, 0, 0, 0, 0, 0},  // 0.404报错页面
                          {0, 1, 2, 2, 2, 3, 0},
                          {0, 20, 20, 20, 20, 20, 0},
                          {0, 15, 16, 17, 18, 19, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{8, 0, 0, 0, 0, 0, 0},  // 1.首页
                          {0, 1, 2, 2, 2, 3, 0},
                          {0, 4, 5, 6, 7, 8, 0},
                          {0, 9, 9, 10, 10, 10, 0},
                          {0, 11, 11, 12, 12, 12, 0},
                          {0, 13, 13, 14, 14, 14, 0},
                          {0, 15, 16, 17, 18, 19, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{10, 0, 0, 0, 0, 0, 0},  // 2.热门 (待更新 )
                          {0, 1, 2, 2, 2, 3, 0},
                          {0, 4, 5, 6, 7, 8, 0},
                          {0, 25, 25, 26, 27, 28, 0},
                          {0, 21, 21, 21, 21, 21, 0},
                          {0, 22, 22, 22, 22, 22, 0},
                          {0, 23, 23, 23, 23, 23, 0},
                          {0, 24, 24, 24, 24, 24, 0},
                          {0, 15, 16, 17, 18, 19, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{8, 0, 0, 0, 0, 0, 0},  // 3.我的(待更新 )
                          {0, 1, 2, 2, 2, 3, 0},
                          {0, 4, 5, 6, 7, 8, 0},
                          {0, 9, 9, 10, 10, 10, 0},
                          {0, 11, 11, 12, 12, 12, 0},
                          {0, 13, 13, 14, 14, 14, 0},
                          {0, 15, 16, 17, 18, 19, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{9, 0, 0, 0, 0, 0, 0},  // 4.视频详细页(简介)
                          {0, 29, 29, 29, 29, 29, 0},
                          {0, 30, 31, 32, 32, 33, 0},
                          {0, 34, 34, 34, 34, 35, 0},
                          {0, 36, 36, 36, 36, 36, 0},
                          {0, 37, 38, 39, 40, 41, 0},
                          {0, 42, 42, 42, 42, 42, 0},
                          {0, 43, 43, 43, 43, 43, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{8, 0, 0, 0, 0, 0, 0},  // 5.消息页面 (待更新 )
                          {0, 1, 2, 2, 2, 3, 0},
                          {0, 25, 25, 26, 27, 28, 0},
                          {0, 21, 21, 21, 21, 21, 0},
                          {0, 22, 22, 22, 22, 22, 0},
                          {0, 23, 23, 23, 23, 23, 0},
                          {0, 24, 24, 24, 24, 24, 0},
                          {0, 0, 0, 0, 0, 0, 0}}};
/*appMap取到的内容对应下面的下标，
信息依次是映射到实际打印处的 x/y/高/宽/类别
类别： 0 无功能项 点击进不去
           1 有功能项 点击进入action
           2 待开发
*/
int appMapMessage[100][5] = {
    {0, 0, 0, 0, 0},
    {1, 2, 2, 6, 1},  // 1-3 顶部信息栏 头像/搜索框/信息
    {1, 12, 2, 31, 1},
    {1, 43, 2, 8, 1},
    {4, 2, 1, 9, 1},   // 4-8 顶部标签功能栏
    {4, 12, 1, 9, 1},  //推荐
    {4, 22, 1, 9, 1},  //热门
    {4, 32, 1, 9, 1},  //
    {4, 42, 1, 9, 1},
    {6, 1, 11, 25, 1},  // 9-14 6个视频点位
    {6, 27, 11, 25, 1},
    {18, 1, 11, 25, 1},
    {18, 27, 11, 25, 1},
    {30, 1, 11, 25, 1},
    {30, 27, 11, 25, 1},
    {42, 2, 4, 9, 1},  // 15-19 底部标签功能栏
    {42, 12, 4, 9, 1},
    {42, 22, 4, 9, 1},
    {42, 32, 4, 9, 1},
    {42, 42, 4, 9, 1},
    {6, 2, 35, 49, 0},  // 20.404页面
    {10, 2, 7, 49, 1},  // 21-24. top热门榜页面
    {18, 2, 7, 49, 1},
    {26, 2, 7, 49, 1},
    {34, 2, 7, 49, 1},
    {6, 4, 4, 10, 1},  // 25-28 中间4格栏，热门 信息共用
    {6, 16, 4, 10, 1},
    {6, 27, 4, 10, 1},
    {6, 39, 4, 10, 1},
    {1, 1, 14, 51, 1},  // 29 视频模块
    {15, 7, 1, 4, 1},  // 30-33 视频下方功能 简介/评论/点我发弹幕/弹幕开关
    {15, 18, 1, 10, 1},
    {15, 32, 1, 15, 1},
    {15, 47, 1, 2, 1},
    {17, 1, 2, 40, 1},  // 34-36 视频下方消息栏 个人页/关注/视频标题+简介
    {17, 44, 2, 8, 1},
    {20, 1, 5, 51, 1},
    {26, 2, 4, 9, 1},  // 37-41 点赞/不喜欢/投币/收藏/转发
    {26, 12, 4, 9, 1},
    {26, 22, 4, 9, 1},
    {26, 32, 4, 9, 1},
    {26, 42, 4, 9, 1},
    {31, 2, 7, 49, 1},
    {39, 2, 7, 49, 1},  // 42-45 个人页面4个视频模块 和推荐公用
    {31, 2, 7, 49, 1},
    {39, 2, 7, 49, 1},  // 43-45视频下方广告 + 推荐视频
};
char appMapIcon[100][1000] = {
    " ",
    "[我是][头像]",  // 1-5
    "|o #########################  || \\                           |",
    "〖信息〗        ",
    "〖直播〗 ",
    "〖推荐〗 ",
    "〖热门〗 ",  // 6-10
    "〖追番〗 ",
    "〖影视〗 ",
    ".=======================."
    "|                       |"
    "|                       |"
    "|        暂无封面       |"
    "|                       |"
    "|                       |"
    "|__________________x:xx_|"
    "|肥肠抱歉，你要找的视频 |"
    "|不见了                 |"
    "|Up：xxxxxx          Ξ |"
    ".=======================.",
    ".=======================."
    "|                       |"
    "|                       |"
    "|        暂无封面       |"
    "|                       |"
    "|                       |"
    "|__________________x:xx_|"
    "|肥肠抱歉，你要找的视频 |"
    "|不见了                 |"
    "|Up：xxxxxx          Ξ |"
    ".=======================.",
    ".=======================."
    "|                       |"
    "|                       |"
    "|        暂无封面       |"
    "|                       |"
    "|                       |"
    "|__________________x:xx_|"
    "|肥肠抱歉，你要找的视频 |"
    "|不见了                 |"
    "|Up：xxxxxx          Ξ |"
    ".=======================.",  // 11-15
    ".=======================."
    "|                       |"
    "|                       |"
    "|        暂无封面       |"
    "|                       |"
    "|                       |"
    "|__________________x:xx_|"
    "|肥肠抱歉，你要找的视频 |"
    "|不见了                 |"
    "|Up：xxxxxx          Ξ |"
    ".=======================.",
    ".=======================."
    "|                       |"
    "|                       |"
    "|        暂无封面       |"
    "|                       |"
    "|                       |"
    "|__________________x:xx_|"
    "|肥肠抱歉，你要找的视频 |"
    "|不见了                 |"
    "|Up：xxxxxx          Ξ |"
    ".=======================.",
    ".=======================."
    "|                       |"
    "|                       |"
    "|        暂无封面       |"
    "|                       |"
    "|                       |"
    "|__________________x:xx_|"
    "|肥肠抱歉，你要找的视频 |"
    "|不见了                 |"
    "|Up：xxxxxx          Ξ |"
    ".=======================.",
    "  /-\\     │  │   └―┘    首页  ",
    "     O       O O           |   频道 ",  // 16-20
    "     ↗     ↖┼↘     ↙  |   动态 ",
    "   ︵      ╔─╗   ┗─┛ | 会员购 ",
    "   \\  /    ╔―╗   ┗―┛ |  我的  ",
    " ",
    "#################################################"
    "#                       #《原神》角色展示- [甘  #"
    "#                       #雨：梦里韶光]          #"
    "#                       #[5万点赞]              #"
    "#                       #[up] 原神              #"
    "#                       #[▲] 65.6 观看        :#"
    "#__________________2:06_#########################",  // 21-25
    "#################################################"
    "#                       #实在是妙! 2020年度百大 #"
    "#                       #UP主获奖感言第一弹...  #"
    "#                       #[5万点赞]              #"
    "#                       #[up] 哔哩哔哩UP主执事  #"
    "#                       #[▲] 54   观看        :#"
    "#__________________8:29_#########################",
    "#################################################"
    "#                       #这也叫少女  !戏里戏外  #"
    "#                       #大搞双标!硬核吐槽《... #"
    "#                       #                       #"
    "#                       #[up] 开心嘴炮          #"
    "#                       #[▲] 41.1  观看       :#"
    "#__________________9:11_#########################",
    "#################################################"
    "#                       #最牛气女学员扣1965分， #"
    "#                       #科三考试全程横冲直撞.. #"
    "#                       #                       #"
    "#                       #[up] 老实的刘教练说车  #"
    "#                       #[▲] 87.8  观看       :#"
    "#__________________1:13_#########################",
    "    ■      ■■      ■■■    排行榜  ",
    "   \\︵/      (☆)       ︶     入站必刷 ",  // 26-30
    " +------+  |::  三|  +------+  每周必看 ",
    " 、 ，     ╔―╗╗  ┗―┛┛  年度动画 ",
    " ",
    "简介",
    "评论00000",  // 31-35
    "( 点我发弹幕 |",
    "√ )",  //×
    " [我是] 绝伦N                            [头像] xxxx粉丝                  "
    "      ",
    "┌----┐└+关注 ",
    " 【标题填充】 bad apple原版高清1440*1080         ^ "
    " -----------------------------------------         "
    " {▲}[25.5万] {三}[2321] 1-12 BV1Wx411c7JT         "
    " Θ未经作者授权禁止转载                            "
    " ------------------------------------------------- ",  // 36-40
    "   ┓     ┏┫┻┓ ┗┻━┛  点赞   ",
    " ┏┏━┓ ┗┣┳┛   ┛     不喜欢  ",
    "  ︵      (币)      ︶       投币   ",
    "   ☆     ☆☆☆    ☆☆     收藏   ",
    "   _◣     (/◤              分享   ",  // 41-45
    " ",
    " ",
    "#################################################"
    "#                       #正确护肤，纵享水润滑肌 #"
    "#                       #                       #"
    "#                       #                       #"
    "#                       #[up]                   #"
    "#                       #[▲] 87.8  观看       :#"
    "#################################################",
    "#################################################"
    "#                       #(東京嗆種Tokyo Ghoul)  #"
    "#                       #Unravel-郑晟河-指弹..  #"
    "#                       #                       #"
    "#                       #[up] 郑晟河            #"
    "#                       #[▲] 87.8  观看       :#"
    "###################4:26##########################",
    " ",  // 46-50
    " ",
    " ",
    " ",
    "#################################################"
    "# [我是]  绝伦N lv6 UP                          #"
    "# [头像]                                        #"
    "#         肥肠抱歉，暂时没有评论                #"
    "#         ######################                #"
    "#         共00000条回复 >                      :#"
    "#################################################",
    "#################################################"
    "# [头像]  绝伦N lv6 UP                          #"
    "# [一个]                                        #"
    "#         肥肠抱歉，暂时没有评论                #"
    "#         ######################                #"
    "#         共00000条回复 >                      :#"
    "#################################################",  // 51-55
    "#################################################"
    "# [头像]  绝伦N lv6 UP                          #"
    "# [一个]                                        #"
    "#         肥肠抱歉，暂时没有评论                #"
    "#         ######################                #"
    "#         共00000条回复 >                      :#"
    "#################################################",
    "#################################################"
    "# [发条友善的评论                     ]     0v0 #"
    "#################################################",
    " 热门评论                               Ξ按时间 ",
    " ",
    " ",  // 56-60
    " ",
    " ",
    " ",
    " ",
};
//结构体设计
struct User {
    int uid;           // b站uid
    char head[L * L];  //头像
    char userName[N];  //用户名
    int coin;          //硬币数
    int lv;            // b站等级
    //下标0位是计数位，不存信息
    int concern[M];        //关注
    int follower[M];       //粉丝
    int myVideos[M];       //我的视频
    int collectVideos[M];  //我收藏的视频
};

struct Vedio {
    int vid;              //视频唯一编号
    char title[N];        //标题
    char intro[2 * N];    //简介
    char cover[LL * LL];  //封面
    //视频统计量/观看/点赞/投币/收藏/
    int viewNum, likeNum, coinNum, collectNum;
    char viewTime[N];  //#播放时长
    char comment[M];   //#评论
};
//一些系统功能函数
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
void fixedWindow() {                 //可以忽略
    HWND hWnd = GetConsoleWindow();  //获得cmd窗口句柄
    RECT rc;
    GetWindowRect(hWnd, &rc);  //获得cmd窗口对应矩形
    //改变cmd窗口风格
    SetWindowLongPtr(hWnd, GWL_STYLE,
                     GetWindowLong(hWnd, GWL_STYLE) & ~WS_THICKFRAME &
                         ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
    //因为风格涉及到边框改变，必须调用SetWindowPos，否则无效果
    SetWindowPos(hWnd, NULL, rc.left, rc.top, rc.right - rc.left,
                 rc.bottom - rc.top, NULL);
}
void setColor(short x)  //自定义函根据参数改变颜色
{
    // if (x >= 0 && x <= 15) //参数在0-15的范围颜色
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            x);  //只有一个参数，改变字体颜色
    /* else                              //默认的颜色白色
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);*/
}
//一些系统功能函数

void debug() {
    static int x = 0, y = 0;
    static char flag = '#';
    //按下数字就会切换到对应的东西打印方便画画
    char print[11] = " !@#$%^&*(";
    int nextgo[4][2] = {-1, 0, 0, 1, 1, 0, 0, -1};  //顺时针wdsa下一步的移动
    int bookMap[128] = {0};
    bookMap['w'] = 0, bookMap['d'] = 1;
    bookMap['s'] = 2, bookMap['a'] = 3;
    gotoxy(y, x), printf("%c", flag);
    char turn = _getch();
    if (turn == ' ')
        system("cls");
    else if (turn >= '0' && turn <= '9')
        flag = print[turn - '0'];
    else {
        x += nextgo[bookMap[turn]][0];
        y += nextgo[bookMap[turn]][1];
        gotoxy(y, x), printf("_");
        gotoxy(0, 20), printf("x=%d y=%d nowturn='%c'", x, y, flag);
    }
}
void printCuttingLine(int deep, char wall = '-') {  ///打印分割线
    for (int i = 1; i < WIDTH - 1; i++) {
        gotoxy(i, deep);
        printf("%c", wall);
    }
}
//打印组件函数区域：
void printBoder() {  //打印边框
    setColor(Boder_Color);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            gotoxy(j, i);
            if (i == 0 || i == HEIGHT - 1)
                printf("#");
            else if (j == 0 || j == WIDTH - 1)
                printf("#");
        }
    }
    setColor(Normal_Color);
        //打印page内置分割线
    int mapHeight = appMap[nowPage][0][0];  //获取地图高度
    int cuttingLine =
        appMap[nowPage][mapHeight][0];  //再最后一行0位获取分割线个数
    for (int i = 0; i < cuttingLine; i++) {
        printCuttingLine(appMap[nowPage][mapHeight][i + 1]);
    }
}

//万能字符打印模块
void printDemo(int index, bool on = false) {
    int x = appMapMessage[index][0];
    int y = appMapMessage[index][1];
    int h = appMapMessage[index][2];
    int w = appMapMessage[index][3];
    int turn = appMapMessage[index][4];  //获取填写图标状态
    char prePrint[h + 1][WIDTH];
    int cnt = 0;
    if (turn) {
        for (int i = 0; i < h; i++) {
            strncpy(prePrint[i], appMapIcon[index] + w * i, w);
            prePrint[i][w] = '\0';
            gotoxy(y, x + i);
            printf("%s", prePrint[i]);
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            gotoxy(y + j, x + i);
            if (!on) {  //未被选中状态
                if (!turn) {
                    printf("#");
                }  //什么都没有的内容页
                else if (turn == 1 &&
                         (activeIndexTop == index ||
                          activeIndexBottom == index) &&
                         j == 0) {
                    gotoxy(y + j, x + i);
                    setColor(Is_Active_Color);
                    printf("%s", prePrint[i]);
                    setColor(Normal_Color);
                }
                // printf("%c",str[index++]);
            } else {
                if (i == 0 || i == h - 1 || j == 0 || j == w - 1) {
                    //如果当前是首页推荐标签，那么推荐标签应该是常亮的active状态
                    //后面则是选中状态
                    if (turn == 1 && j == 0) {
                        gotoxy(y + j, x + i);
                        if (index >= 9 && index <= 14 ||
                            index >= 21 && index <= 24 ||
                            index >= 43 && index <= 45) {
                            setColor(Video_Color);
                        }  //视频的样式
                        else if (index >= 15 && index <= 19 ||
                                 index >= 25 && index <= 28 ||
                                 index >= 37 && index <= 41 || index == 35) {
                            setColor(Is_Selected3_Color);
                        }  //一些错位字符的样式
                        else
                            setColor(Is_Selected1_Color);  //正常样式
                        printf("%s", prePrint[i]);
                        // printf("%c",str[index++]);
                    }else if (turn == 0) {
                    setColor(Is_Selected2_Color);
                    printf("O");
                	}
                } 
	            else {
	                if (!turn)
	                    printf("#");
	            }
	            setColor(Normal_Color);
            }
        }
    }
    if (!on && !turn) {
        gotoxy(20, 20);
        printf("肥 肠 抱 歉 ");
        gotoxy(14, 22);
        printf("你 要 找 的 页 面 不 见 了");
        gotoxy(17, 24);
        printf("作 者 正 在 开 发 中");
        gotoxy(17, 26);
        printf("按 q 返 回 上 一 页");
    }
}
void clsPage() {
    //此处写不清除边框代码
    system("cls");
}
void loadPage() {         //加载首页菜单
    int book[100] = {0};  //伪预加载，加载过的界面就不再加载
    printBoder();
    int h = appMap[nowPage][0][0] - 2;  //减去上下墙体
    int w = 5;
    for (int i = 1; i <= h; i++) {
        for (int j = 1; j <= w; j++) {
            int index = appMap[nowPage][i][j];  //获取对应模块的对应值
            if (!book[index])
                printDemo(index, false);
            book[index] = 1;
        }
    }
}

void checkAction(int action) {  //引入路由跳转
    if (action == 3) {          //消息栏
        nowPage = 5;
    } else if (action == 5) {  //首页
        nowPage = 1;
    } else if (action == 6) {  //热门
        nowPage = 2;
    } else if ((action >= 9 && action <= 14) ||
               (action >= 21 && action <= 24)) {
        onPlay = true;
        nowPage = 4;                            //进入视频详细页面
    } else if (action >= 15 && action <= 19) {  //底层标签栏
        if (action == 15) {
            activeIndexBottom = 15;  //响应变成首页
            activeIndexTop = 5;
            nowPage = 1;  //没写界面先放首页 后面就nowPage要对应内容
        } else
            nowPage = 0;
    } else if (action == 29) {  //点击视频模块  则对应停止视频
        if (!stopPlay)
            stopPlay = true;
        else
            stopPlay = false;
    } else {
        nowPage = 0;  // 没写的界面404
    }
    activeIndexTop = action;  //判断完再切回来
}

void move() {
    //处理初始数据
    static int x = 1, y = 1;
    static int oldPage = nowPage;
    int oldX = x, oldY = y;
    int nextgo[5][2] = {0, 0, -1, 0, 0,
                        1, 1, 0,  0, -1};  //顺时针wdsa下一步的移动
    int bookMap[128] = {0};
    bookMap['w'] = 1, bookMap['d'] = 2;
    bookMap['s'] = 3, bookMap['a'] = 4;
    //取到当前位置
    //一个index下标位置就存储了一个action
    int index = appMap[nowPage][x][y];
    int oldIndex = index;
    printDemo(index, false);  //回退上次的位置
    char turn = _getch();
    x += nextgo[bookMap[turn]][0];
    y += nextgo[bookMap[turn]][1];
    index = appMap[nowPage][x][y];
    if (index == 0) {  //如果是边界则回退
        x = oldX, y = oldY;
    } else {  //负责就移动到下一个新的位置
        while (oldIndex == index && index != 0 && bookMap[turn] != 0) {
            x += nextgo[bookMap[turn]][0];
            y += nextgo[bookMap[turn]][1];
            index = appMap[nowPage][x][y];
        }  // while就是跳过占多个坐标的状态
        if (index == 0)
            x = oldX, y = oldY;
    }
    index = appMap[nowPage][x][y];
    printDemo(index, true);  //打印新的状态
    if (turn == ' ' || turn == 'q') {
        if (turn == ' ') {
            checkAction(index);
        } else {
            onPlay = false;
            stopPlay = true;
            nowPage = 1;  //回到首页
        }

        if (nowPage != oldPage) {  //界面变化
            oldPage = nowPage;
            x = 1;
            y = 1;
            clsPage();   //除了边框全部擦除
            loadPage();  //加载新界面
        }
    }
}
void playVideo() { //为对接需要本地文件 
    int w = 51;             //宽度
    int h = 14;             //高度
    int PAGE_LIMIT = 4023;  //帧数
    int N = w * h;          //字符数
    char str[N];
    char name[] = "badapple/ASCII-序列 010001.txt";  // 13 14 15 16位
    int imageNum, tempNum, n = 1, p, q;
    //帧操作
    for (imageNum = 1; imageNum <= PAGE_LIMIT; imageNum++) {
        if (!onPlay)
            return;      //杀死线程则返回
        if (stopPlay) {  //停止播放则暂停
            if (imageNum > 0)
                imageNum--;
        } else {
            int tempNum = imageNum;
            for (p = 0; p < 4; p++) {
                int num = tempNum % 10;  //获取第p+1位
                tempNum /= 10;
                name[25 - p] = num + '0';
            }
            FILE* fp;
            //打开文本
            if ((fp = fopen(name, "r")) == NULL) {
                printf("%d", imageNum);
                system("pause");
                continue;
            }
            //读取文本
            int line = 1;
            while (fgets(str, N, fp) != NULL) {
                gotoxy(1, line++);
                printf("%s", str);
            }
            //关闭文本
            fclose(fp);
        }
        int timeLimit = 1000;
        while (timeLimit--) {
            if (kbhit())
                move();
        }
        // system("cls");
    }
}
void init() {
    char cmd[50];
    sprintf(cmd, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
    printf("%s", cmd);
    system(cmd);         //设置界面大小
    system("color 07");  //设置界面颜色。
    fixedWindow();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            gotoxy(j, i);
            printf("%c", bacground[i][j]);
        }
    }
    printBoder();
    getchar();
    system("cls");
    loadPage();
    HideCursor();
}

int main() {
    // ctrl+滚轮可以调大界面
    init();
    while (1) {
        if (onPlay) {
            playVideo();
        }
        if (kbhit()) {
            // debug(); //开启debug就会显示坐标信息，并且注释move()
            move();
        }
    }
}
// 7  - 41
