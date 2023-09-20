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

//��ʼģʽ
enum ERV_STARTMODE
{
    ERV_STARTMODE_FULLREADY,			//���˿�ʼ
    ERV_STARTMODE_ALLREADY,				//����׼��
    ERV_STARTMODE_SYMMETRY,				//�Գƿ�ʼ
    ERV_STARTMODE_TIMERCONTROL,			//ʱ�����
};

//��������
enum ERV_SCOREKIND
{
    ERV_SCOREKIND_WIN,					//ʤ
    ERV_SCOREKIND_LOST,					//��
    ERV_SCOREKIND_DRAW,					//��
    ERV_SCOREKIND_FLEE,					//��
    ERV_SCOREKIND_SERVICE,				//����
    ERV_SCOREKIND_PRESENT,				//����
};

enum ERV_GER
{
    ERV_GER_NORMAL,                     //�������
    ERV_GER_DISMISS,                    //��Ϸ��ɢ
    ERV_GER_USER_LEFT,                  //�û��뿪
};

enum ERV_GS
{
    GS_FREE,                            //����״̬
    GS_PLAYING = 100,                   //��Ϸ״̬
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

//�������ṩ��������Ϸ�Ļص��ӿ�
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

//��Ϸ�ṩ�������ܵĵ��ýӿ�
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
    virtual bool is_robot() const = 0; //�Ƿ������

    virtual uidl_int64 get_score() const = 0;

    virtual uidl_int64 uid() const = 0;

    virtual short get_chairidx() const = 0;

    virtual unsigned char get_status() const = 0;

    virtual rv::ua_game_t& get_game_t() = 0;
};

//�����ṩ����Ϸ�Ļ����˽ӿ�
class vxrom_robot_i
{
    //״̬����
public:
    //��ȡ״̬
    virtual unsigned char get_gamestatus()=0;
    //����״̬
    virtual void set_gamestatus(unsigned char cbGameStatus)=0;

    //��Ϣ�ӿ�
public:
    //��ȡ I D
    virtual uidl_int64 GetUserID()=0;
    //���Ӻ���
    virtual short GetTableID()=0;
    //���Ӻ���
    virtual short GetChairID()=0;

    //���ܽӿ�
public:
    //��ȡ�Լ�
    virtual vxgam_user_i * GetMeUserItem()=0;
    //��Ϸ�û�
    virtual vxgam_user_i * GetTableUserItem(short wChariID)=0;

    //����ӿ�
public:
    //���ͺ���
    virtual bool SendSocketData(short wSubCmdID)=0;
    //���ͺ���
    virtual bool SendSocketData(short wSubCmdID, void * pData, short wDataSize)=0;

    //���ܽӿ�
public:
    //ɾ��ʱ��
    virtual bool KillGameTimer(unsigned nTimerID)=0;
    //����ʱ��
    virtual bool SetGameTimer(unsigned nTimerID, unsigned nElapse)=0;
    //����׼��
    virtual bool SendUserReady(void * pData, short wDataSize)=0;
    //��������
    virtual bool SendChatMessage(int dwTargetUserID, const char* pszChatMessage, int msgtype)=0;
};


//��Ϸ������ʵ�ֽӿ�
class vxgam_robot_i
{
    //���ƽӿ�
public:
    //��ʼ�ӿ�
    virtual bool InitUserItemSink()=0;
    //���ýӿ�
    virtual bool RepositUserItemSink()=0;

    //��Ϸ�¼�
public:
    //ʱ����Ϣ
    virtual bool OnEventTimer(unsigned nTimerID)=0;
    //��Ϸ��Ϣ
    virtual bool OnEventGameMessage(short wSubCmdID, void * pData, short wDataSize)=0;
    //��Ϸ��Ϣ
    virtual bool OnEventFrameMessage(short wSubCmdID, void * pData, short wDataSize)=0;
    //������Ϣ
    virtual bool OnEventGameScene(unsigned char cbGameStatus, bool bLookonOther, void * pData, short wDataSize)=0;

    //�û��¼�
public:
    //�û�����
    virtual void OnEventUserEnter(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
    //�û��뿪
    virtual void OnEventUserLeave(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
    //�û�����
    virtual void OnEventUserScore(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
    //�û�״̬
    virtual void OnEventUserStatus(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
    //�û���λ
    virtual void OnEventUserSegment(vxrom_robot_i * pIAndroidUserItem, bool bLookonUser)=0;
};


#endif // __vxgam_table_i_h__
