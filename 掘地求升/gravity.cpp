/*-------------------------------------。
程序名字：掘地求升（C语言版）
程序作者：CTGU_LLZ
完成时间：2020/1/15-2020/1/26
--------------------------------------*/
//  goto xy 对应 xy  而map对应map【y】[x]
#include <conio.h> //用于getch（）；
#include <stdio.h>
#include <string.h>
#include <windows.h>
#define people 'M' //光标加M有点像一只小兔子
#define link '*'
#define hammer 'O'
#define load_speed 10     // 0-10   飞速    30-50等待速度
void gotoxy(int x, int y) //坐标函数
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
struct pass_information //关卡结构体 包括出生点  下一关传送点  地图
{
    int start_x;
    int start_y;
    int next_x;
    int next_y;
    char map[50][80];
} s[20] = {
    {4,
     6,
     56,
     13,
     {{"███████████████████████████████"},
      {"■向上跳的同时切换方向■Tip： 按键R重新开始本关卡           ■"},
      {"■是游戏的最重要的技巧■      按键F复位防止卡死             ■"},
      {"■翻越一定是向上起手  ■      按键N next下一关（设置开启）  ■"},
      {"■                    ■      按键B back上一关（设置开启）  ■"},
      {"■                    ■      按键P 返回菜单                ■"},
      {"■                    ■      触碰⊙坠落     ⊙为关卡衔接   ■"},
      {"■                    ■      触碰 进入下一关              "
       "■"},
      {"■                    ■■■■■■■■■■■■■■■■■■■■"},
      {"■                                                          ■"},
      {"■                                                          ■"},
      {"■■■■■                                                  ■"},
      {"■              ■■■        ■■■     ■■■             ■"},
      {"■                                                         ■"},
      {"■                                                          ■"},
      {"■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■"},
      {"■基本操作方式：1.数字键盘对应锤子方向                      ■"},
      {"■              2.空格/按键5 对应拉锤子进行相应运动         ■"},
      {"■              3.左下角□->■就是此时锤子固定可以运动      ■"},
      {"■              4.翻越方向是固定的                          ■"},
      {"■状态栏        5.锤子是可以穿越墙体的                      ■"},
      {"■↓↓↓        6.向上跳的瞬间切换锤子方向可以灵活运动      ■"},
      {"■=〓|=〓=〓〓=〓〓=〓=〓〓|〓=〓〓=〓=〓〓〓=〓〓=〓=〓〓〓■"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓                                                  ▓"},
      {"▓⊙□⊙▓人生的本质在于运动，安谧宁静就是死亡。——帕斯卡  ▓"},
      {"▓⊙⊙⊙▓                                                  ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
    {11,
     19,
     58,
     6,
     {{"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                                  // 0      //pass1
      {"■                                                          ■"},     // 1
      {"■                                                          ■"},     // 2
      {"■                                                          ■"},     // 3
      {"■                                                          ■"},     // 4
      {"■                                                         ■"}, // 5
      {"■                                                          ■"},     // 6
      {"■                                                =〓=〓=〓=■"},     // 7
      {"■                                                =〓〓〓=〓■"},     // 8
      {"■                                           \\〓〓=〓〓〓=〓■"},    // 9
      {"■                                            〓=〓〓〓=〓〓■"},     // 10
      {"■                                                〓〓=〓=〓■"},     // 11
      {"■                             #                〓=〓=〓=〓=■"},     // 12
      {"■    ___                   ===#-             〓〓= = 〓=〓=■"},     // 13
      {"■     _//                     #                  =〓=〓=〓=■"},     // 14
      {"■|    #///                    #__/           〓〓=〓=〓=〓=■"},     // 15
      {"■|    #                       #                 〓〓=〓=〓=■"},     // 16
      {"■|    #/                     \\#                 〓〓=〓=〓 ■"},    // 17
      {"■|    #                       #                 〓〓=〓=〓=■"},     // 18
      {"■|    #          /====        #              〓〓=〓=〓=〓=■"},     // 19
      {"■=〓〓=〓=〓〓〓=〓〓=〓=〓=〓〓=〓=〓=〓〓=〓=〓〓〓=〓〓=■"},     // 20
      {"■〓〓〓=〓=〓〓=〓=〓=〓〓=〓=〓=〓〓=〓=〓〓==〓〓〓=〓〓=■"},     // 21
      {"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙游戏场景■"},                              // 22
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓Pass：1   ▓            欢迎回家                  ▓"},
      {"▓⊙□⊙▓新手村    ▓   有希望在的地方，痛苦也成欢乐。     ▓"},
      {"▓⊙⊙⊙▓梦的起点  ▓认识错误是拯救自己的第一步 —伊壁鸠鲁 ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
    {43,
     11,
     16,
     1,
     {{"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"}, // 0pass 4
      {"■                                    || ■■■■■■■■  "
       "■"},                                                            // 1
      {"■           ###########               ||              #### ■"}, // 2
      {"■          |# ################________||________      #### ■"}, // 3
      {"■          |    ###                                  ##■# ■"},  // 4
      {"■        _ |    ###      □                          ##■# ■"},   // 5
      {"■   |           ###          |                       ##### ■"}, // 6
      {"■                      #######                      #|   # ■"}, // 7
      {"■                                                  |#|   # ■"}, // 8
      {"■                                                   #|   # ■"}, // 9
      {"■        |                                          #|___# ■"}, // 10
      {"■                                                   ###### ■"}, // 11
      {"■            |     |                               |#|   # ■"}, // 12
      {"■                                                   #|   # ■"}, // 13
      {"■                                                   #□  # ■"},  // 14
      {"■                                                   #□__# ■"},  // 15
      {"■                                                 ==###### ■"}, // 16
      {"■                                                   #|   # ■"}, // 17
      {"■                                                   #|   # ■"}, // 18
      {"■                                                   #|   # ■"}, // 19
      {"■                                                 ■#|___# ■"},  // 20
      {"■                                  〓=〓=〓〓=〓〓=〓= =〓 ■"}, // 21
      {"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙游戏场景■"},                          // 22
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓Pass：2 ▓                                        ▓"},
      {"▓⊙□⊙▓建筑工地▓           唯有理智最为可贵。           ▓"},
      {"▓⊙⊙⊙▓g = 9.8 ▓                                        ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
    {29,
     17,
     57,
     2,
     {// ok
      {"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},
      {"■                                                          ■"},
      {"■                                                         ■"},
      {"■                                                          ■"},
      {"■                                                      ■■■"},
      {"■                                                    ■■■■"},
      {"■                                                    =■== ■"},
      {"■                                                     ■== ■"},
      {"■                                  __       〓〓       ■==■"},
      {"■                                _|  |_      ||         ■=■"},
      {"■                              ===■=■ =■=■=■=     ==■■"},
      {"■                                    = =■ =■==        =■■"},
      {"■                                       = =■==         =■■"},
      {"■                                     = =■=■■■■■ ==■■"},
      {"■                                  =■=■=■■=■■■■=■=■"},
      {"■                                   ■=■=■■■=■=■■=■■"},
      {"■                                =■=■=■■■■=■=■ ==■■"},
      {"■                                       ■■■■■=■=■=■■"},
      {"■               ___________|N|________■=■=■■  ■■■■ ■"},
      {"■             _| |___________________|_■■■■■■■■■■■"},
      {"■           _|  □ |____________________|_ ■■■■■■■■■"},
      {"■          |   □□   |___________________|  ■■■■■■■■"},
      {"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙游戏场景■"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓Pass：3 ▓                                        ▓"},
      {"▓⊙□⊙▓新的起点▓    没有物体，只有运动。——伯格森      ▓"},
      {"▓⊙⊙⊙▓帽子崖  ▓                                        ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
    {25,
     5,
     58,
     2,
     {{"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                                  // 0     //pass 2
      {"■                                                          ■"},     // 1
      {"■                                                         ■"}, // 2
      {"■                             〓〓〓=〓=〓=〓            〓■"},     // 3
      {"■                                           〓 〓  〓=〓=〓■"},     // 4
      {"■                                            〓  〓  〓 〓 ■"},     // 5
      {"■                                           〓 〓 〓〓〓=〓■"},     // 6
      {"■                                           〓=〓=〓〓〓=〓■"},     // 7
      {"■                                     〓〓=〓〓=〓=〓〓〓= ■"},     // 8
      {"■                                        〓〓=〓=〓〓〓=〓 ■"},     // 9
      {"■                                           〓=〓〓=〓=〓〓■"},     // 10
      {"■                              〓〓〓      =〓=〓〓〓=〓〓=■"},     // 11
      {"■                              〓〓〓    〓〓=〓=〓〓〓=〓 ■"},     // 12
      {"■                                  〓    =〓〓=〓=〓〓〓=〓■"},     // 13
      {"■                                        =〓〓=〓=〓〓〓=〓■"},     // 14
      {"■                                       =〓〓=〓=〓〓〓=〓 ■"},     // 15
      {"■                                  〓 =〓=〓〓=〓=〓〓〓=〓■"},     // 16
      {"■                  ！                 =〓=〓〓=〓=〓〓〓=〓■"},     // 17
      {"■                                  〓=〓〓=〓=〓〓〓=〓〓= ■"},     // 18
      {"■                                   =〓=〓=〓〓〓=〓〓= =〓■"},     // 19
      {"■                         〓〓〓=〓=〓=〓〓=〓=〓〓〓=〓〓=■"},     // 20
      {"■                         〓=〓=〓=〓=〓=〓〓=〓〓=〓= =〓 ■"},     // 21
      {"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙自创关卡■"},                              // 22
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓Pass：4 ▓                                        ▓"},
      {"▓⊙□⊙▓攀登    ▓  痛苦就是被迫离开原地。——康德        ▓"},
      {"▓⊙⊙⊙▓小心 ！ ▓                                        ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
    {57,
     19,
     41,
     2,
     {{"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                                  // 0   passs 3
      {"■                                                          ■"},     // 1
      {"■                                                         ■"}, // 2
      {"■                     _            ==〓=〓==               ■"},     // 3
      {"■                              =====〓=〓=〓〓 〓  〓=   ==■"},     // 4
      {"■                              =             〓  〓 〓     ■"},     // 5
      {"■                           =  =            〓 〓 〓       ■"},     // 6
      {"■                         =    =            〓= →→〓   〓■"},       // 7
      {"■                   =                 〓〓=〓〓=〓=〓      ■"},     // 8
      {"■                                             =    〓      ■"},     // 9
      {"■                        =   =                             ■"},     // 10
      {"■                               ===                       =■"},     // 11
      {"■                                〓〓    〓〓=〓=〓〓〓=〓 ■"},     // 12
      {"■                                〓      =〓〓   ==    〓= ■"},     // 13
      {"■                                〓=    =           〓〓=〓■"},     // 14
      {"■                                〓=                〓=〓  ■"},     // 15
      {"■                                  〓     =〓        〓〓= ■"},     // 16
      {"■                                      = =                 ■"},     // 17
      {"■                                  〓=〓〓       〓=       ■"},     // 18
      {"■                                   =〓  =                 ■"},     // 19
      {"■                                  =〓=〓=〓=〓〓〓= 〓〓= ■"},     // 20
      {"■                                  〓=〓=〓〓=〓〓=〓= =〓 ■"},     // 21
      {"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙自创关卡■"},                              // 22
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓Pass：5 ▓                                        ▓"},
      {"▓⊙□⊙▓山洞    ▓          我正在说谎。——罗素          ▓"},
      {"▓⊙⊙⊙▓学会卡位▓                                        ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
    {41,
     18,
     13,
     3,
     {{"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                                  // 0
      {"■                                                          ■"},     // 1
      {"■                                                          ■"},     // 2
      {"■                                                         ■"}, // 3
      {"■                                                          ■"},     // 4
      {"■           ⊙⊙                 ⊙⊙                      ■"},         // 5
      {"■           ⊙⊙=                ⊙⊙   ●    ●           ■"},           // 6
      {"■                                                          ■"},     // 7
      {"■                     =⊙⊙=                      ●       ■"},        // 8
      {"■                      ⊙⊙                                ■"},       // 9
      {"■                                                          ■"},     // 10
      {"■                                                    ●    ■"},      // 11
      {"■                                                          ■"},     // 12
      {"■                                                          ■"},     // 13
      {"■                                                      ●  ■"},      // 14
      {"■                                                  ●      ■"},      // 15
      {"■                                                          ■"},     // 16
      {"■                                             ●           ■"},      // 17
      {"■                                                          ■"},     // 18
      {"■                                     ■■■■             ■"},         // 19
      {"■                                     |      |             ■"},     // 20
      {"■                                     |      |             ■"},     // 21
      {"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                                  // 22
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓Pass：6 ▓                                        ▓"},
      {"▓⊙□⊙▓大雪球  ▓      只要有信心，人永远不会挫败。      ▓"},
      {"▓⊙⊙⊙▓        ▓                                        ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
    {23,
     17,
     54,
     3,
     {{"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                                  // 0
      {"■                                                          ■"},     // 1
      {"■                                                          ■"},     // 2
      {"■                                                         ■"}, // 3
      {"■                                                          ■"},     // 4
      {"■       _________                                          ■"},     // 5
      {"■      |     |   |           =〓=〓=〓=〓=〓=〓〓=〓=〓=〓=■"},     // 6
      {"■   = 〓〓   |   |         =〓〓=〓=〓=〓=〓= 〓 〓= 〓=〓=■"},     // 7
      {"■     = 〓〓 =   |     〓〓=〓〓=〓=〓=〓=〓=〓〓=〓=〓=〓=■"},     // 8
      {"■  〓=〓=〓 〓=  |       |   =〓〓=〓=〓〓〓=〓〓〓=〓=〓〓■"},     // 9
      {"■  Do Not Touch 〓==     |        =〓=〓=〓=〓〓〓=〓=〓=〓■"},     // 10
      {"■  The Snake 〓=〓       |            =〓〓=〓=〓=〓=〓 =〓■"},     // 11
      {"■   〓 ↓↓〓〓          |            〓〓==〓〓=〓=〓〓=〓■"},       // 12
      {"■   |                    |             〓〓=〓=〓〓〓=〓〓=■"},     // 13
      {"■   |      ___/          ^                =〓〓=〓=〓 =〓 =■"},     // 14
      {"■   | ____/             |_|                                ■"},     // 15
      {"■    /                                                     ■"},     // 16
      {"■   |                                                      ■"},     // 17
      {"■   |          =〓=〓〓=〓=〓=〓=〓=                       ■"},     // 18
      {"■   |           〓 〓== =〓 〓= =〓                        ■"},     // 19
      {"■   |            = 〓 = 〓〓 = =〓                         ■"},     // 20
      {"■   |             == =〓〓=〓〓=                           ■"},     // 21
      {"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                                  // 22
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓Pass：7 ▓                                        ▓"},
      {"▓⊙□⊙▓快乐桶  ▓    人可以犯错，但是不可犯同一个错。    ▓"},
      {"▓⊙⊙⊙▓快乐玩蛇▓                                        ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
    {3,
     16,
     38,
     2,
     {{"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                                  // 0
      {"■                                    ||                   ■"}, // 1
      {"■                                     ||                   ■"},     // 2
      {"■                                    -||                   ■"},     // 3
      {"■                                     ||                   ■"},     // 4
      {"■                                    _||_                  ■"},     // 5
      {"■                                  \\| ◇||                 ■"},     // 6
      {"■                                   | ◆ |                 ■"},      // 7
      {"■                                  || ◇||                 ■"},      // 8
      {"■                                   | ◆ |                 ■"},      // 9
      {"■                                   | ◇||                 ■"},      // 10
      {"■                                  _| ◆ |                 ■"},      // 11
      {"■                                   | ◇||                 ■"},      // 12
      {"■                              _____|_◆_|_____            ■"},      // 13
      {"■                           __|________________|__         ■"},     // 14
      {"■                          |   |   |   |   |  |   |        ■"},     // 15
      {"■                          |   |   |   |   |  |   |        ■"},     // 16
      {"■                         =〓=〓=〓〓=〓=〓 =〓==〓 =〓=〓=■"},     // 17
      {"■                        =〓=〓=〓〓=〓=〓=〓==〓 =〓=〓=〓■"},     // 18
      {"■                       =〓=〓=〓〓=〓=〓=〓==〓〓=〓=〓=〓■"},     // 19
      {"■    〓  =   =   =〓=〓=〓=〓=〓〓=〓=〓=〓==〓〓=〓=〓=〓=■"},     // 20
      {"■     =      =    〓=〓=〓=〓=〓〓=〓=〓=〓==〓〓=〓=〓=〓=■"},     // 21
      {"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                                  // 22
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓Pass：8 ▓                                        ▓"},
      {"▓⊙□⊙▓最终章  ▓         仰望星空，成功就在眼前         ▓"},
      {"▓⊙⊙⊙▓通天塔  ▓                                        ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
    {3,
     6,
     1,
     1,
     {{"■⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙⊙■"},                              // 0
      {"■回家  ■          end  恭喜顺利通关！！！！ end           ■"},  // 1
      {"■■■■■ 然而这破游戏并没有结局，Bennett Foddy只能选择回家■"},     // 2
      {"■■■■■    后记：                                        ■"},     // 3
      {"■      ■    写这个游戏几乎用上了我大一上学的所有技巧      ■"},  // 4
      {"■      ■                                                  ■"},  // 5
      {"■  ||■■    相当于一个十分大的功能整合程序                ■"},   // 6
      {"■  ||                                                      ■"}, // 7
      {"■   |        游戏里许多小细节的实现都是一个大程序          ■"}, // 8
      {"■   |                                                      ■"}, // 9
      {"■   |        但游戏重点还是游戏内容                        ■"}, // 10
      {"■   |                                                      ■"}, // 11
      {"■   |        前面花费太多时间实现基本功能以至于内容有点赶  ■"}, // 12
      {"■   |                                                      ■"}, // 13
      {"■   |        最重要的是为了写这个，我被正版游戏虐到吐了    ■"}, // 14
      {"■   |                                                      ■"}, // 15
      {"■   |        为了构思关卡去玩正版，直接心态爆炸            ■"}, // 16
      {"■   |                                                      ■"}, // 17
      {"■   |        很少用时间去设置导致游戏内容效果不是太完美    ■"}, // 18
      {"■   |                                                      ■"}, // 19
      {"■   |〓      但框架写的真的舒服，感觉可以自己控制任意模块 =■"}, // 20
      {"■   | =                                                   =■"}, // 21
      {"■⊙⊙⊙⊙⊙⊙总之收获很大，2020加油                      ⊙■"},        // 22
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"},
      {"▓⊙⊙⊙▓Pass：o ▓                                        ▓"},
      {"▓⊙□⊙▓  END   ▓     有理想在的地方，地狱就是天堂。     ▓"},
      {"▓⊙⊙⊙▓ 回家   ▓                                        ▓"},
      {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}}},
};

char
    menu_message[10]
                [200] = {
                    {"┏---------┓"}, {"┗---------┚"}, {"| ◇新游戏 |"}, {"| ◇继续   |"}, {"| ◇设定   |"}, {"| ◇更多   |"}, {"| ◇退出   |"} /*6	37	49*/ /*       27       7   */};
char loading[200][800] = {
    {"███████████████████████████████"},
    {"█                                                          █"},
    {"█  ■■      Getting Over It with Bennett Foddy      ■■  █"},
    {"█                                                          █"},
    {"█  ■■     ▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽     ■■  █"},
    {"█  ■■     ⊙程序名字：        掘地求升      ⊙     ■■  █"},
    {"█  ■■     ⊙                                ⊙     ■■  █"},
    {"█  ■■     ⊙版本信息：        o.o.o         ⊙     ■■  █"},
    {"█  ■■     ⊙                                ⊙     ■■  █"},
    {"█  ■■     ⊙程序作者：        404name       ⊙     ■■  █"},
    {"█  ■■     △△△△△△△△△△△△△△△△△△     ■■  █"}, // 10
    {"█                                                          █"},
    {"█  ⊙log    ██████████████████           █"}, // 13,12
    {"█                                                          █"},
    {"█           ██████████████████           █"},
    {"█                                                          █"},
    {"█           ██████████████████           █"},
    {"█                                                          █"},
    {"█           ██████████████████           █"},
    {"█                                                          █"},
    {"█           ██████████████████           █"},
    {"█                                                          █"},
    {"███████████████████████████████"}, // 22
    {"▓          L O A D I N G：□□□□□□□□□□□           ▓"},
    {"▓          加载文档     : ◇    0000/1000     ◇           ▓"},   //  27 24
    {"▓                                                          ▓"}, // 34 25
    {"▓                                                          ▓"},
    {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}};
/*

    {"█  ■■     ⊙1.21             设置地图+整合游戏     ■■  █"},

    {"█                                                          █"},*/
char end[200][800] = {
    {"■■□□□■■□□□■■□□□□□□□□□□□□■■□■□□□"}, {"□■■□□□■□□□■■■■■□■■■■■■□□■■□■■□□"}, {"□□□■■■■■□■■■■■■□■■■■■■□□■■□□■□□"}, {"□□□■■■■■■■□□□□□□□□□□■■□□■■■■■■□"}, {"■■□□■■□□■□■■■■■□□□□□■■■■■■■■■■□"}, {"□■■□■■■■□□■■■■■□□■■□■■■■■■□□□□□"}, {"□□□□■■■■□□□□■■□□□■■□■■□□■■□□■□□"}, {"□□□□■■□■□■■■■■■□□□■■■□□□■■□■■□□"}, {"□□■□■■□■□■■■■■■□□□□■■□□□■■□■■□□"}, {"□□■□■■□■□□□■■□□□□□■■■■□□□■■■□□□"}, {"□■■□■■□■□□□■■□□□□■■□□■□□□■■□□□■"}, {"□■□□■■□■□□□■■□□□■■□□□□□□■■■■□■■"}, {"■■□■■■□■□□■■■□□□■□□□□□■■■□□■■■■"}, {"■■□■■□■■□□■■□□□□□□□□□□■■□□□□■■■"}, {"□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□"}, {"□□■■□□□□□■■□□□□□□□□□□□□■■□□□□□□"}, {"□□■■□□■■■■■■■■■□■■■■■■■■■■■■■■■"}, {"□■■□□□■■■■■■■■■□■■■■■■■■■■■■■■■"}, {"■■□□■■□□□■■□□□□□□□□□□□□■■□□□□□□"}, {"■■■■■■□■■■■■■■□□□□■■■■■■■■■■■■□"}, {"□□■■□□□■■■■■■■□□□□■■■■■■■■■■■■□"}, {"□■■□□□□□□□□□□□□□□□■■□□□■■□□□■■□"}, {"■■■■■■□■■■■■■■□□□□■■■■■■■■■■■■□"}, {"■■■■■■□■■■■■■■□□□□□□□■■■■■□□□□□"}, {"□□□□□□□■■□□□■■□□□□□□■■□■■■■□□□□"}, {"□□□■■■□■■□□□■■□□□□□■■□□■■□■■□□□"}, {"■■■■■■□■■■■■■■□□□■■■□□□■■□□■■■□"}, {"■■■□□□□■■■■■■■□□□■■□□□□■■□□□■■□"}, {"□□□□□□□■■□□□■■□□□□□□□□□■■□□□□□□"}};
char more_message[200][800] = {
    {"懒得开发模块了"},
    {"按任意键继续"},
    {"游戏信息：掘地求升C语言版"},
    {"游戏作者：CTGU_LLZ       "},
    {"游戏性质：寒假答辩作品"},
    {"开发日志:"},
    {"⊙1.15             实现跳跃与重力    "},
    {"⊙1.16-1.18        构建游戏框架      "},
    {"⊙1.19             实现关卡间跳跃    "},
    {"⊙1.20             写菜单+游戏整合   "},
    {"⊙1.21-later       丰富关卡+优化操作 "}, // 38
    {"更多相关内容："},
    {"github：https://github.com/404name/C-game"},
    {"博客园：https://www.cnblogs.com/404name/"},
    {"启明星论坛：https://www.ctguqmx.com/article/category-14"},
    {"没了"}};
char menu_message_1[10][20] = {{"取消"}, {"退出"}, {"：开挂"}, {"：返回"}, {"：——"}, {"：——"}};
char loading_message[10][50] = {
    {"⊙Bennett Foddy 通关游戏后飞向了太空 "},
    {"⊙他长时间暴露在宇宙射线下发生了变异 "},
    {"⊙变成了一只会玩锤子的可爱小白兔  M  "},
    {"⊙并且只存活在C语言代码中            "},
    {"⊙你能帮他继续往上抡锤子吗           "}}; // 38
char setting_message[8][100] = {
    {"███████████████████████████████"},
    {"▓                                                          ▓"},
    {"▓                         2              3                 ▓"},
    {"▓                         4              5                 ▓"},
    {"▓                                                          ▓"}, // 27 42
                                                                      // 24-25
    {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}};
char exit_message[8][100] = {
    {"███████████████████████████████"},
    {"▓                                                          ▓"},
    {"▓              是 否 确 认 退 出 游 戏                     ▓"},
    {"▓                         0              1                 ▓"},
    {"▓                                                          ▓"}, // 27 42
                                                                      // 24-25
    {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}};
char menu[200][800] = {
    {"███████████████████████████████"},
    {"█■■■■■■■■■■■                                    █"},
    {"█■■■■■■■■■■■■    _________                     █"},
    {"█■■ 和Bennett Foddy一起■ /_______◎\\                    █"},
    {"█■■■■■■■■攻克难关■|____   ____\\                   █"},
    {"█■■■■■■■■■■■■■■   |*|                        █"},
    {"█■■■■■■■■■■■■■■   |*|                0       █"},
    {"█■■■■■■■■■■■■■■■ |*|                2       █"},
    {"█■■■■■■■■■■■■■■■ |*|                1       █"},
    {"█■■■■■■■■■■■■■■■ |*|                0       █"},
    {"█■■■■■■■■■■■■■■■ |*|                3       █"},
    {"█■■■■■■■■■■■■■■■ |*|                1       █"},
    {"█■■■■■■■■■■■■■■■ |*|                0       █"},
    {"█■■■■■■■■■■■■■■■ |*|                4       █"},
    {"█■■■■■■■■■■■■■■■ |*|                1       █"},
    {"█■■■■■■■■■■■■■■◤ |*|                0       █"},
    {"█■■■■■■■■■■■■■■   |*|                5       █"},
    {"█■■■■■■■■■■■■■■   |*|                1       █"},
    {"█■■■■■■■■■■■■■◤   |*|                0       █"},
    {"█■■■■■■■■■■■■■     |*|                6       █"},
    {"█■■■■■■■■■■■■■     |*|                1       █"},
    {"█version o.o.o■■■■■◤      |*|                        █"},
    {"███████████████████████████████"},
    {"▓          L O A D I N G：■■■■■■■■■■■■         ▓"},
    {"▓          加载文档     :       成 功 加 载                ▓"},
    {"▓          Tip          :   方向键可控制上下左右           ▓"},
    {"▓                                                          ▓"},
    {"▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓"}};
char map[200][800]; //地图载体
char message[10][3] = {{"⊙"}, {"●"}, {"□"}, {"■"}};
char turn = '8', old_turn = turn;
int now_pass = 0, old_pass = 0, back_to_last_pass_x = 0, if_back = 0;
int check[200] = {4, 5, 6, 7, 0, 1, 2, 3}; //让锤子位置和人位置相对相反
int x = 7, y = 13, old_x = 7, old_y = 13, x_link = 7, y_link = 12, x_hammer = 7,
    y_hammer = 11, reset_x = x, reset_y = y;
int direct_people = 6, old_direct_people = 6, direct_hammer = 2,
    old_direct_hammer = 2;
int can_jump = 0, jumpturn = 8, jump_time = 0;
int gravity_on = 1, jump_up_flag = 0, paste = 0, old_paste, G_flag = 0,
    game_end = 0;
int next[8][2] = {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}};
//                  0左     1左上     2上      3右上     4右     5右下    6下
//                  7左下
int jumpform[8][11] = {
    //对应next
    //两倍
    {1, 2},          // 0直接跳   (0下标为执行次数,后面记录状态)
    {3, 4, 4, 6},    // 1右小跳
    {3, 0, 0, 6},    // 2左小跳
    {4, 2, 4, 4, 6}, // 3右翻越
    {4, 2, 0, 0, 6}, // 4左翻越
    {4, 0, 2, 2, 4}, // 5下
    {4, 2, 2, 0, 0}, // 6左上
    {4, 2, 2, 4, 4}, // 7右上
};
int message_place[10][2] = {{0, 0}, {2, 26}, {4, 26}, {6, 26}, {2, 25}, {4, 25}, {6, 25}, {2, 24}, {4, 24}, {6, 24}}; // x 水平
int direct_hammer_to_message_place[8] = {4, 7, 8, 9, 6, 3, 2, 1};
int jumpform_to_direct_hammer[8][11] = { // jump时保持锤子不动   两倍
    {8, 8},
    {8, 6, 7, 0},
    {8, 6, 5, 4},
    {8, 5, 6, 7, 0},
    {8, 7, 6, 5, 4},
    {8, 3, 4, 5, 6},
    {8, 0, 7, 6, 5},
    {8, 4, 5, 6, 7}};

int direct_people_to_jump[8] = {3, 1, 0, 2, 4, 6, 5, 7};
int back_pass_trigger[80][80] = {0}; // 0没事   1返回上一关
int next_pass_trigger[80][80] = {0}; // 0没事   1下一关
int move(int x, int y, int limit_x1, int limit_x2, int limit_y1, int limit_y2,
         int xx, int yy)
{
    //主要函数 ：灵活的实现选择指针；
    //参数：开始坐标x y  限制活动空间 x左右  y 上下 移动距离 x y
    int tx = x, ty = y, i;
    gotoxy(x - 14, y);
    printf("▉");
    gotoxy(x, y);
    printf("▉"); //可自定义2个字节的指针图标
    char ch = '\0', ch1 = '\0';
    char res[20];
    while (ch != ' ')
    {
        ch = getch();
        if (ch != ' ' && ch != 'w' && ch != 's' && ch != 'a' && ch != 'd' &&
            ch != -32)
            continue;
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
        gotoxy(x - 14, y);
        printf("  ");
        gotoxy(x, y);
        printf("  ");
        switch (ch)
        {
        case 'a':
            x = x - xx;
            break;
        case 'd':
            x = x + xx;
            break;
        case 'w':
            y = y - yy;
            break;
        case 's':
            y = y + yy;
            break;
        case ' ':
            break;
        }
        if (x < limit_x1 || x > limit_x2 || y < limit_y1 || y > limit_y2)
        {
            x = tx;
            y = ty;
        }
        ty = y, tx = x;
        gotoxy(x - 14, y);
        printf("▉");
        gotoxy(x, y);
        printf("▉");
    }
    return map[y][x] - '0';
}
int init()
{
    system("mode con cols=62 lines=29");
    system("color 07");
    check['4'] = 0;
    check['7'] = 1;
    check['8'] = 2;
    check['9'] = 3;
    check['6'] = 4;
    check['3'] = 5;
    check['2'] = 6;
    check['1'] = 7; //直接让锤子移动对应next；
    check['a'] = 0;
    check['d'] = 4;
    for (int i = 2; i < 60; i++) //触发坠落
    {
        back_pass_trigger[22][i] = 1;
    }
    for (int i = 0; i <= 27; i++)
    {
        printf("%s\n", loading[i]);
    }
    Sleep(200);
    gotoxy(13, 12);
    printf("按任意键开始加载◇◇◇(按p直接跳过)");
    char temp = getch();
    if (temp == 'p')
        return 1;
    int flag_1 = 0, flag_2 = 0;
    for (int i = 0; i <= 1000; i++)
    {
        if (i % 200 == 0 && i != 1000)
        { // 13 12 38
            gotoxy(12, 12 + (2 * flag_1));
            printf("%38s", loading_message[flag_1]);
            flag_1++;
        }
        if (i % 91 == 0)
        {
            gotoxy(27 + (2 * flag_2), 23);
            printf("■");
            flag_2++;
        }
        gotoxy(33, 24);
        printf("%4d", i);
        Sleep(load_speed);
    }
    gotoxy(12, 25);
    printf("⊙帮助它(按任意键进入游戏)");
    Sleep(200);
    gotoxy(12, 26);
    printf("⊙玩个锤子的兔子啊(按P拒绝)");
    char ch = getch();
    if (ch == 'p' || ch == 'P')
        return 0;
    else
        return 1;
}

void load_game() //读取关卡信息
{
    gotoxy(0, 0);
    int i, j;
    direct_people = 6, old_direct_people = 6;
    direct_hammer = 2, old_direct_hammer = 2;
    for (i = 0; i <= 27; i++)
        strcpy(map[i], s[now_pass].map[i]);
    if (back_to_last_pass_x == 0)
    { //正常通关
        x = s[now_pass].start_x;
        y = s[now_pass].start_y;
    }
    else
    {
        x = back_to_last_pass_x;
        y = 3;
        back_to_last_pass_x = 0;
    }
    old_x = x, old_y = y;
    x_link = x, y_link = y - 1;
    x_hammer = x, y_hammer = x - 2;
    for (i = 0; i < 8; i++) //新的下一关触发器设置
        next_pass_trigger[s[old_pass].next_y + next[i][1]]
                         [s[old_pass].next_x + next[i][0]] = 0;
    for (i = 0; i < 8; i++) //新的下一关触发器设置
        next_pass_trigger[s[now_pass].next_y + next[i][1]]
                         [s[now_pass].next_x + next[i][0]] = 1;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    for (i = 0; i <= 22; i++)
        printf("%s\n", map[i]);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); //附加信息框
    for (i = 23; i <= 27; i++)
        printf("%s\n", map[i]);
    gotoxy(x, y);
    printf("%c", people);
}

void show_move() //移动显示
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 00);
    if (old_x != x || old_y != y)
    {
        gotoxy(old_x, old_y);
        printf(" ");
        direct_hammer = check[direct_people];
    }
    if (map[y_link][x_link] == link)
    {
        map[y_link][x_link] = ' ';
        gotoxy(x_link, y_link);
        printf(" ");
    }
    if (map[y_hammer][x_hammer] == hammer)
    {
        map[y_hammer][x_hammer] = ' ';
        gotoxy(x_hammer, y_hammer);
        printf(" ");
    }
    //消除上次
    if (jump_time &&
        jumpform_to_direct_hammer[jumpturn][jumpform[jumpturn][0] + 1 -
                                            jump_time] != 8)
    { //锤子不动
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
        gotoxy(message_place[direct_hammer_to_message_place[direct_hammer]][0],
               message_place[direct_hammer_to_message_place[direct_hammer]][1]);
        printf("%2s", message[0]);
        direct_hammer =
            jumpform_to_direct_hammer[jumpturn]
                                     [jumpform[jumpturn][0] + 1 - jump_time];
        direct_people = check[direct_hammer];
        x_hammer =
            x +
            2 * next[jumpform_to_direct_hammer[jumpturn][jumpform[jumpturn][0] +
                                                         1 - jump_time]][0];
        y_hammer =
            y +
            2 * next[jumpform_to_direct_hammer[jumpturn][jumpform[jumpturn][0] +
                                                         1 - jump_time]][1];
        x_link = (x_hammer + x) / 2;
        y_link = (y_hammer + y) / 2;
        gotoxy(message_place[direct_hammer_to_message_place[direct_hammer]][0],
               message_place[direct_hammer_to_message_place[direct_hammer]][1]);
        printf("%2s", message[1]);
    }
    else
    { //锤子伴随人动
        x_link = x + next[direct_hammer][0];
        y_link = y + next[direct_hammer][1];
        x_hammer = x + 2 * next[direct_hammer][0];
        y_hammer = y + 2 * next[direct_hammer][1];
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    if (map[y_link][x_link] == ' ')
    {
        map[y_link][x_link] = link;
        gotoxy(x_link, y_link);
        printf("%c", link);
    }
    if (map[y_hammer][x_hammer] == ' ')
    {
        map[y_hammer][x_hammer] = hammer;
        gotoxy(x_hammer, y_hammer);
        printf("%c", hammer);
    } //更新下一次
    gotoxy(x, y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    printf("%c", people);
    gotoxy(x, y);
    /*gotoxy(0,30);                        //打印坐标信息
    printf("x is %d\ny is %d",x,y); */
}

void autochange() //系统自动更新
{
    old_paste = paste;
    old_x = x;
    old_y = y;
    old_direct_hammer = direct_hammer;
    if (next_pass_trigger[y][x] == 1 ||
        next_pass_trigger[y_hammer][x_hammer] == 1 ||
        next_pass_trigger[y_link][x_link] == 1)
    {
        now_pass++;
        return;
    }
    if (back_pass_trigger[y + 1][x] == 1)
    {
        back_to_last_pass_x = x;
        now_pass--;
        return;
    }
    if (back_pass_trigger[y_link + 1][x_link] == 1 &&
        map[y_link][x_link] == link)
    {
        back_to_last_pass_x = x;
        now_pass--;
        return;
    }
    if (back_pass_trigger[y_hammer + 1][x_hammer] == 1 &&
        map[y_hammer][x_hammer] == hammer)
    {
        back_to_last_pass_x = x;
        now_pass--;
        return;
    }
    if (/*map[y_link][x_link] == link && map[y_hammer][x_hammer] == hammer && */
        can_jump == 1)
    {
        jumpturn = direct_people_to_jump[direct_people];
        jump_time = jumpform[jumpturn][0];
        /*if ((direct_people == 0 || direct_people == 4) && map[y + 1][x] != '
           ') jump_time = 0;*/
        can_jump = 0;
    }
    if (map[y +
            next[jumpform[jumpturn][jumpform[jumpturn][0] + 1 - jump_time]][1]]
           [x + next[jumpform[jumpturn][jumpform[jumpturn][0] + 1 - jump_time]]
                    [0]] != ' ')
    {
        jump_time = 0;
        jumpturn = 8;
    }
    if (map[y +
            2 * next[jumpform[jumpturn][jumpform[jumpturn][0] + 1 - jump_time]]
                    [1]][x + 2 * next[jumpform[jumpturn][jumpform[jumpturn][0] +
                                                         1 - jump_time]][0]] !=
        ' ')
    {
        jump_time = 0;
        jumpturn = 8;
    }
    if (jump_time != 0)
    {
        reset_x = x;
        reset_y = y;
        x = x +
            2 * next[jumpform[jumpturn][jumpform[jumpturn][0] + 1 - jump_time]]
                    [0];
        y = y +
            2 * next[jumpform[jumpturn][jumpform[jumpturn][0] + 1 - jump_time]]
                    [1];
        show_move();
        jump_time--;
        Sleep(150);
    }
    else if (gravity_on == 1 &&
             (map[y + 1][x] == ' ' || map[y + 1][x] == link ||
              map[y + 1][x] == hammer))
    { //重力下坠
        y++;
        show_move();
        Sleep(100);
    }
    if (jump_time == 0)
    {
        jump_time = 0;
        jumpturn = 8;
    }
    gravity_on = 1;
    paste = 0; //是否黏住
    if (map[y_hammer][x_hammer] ==
        hammer) //若锤子固定  则取消重力  取消跳跃状态。
        for (int i = 0; i < 8; i += 2)
        {
            /*if (i == 2)               //取消上方粘
                continue;*/
            if (map[y_hammer + next[i][1]][x_hammer + next[i][0]] != link &&
                map[y_hammer + next[i][1]][x_hammer + next[i][0]] != ' ')
            {
                gravity_on = 0;
                // jump_time = 0;
                // jumpturn = 8;
                paste = 1;
            }
        }
    if (old_paste != paste)
    { //显示粘贴信息
        gotoxy(message_place[5][0], message_place[5][1]);
        printf("%2s", message[paste + 2]);
        gotoxy(x, y);
    }
    /*gotoxy(1,17);
        if(jump_up_flag == 1)
                printf("can jump");
        else printf("no  jump");
        gotoxy(1,18);
        if(gravity_on == 1)
                printf("will drop");
        else printf("no   drop");*/
}

void manchange() //人为输入更新
{
    if (kbhit())
    {
        old_x = x;
        old_y = y;
        old_direct_people = direct_people;
        old_direct_hammer = direct_hammer;
        turn = getch();
        if (turn == ' ' && jump_time == 0 &&
            ((map[y_hammer][x_hammer] != hammer && map[y + 1][x] != ' ' &&
              map[y + 1][x] != link &&
              (direct_people ==
               2 /*|| direct_people == 1 || direct_people == 3*/)) ||
             paste == 1))
        {
            can_jump = 1;
        }
        /*else if (turn == 'a' || turn == 'd' || turn == 'w' || turn == 's')
        //为了优化操作取消wasd控制人物
        {
            if (paste == 0 && (map[y + next[check[turn]][1]][x +
        next[check[turn]][0]] == ' ' || map[y + next[check[turn]][1]][x +
        next[check[turn]][0]] == link))
            {
                x = x + next[check[turn]][0];
                y = y + next[check[turn]][1];
                show_move();
            }
            else
            {
                if (turn == 'a')
                {
                    if (direct_people > 1 && direct_people <= 3)
                    {
                        direct_people--;
                    }
                    else if (direct_people >= 5 && direct_people < 7)
                    {
                        direct_people++;
                    }
                }
                else if (turn == 'd')
                {
                    if (direct_people >= 1 && direct_people < 3)
                    {
                        direct_people++;
                    }
                    else if (direct_people > 5 && direct_people <= 7)
                    {
                        direct_people--;
                    }
                }
                else if (turn == 'w')
                {
                    if (direct_people > 3 && direct_people <= 5)
                    {
                        direct_people--;
                    }
                    else if (direct_people == 7 || direct_people == 0)
                    {
                        direct_people++;
                    }
                }
                else if (turn == 's')
                {
                    if (direct_people >= 3 && direct_people < 5)
                    {
                        direct_people++;
                    }
                    else if (direct_people == 0 || direct_people == 1)
                    {
                        direct_people--;
                    }
                }
                if (direct_people < 0)
                    direct_people = 7;
                if (direct_people > 7)
                    direct_people = 0;
                if (map[y_hammer + 2 * next[direct_people][1]][x_hammer + 2 *
        next[direct_people][0]] == ' ')
                {
                    if (map[(y + y_hammer + 2 * next[direct_people][1]) / 2][(x
        + x_hammer + 2 * next[direct_people][0]) / 2] == ' ')
                    {
                        y = y_hammer + 2 * next[direct_people][1];
                        x = x_hammer + 2 * next[direct_people][0];
                        show_move();
                    }
                    else
                        direct_people = old_direct_people;
                }
                else
                    direct_people = old_direct_people;
            }
        }*/
        else if (turn == 'p')
        {
            if_back = 1;
        }
        /*else if (turn == 'q' || turn == 'e') //遇到障碍物会卡住
        {
            if (turn == 'q')
                direct_hammer--;
            else
                direct_hammer++;
            if (direct_hammer < 0)
                direct_hammer = 7;
            if (direct_hammer > 7)
                direct_hammer = 0;
            direct_people = check[direct_hammer];
            show_move();
        }*/
        //困难模式开启0.0
        else if (turn >= '1' && turn <= '9' && turn != '5') //不卡住
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 07);
            gotoxy(
                message_place[direct_hammer_to_message_place[direct_hammer]][0],
                message_place[direct_hammer_to_message_place[direct_hammer]]
                             [1]);
            printf("%2s", message[0]);
            direct_hammer = check[turn]; //数字键盘快捷改变方向
            direct_people = check[direct_hammer];
            gotoxy(
                message_place[direct_hammer_to_message_place[direct_hammer]][0],
                message_place[direct_hammer_to_message_place[direct_hammer]]
                             [1]);
            printf("%2s", message[1]);
            show_move();
        }
        else if (turn == 'n' && G_flag == 1 && now_pass < 9)
            now_pass++;
        else if (turn == 'b' && G_flag == 1 && now_pass > 0)
            now_pass--;
        else if (turn == 'f')
        {
            x = reset_x;
            y = reset_y;
            show_move();
        }
        else if (turn == 'r' && map[y + 1][x] != ' ' &&
                 map[y + 1][x] != link && paste != 1)
        {
            load_game();
            reset_x = x;
            reset_y = y;
        }
    }
}
//后面是相应的首页菜单功能
void more()
{
    char ch;
    int message = 16;
    for (int i = 1; i <= message; i++)
    {
        gotoxy(2, i);
        printf("                                                          ");
        gotoxy(2, i + 1);
        printf("                                                          ");
        gotoxy(2, i);
        printf("%s", more_message[i - 1]);
        ch = getch();
    }
}
void exit()
{
    for (int i = 22; i <= 27; i++)
    {
        strcpy(map[i], exit_message[i - 22]);
        gotoxy(0, i);
        for (int j = 0; j <= 61; j++)
        {
            if (map[i][j] >= '0' && map[i][j] <= '9')
            {
                gotoxy(j - 11, i);
                printf("◆ %s ◆", menu_message_1[map[i][j] - '0']);
                gotoxy(j + 1, i);
            }
            else if (i == 0 || i == 22 || i == 23 || i == 27 || j == 1 ||
                     j == 0 || j == 61 || j == 60)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                printf("%c", map[i][j]);
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                printf("%c", map[i][j]);
            }
        }
        printf("\n");
    }
    int flag =
        move(27, 25, 27, 42, 25, 25, 15, 0); // 2新游戏 3选关卡 4更多 5退出
    switch (flag)
    {
    case 0:
        break;
    case 1:
    {
        game_end = 1;
        break;
    }
    }
}

void setting()
{
    for (int i = 22; i <= 27; i++)
    {
        strcpy(map[i], setting_message[i - 22]);
        gotoxy(0, i);
        for (int j = 0; j <= 61; j++)
        {
            if (map[i][j] == '2' && G_flag == 0)
            {
                gotoxy(j - 10, i);
                printf("◇%s", menu_message_1[map[i][j] - '0']);
                gotoxy(j + 1, i);
            }
            else if (map[i][j] >= '0' && map[i][j] <= '9')
            {
                gotoxy(j - 10, i);
                printf("◆%s", menu_message_1[map[i][j] - '0']);
                gotoxy(j + 1, i);
            }
            else if (i == 0 || i == 22 || i == 23 || i == 27 || j == 1 ||
                     j == 0 || j == 61 || j == 60)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                printf("%c", map[i][j]);
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                printf("%c", map[i][j]);
            }
        }
        printf("\n");
    }
    int flag = move(27, 24, 27, 50, 24, 25, 15,
                    1); // 2新游戏 3选关卡 4更多 5退出//28     43        24-25
    switch (flag)
    {
    case 2:
    {
        if (G_flag)
            G_flag = 0;
        else
            G_flag = 1;
        break;
    }
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    }
}
void show_menu()
{
    system("cls");
    for (int i = 0; i <= 27; i++)
        strcpy(map[i], menu[i]);
    for (int i = 0; i <= 27; i++)
    {
        for (int j = 0; j <= 61; j++)
        {
            if (map[i][j] >= '0' && map[i][j] <= '9')
            {
                gotoxy(j - 12, i);
                printf("%s", menu_message[map[i][j] - '0']);
                gotoxy(j + 1, i);
            }
            else if (i == 0 || i == 22 || i == 23 || i == 27 || j == 1 ||
                     j == 0 || j == 61 || j == 60)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                printf("%c", map[i][j]);
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                printf("%c", map[i][j]);
            }
        }
        printf("\n");
        Sleep(30);
    }
}

void game_system()
{
    load_game();
    while (if_back == 0)
    {
        old_pass = now_pass;
        autochange();
        manchange();
        if (now_pass != old_pass)
        {
            if (now_pass < 0)
                now_pass = 0;
            load_game();
            reset_x = x;
            reset_y = y;
        }
    }
    if_back = 0;
    return;
}

void select()
{
    int flag = move(52, 7, 52, 52, 7, 19, 0, 3); // 2新游戏 3选关卡 4更多 5退出
    switch (flag)
    {
    case 2:
    {
        now_pass = 0;
        game_system();
        break;
    }
    case 3:
        game_system();
        break;
    case 4:
        setting();
        break;
    case 5:
        more();
        break;
    case 6:
        exit();
        break;
    }
}

int main()
{
    if (init() == 1)
        while (1)
        {
            show_menu();
            int flag = move(52, 7, 52, 52, 7, 19, 0,
                            3); // 2新游戏 3选关卡 4更多 5退出
            switch (flag)
            {
            case 2:
            {
                now_pass = 0;
                game_system();
                break;
            }
            case 3:
                game_system();
                break;
            case 4:
                setting();
                break;
            case 5:
                more();
                break;
            case 6:
                exit();
                break;
            }
            if (game_end == 1)
                break;
        }
    gotoxy(0, 0);
    for (int i = 0; i <= 28; i++)
    {
        printf("%s\n", end[i]);
        Sleep(100);
    }
    Sleep(10000);
    system("cls");
    printf("游戏结束");
    return 0;
}
// bug区

//设置如何储存继续（p退出后直接进去就是继续，新游戏就会初始化）   ok
//需要解决wasd移动左下角不变化                                    ok删除wasd
//解决进入下一关随机的bug                                         ok
//解决hammer在触发器上没反应的情况 autochange里面                 ok

//优化区
//设置颜色                                                        ok淡色
//设置更丰富的关卡                                                ok
//设置初始教程                                                    ok

