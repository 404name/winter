#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

bool skipFrame = true; //开启回车跳过 

/**
 *  华强买瓜内部源码泄露
 *
 *	程序--绝伦N
 */
// 构建场景ing
class Sence {
   public:
    //场景高度
    const static int WIDTH = 201;
    const static int HEIGHT = 76;
    //当前场景
    static string nowSceneFrame[HEIGHT];
    //演员姓名日志
    static string roleName;
    //演员情绪日志
    static string emotion;
    //演员行动日志
    static string action;
    // 演员台词日志
    static string dialogue;
    // 延迟时间
	static int delayTime; 
    //加载场景
    static void loadFrame(string sName) {
        std::ifstream readFile("./sence/" + sName + ".txt");
        if (readFile)  //读取成功
        {
            string str;
            int fileHeight = 0;
            int firstLine = 0; 
            while (getline(readFile, str)) {
            	firstLine++;
            	if(firstLine == 1){ // 第一行获取运行时间 
            		delayTime = atoi(str.c_str());
            		continue;
				}
                nowSceneFrame[fileHeight++] = str;
            }
        } else  //读取失败
        {
            cout << "Open file faild." << endl;
            for (int i = 0; i < HEIGHT; i++) {
                nowSceneFrame[i] = sName;
            }
        }
    }
    //播放场景
    static void play(string sName) {
        loadFrame(sName);
        system("cls");
        for (int i = 0; i < HEIGHT; i++) {
            cout << nowSceneFrame[i] << endl;
        }
        cout << "【sence】" + sName << endl;
        log();
        if(skipFrame){
        	getchar();
		} else{
        	Sleep(delayTime);			
		}

    }
    // log打印当前场景日志
    static void log() {
        cout << "【log】" << roleName << emotion << action << endl;
        //打印后清空
        roleName = "";
        emotion = "";
        action = "";
    }
};
string Sence::nowSceneFrame[Sence::HEIGHT] = {"初始化"};
string Sence::action = "初始化";
string Sence::emotion = "初始化";
string Sence::roleName = "初始化";
int Sence::delayTime = 1000;
// 构建演员ing
class Actor {
   public:
    //演员姓名
    string actorName;
    //角色姓名
    string roleName;
    // 【定义方法-链式编程】
    //做点什么 [预留代码处理action，写入Sence日志]
    Actor& to(string action) { Sence::action = action; }
    //情绪改变 [预留代码处理emotion，写入Sence日志]
    Actor& be(string emotion) {
        // Sence::roleName = this->roleName;
        Sence::emotion = emotion;
    }
    //讲话
    Actor& say(string dialogue) { Sence::play(dialogue); }
    // 画外音
    Actor& hear(string dialogue) { Sence::play(dialogue); }
    // 开始表演
    Actor& start() { Sence::roleName = this->roleName; }
    Actor(string roleName) { this->roleName = roleName; }
};

