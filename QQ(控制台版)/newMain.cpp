/*-----------------------------------
程序    ：   C语言还原QQ 【加密版】 
作者    ：   404name / 绝伦N CTGU_LLZ
完成时间：   2021.11.22 
说明    ：   程序是面向过程实现的，不适合学习，可以看看设计，面向过程100个人有100种实现方式，不像
面向对象编程 + 算法可以有统一的模板。程序高度还原了qq，通过wads控制光标移动，空格进行确认，回车进行
输入文字退出，q进行页面回退来操控。

已完成界面：[QQ首页][QQ堆话框][QQ登录页]   [联系人和空间是静态的] 
支持操作：登录/注册  发起聊天+接受信息+查询最近聊天+存储信息记录
本程序分为 [demo版][数据伴随MD5加密版]  
	  
	  
	程序部分函数数组意义说明 
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
#include <stack>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <windows.h>
#define Is_Active_Color 115     //当前所在状态栏的颜色 浅蓝色灰底
#define Is_Selected1_Color 128  //被选中的颜色1 蓝色深灰底
#define Is_Selected2_Color 62   //被选中的颜色2 白色蓝底
#define Is_Selected3_Color \
    369  //被选中的颜色3 深蓝色灰色(解决一些字符不显示背景用)
#define Video_Color 3     //视频颜色 蓝色黑底
#define Normal_Color 112  //正常的颜色 灰底
#define Boder_Color 391   //边框的颜色 深灰底
using namespace std;
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
int nowPage = 4;
int indexCnt = 0;// 首页有多少条消息记录 
int startItem = 0,endItem = 0;
bool onPlay = false;   //关闭则是杀死
bool stopPlay = true;  //开启则是停止
bool isLogin = false;
stack<int> pageStack; 
int activeIndexBottom = 17;  //当前响应的页面(顶部
int activeIndexTop = 5;      //当前响应的页面(底部
string username = "绝伦N";
string pwd = "123456"; 

char bacground[48][55] = {
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                      i.,ii;;ii;  .: .i7:            ",
"                    :;iiir7i;X;XXX;i  :i,            ",
"                  7iii;;:iS;::iX7XS  ..              ",
"                 XSr,::i,r;i:i;::XXXi,               ",
"                .Srr;::,rr:.:iii;7X2S                ",
"                ;;:;r:iSSr;,r7;i;iXaa                ",
"                7r;rXX22XrrrXX2rr;22Z                ",
"                ;r;XXSS2XXr7XX: ,XS2S     ::         ",
"                ;r;XXXXXSSS7X:  :Xa@Z                ",
"               .7rXSXSXSXXS2S7S7r:7MMS               ",
"              SZ222S2SXS2X2aXS0a2XSZ8ZS              ",
"             0WZ20a2X2SaXX22aa2aXSSXSX2W, ,.         ",
"            XM8SSa2XaSSXXZZSa22X7aXS2aaMM  .         ",
"            aZXSXSXaSSXXXZ222S;XrZ82S22WMZ           ",
"           ,BXX8SXXXXXXSX2ZaZa27rZa7XXSWMM           ",
"           i8BZX28XXXX2arZZaaZ882araZXWMMW           ",
"            rr.  80ZZZ8a20BZ888ZaZ80r 7X0            ",
"                 iB008ZZaa88S: .XXWZ:                ",
"               7@8ZZZaZZ880ZXirXXraMMW               ",
"               SM228XZMM@BSZ8MM8Z8WMM0     .         ",
"              . r.;:ir2a,    .i::. :                 ",
"            i,.                                      ",
"            .. ,                     .               ",
"                                                     ",
"                     .r..;. :i.,;                    ",
"                     2    S:X   ,X                   ",
"                     7:  X7 X . Si                   ",
"                      ii:r:  ;:i7.                   ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     ",
"                                                     "
};
//三维依次是   哪个界面 界面哪行  某行第几个模块
// appMap[x][0][0]表示该页面有几行
int appMap[20][12][7] = {{{5, 0, 0, 0, 0, 0, 0},  // 0.404报错页面
                          {0, 1, 2, 2, 2, 3, 0},
                          {0, 20, 20, 20, 20, 20, 0},
                          {0, 17, 17, 18, 19, 19, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{11, 0, 0, 0, 0, 0, 0},  // 1.首页
                          {0, 1, 2, 2, 2, 3, 0},  
                          {0, 9, 9, 9, 9, 9, 0},
                          {0, 10, 10, 10, 10, 10, 0},
                          {0, 11, 11, 11, 11, 11, 0},
                          {0, 12, 12, 12, 12, 12, 0},
                          {0, 13, 13, 13, 13, 13, 0},
                          {0, 14, 14, 14, 14, 14, 0},
                          {0, 15, 15, 15, 15, 15, 0},
                          {0, 17, 17, 18, 19, 19, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{9, 0, 0, 0, 0, 0, 0},  // 2.联系人 
                          {0, 1, 2, 2, 2, 3, 0}, 
                          {0, 21, 21, 21, 21, 21, 0},
                          {0, 22, 22, 22, 22, 22, 0},
                          {0, 23, 23, 23, 23, 23, 0},
                          {0, 4, 5, 6, 7, 8, 0},
                          {0, 24, 24, 24, 24, 24, 0},
                          {0, 17, 17, 18, 19, 19, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{7, 0, 0, 0, 0, 0, 0},  // 3.动态 
                          {0, 1, 2, 2, 2, 3, 0},
                          {0, 25, 25, 26, 27, 28, 0},
                          {0, 29, 29, 29, 29, 29, 0},
                          {0, 30, 30, 30, 30, 30, 0},
                          {0, 17, 17, 18, 19, 19, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{7, 0, 0, 0, 0, 0, 0},  // 4.登录注册页面 (待更新 )
                          {0, 31, 31, 31, 31, 31, 0},
                          {0, 32, 32, 32, 32, 32, 0},
                          {0, 33, 33, 33, 33, 33, 0},
                          {0, 34, 34, 34, 34, 34, 0},
                          {0, 35, 35, 36, 37, 37, 0},
                          {0, 0, 0, 0, 0, 0, 0}},
                         {{12, 0, 0, 0, 0, 0, 0},  // 5.消息页面 (待更新 )
                          {0, 47, 47, 47, 47, 47, 0},
                          {0, 38, 38, 38, 38, 38, 0},
                          {0, 39, 39, 39, 39, 39, 0},
                          {0, 40, 40, 40, 40, 40, 0},
                          {0, 41, 41, 41, 41, 41, 0},
                          {0, 42, 42, 42, 42, 42, 0},
                          {0, 43, 43, 43, 43, 43, 0},
                          {0, 44, 44, 44, 44, 44, 0},
                          {0, 45, 45, 45, 45, 45, 0},
                          {0, 46, 46, 46, 46, 46, 0},
                          {0, 0, 0, 0, 0, 0, 0}}};
/*appMap取到的内容对应下面的下标，
信息依次是映射到实际打印处的 x/y/高/宽/类别
类别： 0 无功能项 点击进不去
           1 有功能项 点击进入action
           2 待开发
*/
int appMapMessage[100][5] = {
    {0, 0, 0, 0, 0},
    {1, 2, 2, 40, 1},  // 1-3 顶部信息栏 头像/搜索框/信息
    {3, 2, 3, 49, 1},
    {1, 43, 1, 8, 1},
    {21, 2, 1, 9, 1},   // 4-8 顶部标签功能栏
    {21, 12, 1, 9, 1},  //推荐
    {21, 22, 1, 9, 1},  //热门
    {21, 32, 1, 9, 1},  //
    {21, 42, 1, 9, 1},	
    {6, 1, 6, 51, 1},  // 9-14 6个消息记录列 
    {11, 1, 4, 51, 1},
    {16, 1, 4, 51, 1},
    {21, 1, 4, 51, 1},
    {26, 1, 4, 51, 1},
    {31, 1, 4, 51, 1},
    {35, 1, 7, 51, 1}, // 15 17 底部标签公告 
    {42, 1, 4, 9, 1},
    {42, 2, 4, 9, 1},  // 17-19 底部标签功能栏
    {42, 22, 4, 9, 1},
    {42, 40, 4, 9, 1},
    {6, 2, 35, 49, 0},  // 20.404页面
    {7, 2, 7, 49, 1},  // 21-24. top热门榜页面
    {14, 2, 3, 49, 1},
    {17, 2, 3, 49, 1},
    {23, 2, 15, 49, 1},
    {6, 4, 4, 10, 1},  // 25-28 中间4格栏，热门 信息共用
    {6, 16, 4, 10, 1},
    {6, 27, 4, 10, 1},
    {6, 39, 4, 10, 1},
    {10, 1, 11, 51, 1},  // 29-30 qq空间 
    {23, 1, 17, 51, 1},  
    {8, 20, 4, 18, 1},//31-37 登录界面 
	{14, 2, 3, 49, 1},
    {17, 2, 3, 49, 1},
    {22, 20, 2, 14, 1}, 
    {43, 2, 1, 14, 1},  // 35-37 底部标签功能栏
    {43, 20, 1, 14, 1}, 
    {43, 38, 1, 14, 1}, 
    {3, 1, 4, 51, 1},  // 38-45 6个消息记录列 
    {8, 1, 4, 51, 1},
    {13, 1, 4, 51, 1},
    {18, 1, 4, 51, 1},
    {23, 1, 4, 51, 1},
    {28, 1, 4, 51, 1},
    {33, 1, 4, 51, 1},
    {38, 1, 4, 51, 1},
    {42, 1, 3, 51, 1},  //46 输入框 
    {1, 1, 2, 51, 1}
};
char appMapIcon[100][1000] = {
    " ",
    "〖我是〗 用户名                         "
	"〖头像〗 QQ for Cmd在线 >               ",  // 1-5
    "._______________________________________________."
    "|                    o 搜索                     |"
    "|_____________________\\_________________________|",
    " 添加(+)",
    "〖好友〗 ",
    "〖分组〗 ",
    "〖群聊〗 ",  // 6-10
    "〖设备〗 ", 
    "〖通讯〗 ",
    "._________________________________________________."//9-14
    "|  Img | 暂时没有消息                             |"
    "|  404 |                                          |"
    "|______|__________________________________________|",
    "._________________________________________________."
    "|  Img | 暂时没有消息                             |"
    "|  404 |                                          |"
    "|______|__________________________________________|",
    "._________________________________________________."
    "|  Img | 暂时没有消息                             |"
    "|  404 |                                          |"
    "|______|__________________________________________|",
    "._________________________________________________."
    "|  Img | 暂时没有消息                             |"
    "|  404 |                                          |"
    "|______|__________________________________________|",
    "._________________________________________________."
    "|  Img | 暂时没有消息                             |"
    "|  404 |                                          |"
    "|______|__________________________________________|",
    "._________________________________________________."
    "|  Img | 暂时没有消息                             |"
    "|  404 |                                          |"
    "|______|__________________________________________|",

    "._________________________________________________."
    "|@B站  | 【系统公告】                   2021-11-18|"
    "|绝伦N | - 系统内容：基于加密算法的QQ for Cmd (1) |"
    "|______| - 系统版本：1.0.0 perview            (2) |"
    "|Q:1308| - 软件作者：404name(CTGU_LLZ)        (3) |"
    "|964967| - 涉及内容：RSA、对称加密、MD5       (4) |"
    "|______|__________________________________________|",
	"   ╔―╗"
	"   ┗―┛"
	"   |/    "
	"   联系人",  
	"   ╔―╗"
	"   ┗―┛"
	"    |/   "
	"   消息  ", // 17-20

	"    ()   "
    "    ︵   " 
    "   (  )  "
	"  联系人 ",
    "    ︵☆ "
	"   (  )  "
	"    ︶   "
	"   动态  ",
    " ",
    "#################################################"
    "#可能认识的人 >                                 #"
    "#[IMG ] 马化腾                         [添加] x #"
    "#[404 ] 你可能认识                              #"
    "#[Chin] 特朗普                         [添加] x #"
    "#[a！ ] 你可能认识                              #"
    "#################################################",  // 21-25
    "._______________________________________________."
    "|新朋友|                                      > |"
    "|______|________________________________________|",
    "._______________________________________________."
    "|群通知|                                      > |"
    "|______|________________________________________|",
    "#################################################"
    "# > 特别关心                              5/5 > #"
    "#                                               #"
    "# > 我的好友                           50/100 > #"
    "#                                               #"
    "# > 我的同学                           98/201 > #"
    "#                                               #"
    "# > 其他好友                          300/405 > #"
    "#                                               #"
    "#################################################"
	"                                                 "
	"                                                 "
	"                                                 "
	"                  QQ for Cmd                     "
	"                    V1.0.0                       ",
    " +------+ "
	" |::  三| "
	" +------+ "
	"   相册   ",
	"   ╔―╗ "
	"   ┗―┛ "
	"    |/    "
	"   说说   ",// 26-30
	"    ■    "
	"  ■■    "
	"  ■■■  "
	" 游戏中心 ",
    "     ︵   "
	"    (△)  "
	"     ︶-  "
	"  小视频  ",  
    "._________________________________________________."
    "|  Img | 404name                                  |"
    "|  404 | 2021年11月17日 00:24                     |"
    "|-------------------------------------------------|"
    "| QQ for Cmd 1.0.0版本上线了，点击下方链接下载~   |"
    "| $ https://qq.404name.top                        |"
    "| 浏览 10w次                     点赞|评论|转发   |"
    "| 1.3万人觉得很赞                                 |"
    "| 1981人转发                                      |"
    "| 201条评论回复                                   |"
    "|_________________________________________________|",
    "._________________________________________________."
    "|  Img | 404name                                  |"
    "|  404 | 2021年11月10日 00:24                     |"
    "|-------------------------------------------------|"
    "| 空间功能后期上线，敬请期待                      |"
    "| 浏览 1k次                      点赞|评论|转发   |"
    "| 100人觉得很赞                                   |"
    "| 19人转发                                        |"
    "| 20条评论回复                                    |"
    "|_________________________________________________|"
	"                                                   "
	"                                                   "
	"                                                   "
	"                                                   "
	"                                                   "
	"                  QQ for Cmd                       "
	"                    V1.0.0                         ",
    "    ︵            "
	"   (△)  QQ       "
	"  /(︶)\\ for cmd  "
	"   -︶-  V 1.0.0  ", //31-35 登录注册页面 
    "._______________________________________________."
    "| 账号 |                                      > |"
    "|______|________________________________________|",
    "._______________________________________________."
    "| 密码 |                                      > |"
    "|______|________________________________________|",
    "(登录/注册 ->)",  
    "〖手机号登录〗",  
    " 〖账号密码〗 ", //36-40 
    "〖新用户注册〗",
    ".______.___________________________________.______."//38-45
    "|  Img | 2021-11-21                12:38:18|  Img |"
    "|  404 | hello_world                       |  404 |"
    "|______|___________________________________|______|",
    ".______.___________________________________.______."
    "|  Img | 2021-11-21                12:38:18|  Img |"
    "|  404 | hello_world                       |  404 |"
    "|______|___________________________________|______|",
    ".______.___________________________________.______."
    "|  Img | 2021-11-21                12:38:18|  Img |"
    "|  404 | hello_world                       |  404 |"
    "|______|___________________________________|______|",
    ".______.___________________________________.______."
    "|  Img | 2021-11-21                12:38:18|  Img |"
    "|  404 | hello_world                       |  404 |"
    "|______|___________________________________|______|",
    ".______.___________________________________.______."
    "|  Img | 2021-11-21                12:38:18|  Img |"
    "|  404 | hello_world                       |  404 |"
    "|______|___________________________________|______|",
    ".______.___________________________________.______."
    "|  Img | 2021-11-21                12:38:18|  Img |"
    "|  404 | hello_world                       |  404 |"
    "|______|___________________________________|______|",
    ".______.___________________________________.______."
    "|  Img | 2021-11-21                12:38:18|  Img |"
    "|  404 | hello_world                       |  404 |"
    "|______|___________________________________|______|",
    ".______.___________________________________.______."
    "|  Img | 2021-11-21                12:38:18|  Img |"
    "|  404 | hello_world                       |  404 |"
    "|______|___________________________________|______|",
    "._________________________________________________."
    "|  []  |                                        + |"  //46-47 
    "|______|__________________________________________|",
    "| <                                            三 |"
    "|_________________________________________________|",
};
string GetSystemTime()
{
	SYSTEMTIME m_time;
	GetLocalTime(&m_time);
	char szDateTime[100] = { 0 };
	sprintf(szDateTime, "%02d-%02d-%02d_%02d:%02d:%02d", m_time.wYear, m_time.wMonth,
		m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond);
	string time(szDateTime);
	return time;
}
string replaceBlank(string str){
	for(int i = 0; i < str.size() ; i++){
		if(str[i] == ' ') str[i] = '_';
	}
	return str;
} 
//结构体设计

