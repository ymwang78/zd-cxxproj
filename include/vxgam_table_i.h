// ***************************************************************
//  vxgam_table_i   version:  1.0 - date: 14/2/2013
//  -------------------------------------------------------------
//  Hangzhou Zhidu Network Ltd.
//  Administrator(admin@feicha.net)
//  -------------------------------------------------------------
//  Copyright (C) 2012 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef __vxgam_table_i_h__
#define __vxgam_table_i_h__

#include <ubeda/usys_smartptr.h>

static const int INVALID_TABLE = -1;

static const int INVALID_CHAIR = -1;

enum ERV_UTS
{
    ERV_UTS_NONE,
    ERV_UTS_PLAYER,
    ERV_UTS_LOOKON,
};

//开始模式
enum ERV_STARTMODE
{
    ERV_STARTMODE_FULLREADY,			//满人开始
    ERV_STARTMODE_ALLREADY,				//所有准备
    ERV_STARTMODE_SYMMETRY,				//对称开始
    ERV_STARTMODE_TIMERCONTROL,			//时间控制
};

//分数类型
enum ERV_SCOREKIND
{
    ERV_SCOREKIND_WIN,					//胜
    ERV_SCOREKIND_LOST,					//输
    ERV_SCOREKIND_DRAW,					//和
    ERV_SCOREKIND_FLEE,					//逃
    ERV_SCOREKIND_SERVICE,				//服务
    ERV_SCOREKIND_PRESENT,				//赠送
};

enum ERV_GER
{
    ERV_GER_NORMAL,                     //常规结束
    ERV_GER_DISMISS,                    //游戏解散
    ERV_GER_USER_LEFT,                  //用户离开
};

enum ERV_GS
{
    GS_FREE,                            //空闲状态
    GS_PLAYING = 100,                   //游戏状态
};

enum ERV_SMT
{
    ERV_SMT_INFO = 1,
    ERV_SMT_EJECT = 2,
    ERV_SMT_GLOBAL = 4,
    ERV_SMT_CLOSE_GAME = 0x10000,
};

enum ERV_MAINCMD
{
    ERV_MAINCMD_TABLEDATA = 1,
    ERV_MAINCMD_LOOKONDATA,
    ERV_MAINCMD_GAMEMESSAGE,
};

class vxgam_user_i;

//房间框架提供给具体游戏的回调接口
class vxrom_table_i
{
public:
    virtual vxgam_user_i * get_player(short chairidx) = 0;

    virtual void set_gamestatus(int gs) = 0;

    virtual int get_gamestatus() = 0;

    virtual int send_game_data(vxgam_user_i * user_ptr, int maincmd, int subcmd, void* data, int size) = 0;

    virtual int send_data(short chairidx, unsigned short msgtype, unsigned short cps, unsigned short ie_num, void* data, int size) = 0;

    virtual int broadcast(ERV_UTS, unsigned short msgtype, unsigned short cps, unsigned short ie_num, void* data, int size) = 0;

    virtual bool ConcludeGame() = 0;

    virtual int SendTableData(short chairidx, short subcmd, void* data, int size) = 0;

    virtual int SendLookonData(short chairidx, short subcmd, void* data, int size) = 0;

    virtual bool SendGameMessage(short chairidx, const char* message, short msgtype) = 0;

    virtual bool WriteScore(uidl_int64* vec_useridx, uidl_int64* vec_score, unsigned size) = 0;
};

//游戏提供给房间框架的调用接口
class vxgam_table_i
{
public:
    virtual ERV_STARTMODE get_startmode() = 0;

    virtual void on_init(vxrom_table_i*) = 0;

    virtual void on_user_offline(short chairidx, vxgam_user_i* puser) = 0;

    virtual void on_user_reconnect(short chairidx, vxgam_user_i* puser) = 0;

    virtual void on_user_sitdown(short chairidx, vxgam_user_i* puser, bool islook) = 0;

    virtual bool on_user_standup(short chairidx, vxgam_user_i* puser, bool islook) = 0;

    virtual void on_user_ready(short chairidx, vxgam_user_i* puser, void* data, unsigned datasize) = 0;

    virtual bool on_game_start() = 0;

    virtual bool on_game_end(short wChairID, vxgam_user_i * pvxgam_user_i, unsigned char cbReason) = 0;
};

class vxgam_user_i
{
public:
    virtual bool is_robot() const = 0; //是否机器人

    virtual uidl_int64 get_score() const = 0;

    virtual uidl_int64 uid() const = 0;

    virtual short get_chairidx() const = 0;

    virtual unsigned char get_status() const = 0;

    virtual rv::ua_game_t& get_game_t() = 0;
};

//房间提供给游戏的机器人接口
class vxrom_robot_i
{
    //状态函数
public:
    //获取状态
    virtual unsigned char get_gamestatus()=0;
    //设置状态
    virtual void set_gamestatus(unsigned char cbGameStatus)=0;

    //信息接口
public:
    //获取 I D
    virtual uidl_int64 GetUserID()=0;
    //桌子号码
    virtual short GetTableID()=0;
    //椅子号码
    virtual short GetChairID()=0;

    //功能接口
public:
    //获取自己
    virtual vxgam_user_i * GetMeUserItem()=0;
    //游戏用户
    virtual vxgam_user_i * GetTableUserItem(short wChariID)=0;

    //网络接口
public:
    //发送函数
    virtual bool SendSocketData(short wSubCmdID)=0;
    //发送函数
    virtual bool SendSocketData(short wSubCmdID, void * pData, short wDataSize)=0;

    //功能接口
public:
    //删除时间
    virtual bool KillGameTimer(unsigned nTimerID)=0;
    //设置时间
    virtual bool SetGameTimer(unsigned nTimerID, unsigned nElapse)=0;
    //发送准备
    virtual bool SendUserReady(void * pData, short wDataSize)=0;
    //发送聊天
    virtual bool SendChatMessage(int dwTargetUserID, const char* pszChatMessage, int msgtype)=0;
};


//游戏机器人实现接口
class vxgam_robot_i
{
    //控制接口
public:
    //初始接口
    virtual bool InitUserItemSink()=0;
    //重置接口
    virtual bool RepositUserItemSink()=0;

    //游戏事件
public:
    //时间消息
    virtual bool OnEventTimer(unsigned nTimerID)=0;
    //游戏消息
    virtual bool OnEventGameMessage(short wSubCmdID, void * pData, short wDataSize)=0;
    //游戏消息
    virtual bool OnEventFrameMessage(short wSubCmdID, void * pData, short wDataSize)=0;
    //场景消息
    virtual bool OnEventGameScene(unsigned char cbGameStatus, bool bLookonOther, void * pData, short wDataSize)=0;

    //用户事件
public:
    //用户进入
    virtual void OnEventUserEnter(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
    //用户离开
    virtual void OnEventUserLeave(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
    //用户积分
    virtual void OnEventUserScore(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
    //用户状态
    virtual void OnEventUserStatus(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
    //用户段位
    virtual void OnEventUserSegment(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
};


#endif // __vxgam_table_i_h__