// 导演模式 开启
// 指导语言 C语言
// sence备注 为剧本
// 程序内容 为演员友情出演
int main() {
	getchar();
    //演员表
    Actor HuaQiang("刘华强");
    Actor XianHui("贤惠的水果摊老板");
    Actor SaRiLa("老板小弟-萨日朗");
    Actor ZhangYuBrother("小弟二号-章鱼哥");
    Actor EiWhatsUp("欸华强一家");

    // sence01 旁白道来华强上
    Sence::play("有一个人前来买瓜");
	getchar();
	getchar();
    // sence02 华强骑车买西瓜
    HuaQiang.start()
        .be("绷着个脸")
        .to("骑着车来");

    // sence03 停车闻到生意刑
    HuaQiang.start()
        .to("停好车")
        .hear("生意刑吗");

    // sence04 华强询问哥们价
    HuaQiang.start()
        .be("冷酷的")
        .to("走向老板")
        .say("瓜多少钱一斤");

    // sence05 老板答曰两块钱
    XianHui.start()
        .be("悠然的")
        .to("回答道")
        .say("两块钱一斤");

    // sence06 华强直呼WhatsUp
    HuaQiang.start()
        .be("嬉皮笑脸")
        .to("回应道")
        .say("我爱你")
        .say("瓜皮子是金子做的还是");

    // sence07 老板吐烟大棚瓜
    XianHui.start()
        .be("不屑的")
        .to("吐了口烟")
        .say("你瞧瞧这现在哪有瓜呀")
        .say("这都是大棚的瓜");

    // sence08 老板称赞强贤惠
    //         小弟呆呆杵原地
    XianHui.start()
        .to("手指指点点")
        .say("你贤惠我还贤惠呢");

    ZhangYuBrother.start()
        .be("呆呆的")
        .to("杵在原地");

    // sence09 华强笑曰来一个
    HuaQiang.start()
        .be("和气的")
        .to("抬头答道")
        .say("给我挑一个");

    // sence10 贤惠老板去挑瓜
    XianHui.start()
        .be("小眼一笑")
        .say("行")
        .to("转身拿西瓜并拍了拍你")
        .say("这个怎么样");

    // sence11 华强询问瓜保熟
    HuaQiang.start()
        .to("双手一张")
        .say("这瓜保熟吗");

    // sence12 老板冷笑卖生瓜
    XianHui.start()
        .be("冷笑的")
        .to("回答道")
        .say("我开水果摊的能卖给你生瓜蛋子");

    // sence13 梅开二度瓜保熟
    HuaQiang.start()
        .be("加强语气的")
        .to("回复道")
        .say("我问你这瓜保熟吗");

    // sence14 老板惊诧望呆呆
    Sence::play("奏乐");
    XianHui.start()
        .be("惊诧的")
        .to("愣愣的看着" + HuaQiang.roleName);

    // sence15 老板急了要不要
    XianHui.start()
        .be("尖锐的")
        .to("问到")
        .say("你是故意找茬是不是")
        .say("你要不要吧");

    // sence16 嬉皮笑脸熟就要
    HuaQiang.start()
        .be("嬉皮笑脸的")
        .to("看着老板")
        .say("你这瓜要是熟我肯定要啊")
        .be("边说边走的")
        .to("到卖瓜坐的椅子上坐下")
        .say("那它要是不熟怎么办呀");

    // sence17 要是不熟我吃它
    XianHui.start()
        .to("手指着西瓜")
        .say("要是不熟我自己吃了它");

    // sence18 瓜十五斤三十块
    XianHui.start()
        .to("拨弄着秤盘")
        .say("十五斤三十块");

    // sence19 华强冷笑有问题
    HuaQiang.start()
        .be("拽拽的坐着")
        .to("冷笑道")
        .say("你这哪儿够十五斤哪")
        .say("你这秤有问题呀");

    // sence20 急了急了他急了
    XianHui.start()
        .say("你这是故意找茬是不是")
        .be("气急败坏的")
        .to("把瓜放下")
        .say("你要不要");

    // sence21 华强翻秤劈西瓜
    HuaQiang.start()
        .to("把秤翻开")
        .say("吸铁石")
        .say("另外你说的")
        .say("这瓜要是生的你自己吞进去")
        .to("拿刀批西瓜");

    // sence22 老板愤怒被反杀
    XianHui.start()
        .be("愤怒的")
        .to("冲向" + HuaQiang.roleName)
        .say("你TM劈我瓜是吧");

    // sence23 横刀指向撒日朗
    SaRiLa.start()
        .be("面无表情的")
        .to("喊着杀人啦并冲向" + HuaQiang.roleName)
        .say("撒日朗");

    HuaQiang.start()
        .to("小刀一甩");
    // sence24 原地求救章鱼哥
    ZhangYuBrother.start()
        .to("抱着受伤的老板")
        .say("章鱼哥");

    HuaQiang.start()
        .to("帅气的骑车离去")
		.say("拜拜"); 
		
    // sence25 回眸一笑诶华强
    EiWhatsUp.start()
        .be("开心的")
        .to("打着招呼")
        .say("诶华强");

    HuaQiang.start()
        .be("开心的")
        .to("回眸一笑扬长去");
        
    Sence::play("华强退场");

    // sence26 剧情结束

    // sence27 DLC剧本
    // 开发中ing
}