class Chat
{
    public:
	    string fromUser;  
	    string toUser;  
	    string message;  
		string time;  
    public:
    	
    	static vector<Chat> chatList;
    	// 初始化 
    	static bool updateAndSave(){
    		ofstream osm("chat.txt");
    		for(Chat chat:chatList){
    			osm <<  chat.toString() << endl;
			}
			osm.close();
		}
		static void printAll(){
			for(Chat chat:chatList){
    			cout << chat.toString() << endl;
			}
		}
		static bool init(){
			//初始化用户 
    		for(int i = 0; i < 5; i++){
    			chatList.push_back(Chat("admin","QQ官方","hello world"));
			}
		}
		static void addChat(string from,string to,string msg){
			chatList.push_back(Chat(from,to,msg));
			updateAndSave();
		}
    	static bool load(){
    		ifstream ism("chat.txt");
			if(!ism) 
				return false;
    		string s;
			while(getline(ism,s))//着行读取数据并存于s中，直至数据全部读取
				chatList.push_back(Chat(s)); 
			ism.close();
		}
		Chat(){}
    	Chat(string from,string to,string msg){
			fromUser = replaceBlank(from);
			toUser = replaceBlank(to);
			if(msg.size() >= 30){
				msg = msg.substr(0,30) + "...";
			}
			message = replaceBlank(msg);
			time = GetSystemTime();
		}
		Chat(string str) {
			stringstream input(str);
			input >> this->fromUser;
			input >> this->toUser;
			input >> this->message;
			input >> this->time;
		}
        string toString() { 
			return fromUser+" "+toUser+" " + message + " " + time;
		} 
        
};
vector<Chat> Chat::chatList = vector<Chat>();
map<string,vector<Chat>> chatHistory;
string chatHis[6][4]; 
class User
{
    public:
	    string username;
	    string pwdmd5;
    public:
    	static vector<User> userList;
    	// 初始化角色 
    	static bool updateAndSave(){
    		ofstream osm("user.txt");
    		for(User user:userList){
    			osm <<  user.toString() << endl;
			}
			osm.close();
		}
		static void printAll(){
			for(User user:userList){
    			cout << user.toString() << endl;
			}
		}
		static bool init(){
			//初始化用户 
    		for(int i = 0; i < 10; i++){
    			userList.push_back(User(to_string(i),"admin"));
			}
		}
		static bool loginOrRegister(string u,string p){
			//如果有账号就检查 
			for(User user:userList){
				if(user.getName() == u){
					return user.getPwd() == p;
				}
			}
			// 没有就注册
			userList.push_back(User(u,p));
			//持久化到本地 
			Chat::addChat("QQ官方",u,"欢迎使用QQ for CMD"); 
			updateAndSave();
			return true;
		}
    	static bool load(){
    		ifstream ism("user.txt");
    		string s;
			while(getline(ism,s))//着行读取数据并存于s中，直至数据全部读取
				userList.push_back(User(s)); 
			ism.close();
		}
        User(string u,string p):username(u),pwdmd5(p) {
			//后期对p加密 
			this->username = replaceBlank(u);
			this->pwdmd5 = replaceBlank(p);
		}
		User(string str) {
			stringstream input(str);
			input >> this->username;
			input >> this->pwdmd5;
		}
        User() { }
        string toString() { 
			return username+" "+pwdmd5;
		} 
		string getName(){
			return this->username;
		}
		string getPwd(){
			return this->pwdmd5;
		}
};
vector<User> User::userList = vector<User>(); 
User toUser;
User nowUser;
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
                            index >= 21 && index <= 24) {
                            setColor(Video_Color);
                        }  //视频的样式
                        /*else if (index >= 15 && index <= 19 ||
                                 index >= 25 && index <= 28 ||
                                 index >= 37 && index <= 41 || index == 35) {
                            setColor(Is_Selected3_Color);
                        }  //一些错位字符的样式*/ 
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
            if(i == h - 1 && j == w - 1){
            	if (on) { 
	            	if (index >= 9 && index <= 14 ||
                        index >= 21 && index <= 24) {
                        setColor(Video_Color);
                    }else{
						setColor(Is_Selected1_Color);                    	
					}
				}
            	if(index == 1){
					gotoxy(11,1);
					cout << nowUser.getName() << "    ";
				}else if(index == 47){
					gotoxy(11,1);
					cout << toUser.getName() << "    ";
				} 
				else if(index == 32){
					gotoxy(22, 15);
					if(username != "")
				    cout << "【" << username << "】"; 
				}else if(index == 33){
					gotoxy(22, 18);
					for(int i = 0; i < pwd.size(); i++){
						cout << '*';
					}
				}else if(index >= 9 && index <= 14){
        			// 判断长度 
		        	if(index-9 < indexCnt){
		        		//打印名字
			        	gotoxy(y + 9, x + 1);
			        	cout << chatHis[index-9][0] << "           "; 
						//打印内容
						gotoxy(y + 9, x + 2);
						cout << "                                  ";
						gotoxy(y + 9, x + 2);
			        	cout << chatHis[index-9][1]; 
						//打印时间
						gotoxy(y + 40, x + 1);
			        	cout << chatHis[index-9][2].substr(0,10); 
						//打印消息数 
			        	gotoxy(y + 44, x + 2);
			        	cout << "(" << chatHis[index-9][3] << ")"; 
					}
				}else if(index >= 38 && index <= 45){
					int itemIndex = index - 38 + startItem;
					if(itemIndex > endItem){
						//制空
						for(int i = 0; i < 4; i++){
							gotoxy(y, x + i);                                       
							printf("                                                   ");
						}
					}else{
						Chat chat = chatHistory[toUser.username][itemIndex];
						//打印时间 
						gotoxy(y + 9,x+1);
						cout << chat.time.substr(0,10);
						gotoxy(y + 35,x+1);
						cout << chat.time.substr(11,8);
						//打印对话
						gotoxy(y + 9,x+2);
						cout << "                      ";
						gotoxy(y + 9,x+2);
						cout << chat.message;
						//删除边框 
						int overFlow = 0;
						if(chat.toUser == nowUser.username){
							overFlow = 44;
						}
						gotoxy(y + overFlow,x+0);
						cout << "       ";
						gotoxy(y + overFlow,x+1);
						cout << "       ";
						gotoxy(y + overFlow,x+2);
						cout << "       ";
						gotoxy(y + overFlow,x+3);
						cout << "       ";
					}
					
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
void loadChat(){ //要先登录 
	//LRU算法拉取最新6条记录 
	stack<string> sta;
	chatHistory.clear();
	for(Chat chat:Chat::chatList){
    	if(chat.fromUser == nowUser.getName()){ //获取发送的 
    		if(!chatHistory.count(chat.toUser)){
    			sta.push(chat.toUser);
    			vector<Chat> temp;
    			temp.push_back(chat);
    			chatHistory[chat.toUser] = temp;
			}else{
				//删除栈中元素 
				stack<string> temp;
				while(sta.top() != chat.toUser){
					temp.push(sta.top());
					sta.pop();
				}
				sta.pop();
				while(temp.size()){
					sta.push(temp.top());
					temp.pop();
				}
				sta.push(chat.toUser);
				chatHistory[chat.toUser].push_back(chat);
			}
		}else if(chat.toUser == nowUser.getName()){ //获取接收的 
    		if(!chatHistory.count(chat.fromUser)){
    			sta.push(chat.fromUser);
    			vector<Chat> temp;
    			temp.push_back(chat);
    			chatHistory[chat.fromUser] = temp;
			}else{
				//删除栈中元素 
				stack<string> temp;
				while(sta.top() != chat.fromUser){
					temp.push(sta.top());
					sta.pop();
				}
				sta.pop();
				while(temp.size()){
					sta.push(temp.top());
					temp.pop();
				}
				sta.push(chat.fromUser); 
				chatHistory[chat.fromUser].push_back(chat);
			}
		}
	}
	int now = 0;
	while(now < 6 && sta.size()){
		string uname = sta.top();
		sta.pop();
		chatHis[now][0] = uname;
		chatHis[now][1] = chatHistory[uname][chatHistory[uname].size()-1].message;
		chatHis[now][2] = chatHistory[uname][chatHistory[uname].size()-1].time;
		chatHis[now][3] = to_string(chatHistory[uname].size());
		now++;
	}
	indexCnt = now; 
	while(now < 6){
		chatHis[now][0] = "";
		chatHis[now][1] = "";
		chatHis[now][2] = "";
		chatHis[now][3] = "";
		now++;
	}
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
void error(string str,int x){
	gotoxy(18,x-1);
	printf("+------------------+"); 
	gotoxy(18,x); 
	cout <<"+------------------+"; 
	gotoxy(19,x); 
	cout << str; 
	gotoxy(18,x+1); 
	printf("+------------------+"); 
}
void checkAction(int action) {  //引入路由跳转
	int prePage = nowPage;
	if (action == 1){ //退出返回登陆界面 
		isLogin = false;
		while(pageStack.size()) pageStack.pop();
		nowPage = 4;
		pageStack.push(nowPage);
	}
	else if (action == 2 || action == 3) {  //搜索栏
        gotoxy(22, 4);
        setColor(Is_Selected1_Color);
        printf("                         ");
        gotoxy(22, 5);
        printf("_________________");
        char searchMsg[100];
        gotoxy(22, 4);
        scanf("%s", searchMsg);
        setColor(Normal_Color);
		//处理搜索消息 
		int flag = 0;
		for(User user:User::userList){
			if(user.getName() == searchMsg){
				toUser = user;
				flag = 1;
				break;
			}
		}
		if(flag == 0){
			error("未找到用户",4); 
		}else{
			int size = chatHistory[toUser.username].size();
			endItem = size - 1;
			if(endItem < 0) endItem = -1;
			startItem = endItem - 7;
			if(startItem < 0) startItem = 0;
			nowPage = 5;
		}
		//找用户 跳转聊天界面 
        
    }else if(action == 32){ //账号 
    	gotoxy(22, 15);
        setColor(Is_Selected1_Color);
        printf("|                         ");
        char searchMsg[100];
        username = "";
        gotoxy(22, 15);
        cin >> username;
        gotoxy(22, 15);
        cout << "【" << username << "】"; 
	}else if(action == 33){ //密码 
    	gotoxy(22, 18);
        setColor(Is_Selected1_Color);
        printf("                          ");
        int len = 0;
        char searchMsg[100];
        gotoxy(22, 18);
		printf("|");
        pwd = "";
        while(1){
        	char ch = getch();
        	if(ch == 13){
        		break;
			}else if(ch == 8){
				if(len >= 1){
					gotoxy(22+len, 18);
					printf("|%c",' ');
					len--;
					pwd.pop_back();
				}
			}else{
				pwd += ch;
				if(len > 0){
					gotoxy(22+len, 18);
					printf("%c",'*');
				}
				len++;
				gotoxy(22+len, 18);
				printf("%c|",ch);
			}
		
		}
        
	}
	else if(action == 46){ //发送信息 
    	gotoxy(10, 43);
        setColor(Is_Selected1_Color);
        printf("|                        ");
        char msg[100];
        gotoxy(10, 43);
        cin >> msg;
        Chat::addChat(nowUser.getName(),toUser.getName(),msg);
        loadChat();
		int size = chatHistory[toUser.username].size();
		endItem = size - 1;
		startItem = endItem - 7;
		if(startItem < 0) startItem = 0;
        loadPage(); 
	}
    else if (action == 3) {          //消息栏
        nowPage = 0;
    } else if (action == 5) {  //首页
        nowPage = 1;
    } else if (action == 6) {  //热门
        nowPage = 2;
    } else if ((action >= 9 && action <= 14)) {
    	int index = action - 9; 
    	//找到目标聊天用户 
    	if(chatHis[index][0] == ""){
    		error("找不到该用户",4);
		} else{
			toUser.username = chatHis[index][0];
			int size = chatHistory[toUser.username].size();
			endItem = size - 1;
			startItem = endItem - 7;
			if(startItem < 0) startItem = 0;
        	nowPage = 5; 
		}
    	                           //进入聊天详细页 
    } else if (action >= 17 && action <= 19) {  //底层标签栏
        if (action == 17) {
            activeIndexBottom = 17;  //响应变成首页
            activeIndexTop = 5;
            nowPage = 1;  //没写界面先放首页 后面就nowPage要对应内容
        } else if (action == 18) {
            activeIndexBottom = 18;  //响应变成联系人 
            activeIndexTop = 5;
            nowPage = 2;  //没写界面先放首页 后面就nowPage要对应内容
        } else if (action == 19) {
            activeIndexBottom = 19;  //响应变成动态 
            activeIndexTop = 5;
            nowPage = 3;  //没写界面先放首页 后面就nowPage要对应内容
        }else
            nowPage = 0;
    }  else if (action == 29) {  //点击视频模块  则对应停止视频
        if (!stopPlay)
            stopPlay = true;
        else
            stopPlay = false;
    } else if(action == 34){ //登录模块 
    	if(username == ""){
    		error("账号不能为空",26); 
		}else if(pwd == ""){
    		error("密码不能为空",26); 
		}else if(User::loginOrRegister(username,pwd)){
    		isLogin = true; 
    		pageStack.pop(); 
	    	nowPage = 1;
	    	pageStack.push(nowPage);
	    	for(User user:User::userList){
	    		if(user.getName() == username){
	    			nowUser = user;
				}
			}
			loadChat();
		}else{
			//登录失败密码匹配错误
			error("账号密码输入错误",26); 
		} 
    	
	}else if(action == 47){
		pageStack.pop();
    	nowPage = pageStack.top(); 
	}else {
        nowPage = 0;  // 没写的界面404
    }
    activeIndexTop = action;  //判断完再切回来
    if(nowPage != prePage){
    	pageStack.push(nowPage);
	}
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
        } else {  //返回 
            pageStack.pop();
			if(pageStack.size() == 0){ //到顶了 
				if(isLogin){ //看看是否登录 
					pageStack.push(1); //首页 
				}else{
					pageStack.push(4); //登录页 
				}
			}
            nowPage = pageStack.top();  //回到首页
        }

        if (nowPage != oldPage) {  //界面变化
            oldPage = nowPage;
            x = 1;
            y = 1;
            clsPage();   //除了边框全部擦除
            loadPage();  //加载新界面
        }
    }
    
    if(nowPage == 5 && (turn == 'a' || turn == 'd')){
    	//0-11 
		//0-7 下一页是 4-11  上一页是0-7 
    	int size = chatHistory[toUser.username].size(); 
    	//区间滑动翻页算法  
    	if(turn == 'a'){
    		startItem -= 8;
    		endItem -= 8;
    		if(startItem < 0){
    			startItem = 0;
    			endItem = startItem + 7;
			}
			if(endItem >= size){
				endItem = size - 1;
			} 
		}else{
			startItem += 8;
    		endItem += 8;
    		if(endItem >= size){
    			endItem = size - 1;
    			startItem = endItem - 7;
			}
			if(startItem < 0){
				startItem = 0;
			}
		}
		x = 1;
        y = 1;
        clsPage();   //除了边框全部擦除
        loadPage();  //加载新界面
	}
}

void init() {
	User::load();
	Chat::load();
	getchar();
	getchar();
	/*User::load();
	Chat::load();*/
    char cmd[50];
    pageStack.push(nowPage);
    sprintf(cmd, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
    printf("%s", cmd);
    system(cmd);         //设置界面大小
    system("color 07");  //设置界面颜色。

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
        if (kbhit()) {
            // debug(); //开启debug就会显示坐标信息，并且注释move()
            move();
        }
    }
}
// 7  - 41
