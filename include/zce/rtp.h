#pragma once
// ***************************************************************
//  rtp.h   version:  1.0   -  date: 5/20/2016
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// RTP_MAXDATA <= MTU - 48
// ***************************************************************
#ifndef __rtp_h__
#define __rtp_h__

#define RTP_HEADER_LEN  12
#define RTP_VERSION        2
#define RTP_MAXDATA        1452
#define RTP_MAXPAYLOAD  (RTP_MAXDATA - 12 - 4)
#define RTP_PAYLOAD_MAX_VALID 2

#define RTP_EXTV1_PROFILE   0x776e
#define RTP_EXTV1_PROFILE2  0x776f
#define RTP_EXTV1_LEN       20
#define RTP_EXTV1_CTRLHEAD_LEN 24
#define RTP_GUID_LEN        (16)

#define RTP_OVER_TCP_HEAD_LEN                    (4)
#define RTP_OVER_TCP_MAGIC                        (0x7E7E)
#define RTP_OVER_TCP_MAGIC_HI                    (0x7E)
#define RTP_OVER_TCP_MAGIC_LO                    (0x7E)

//几种标准类型
enum ENCODE_TYPE_E
{
    ENCODE_TYPE_G711A = 0x8,
    ENCODE_TYPE_N264 = 0x60,
    ENCODE_TYPE_H264 = 0x62, //98;
    ENCODE_TYPE_H265 = 0x43, //67
    ENCODE_TYPE_OPUS = 0x44, //68
    ENCODE_TYPE_WMA1 = 0x45, //69
    ENCODE_TYPE_AAC  = 0x66, //102
    ENCODE_TYPE_FEC  = 0x46, //70

    ENCODE_TYPE_DATA = 0x70, //121
    ENCODE_TYPE_CTRL = 0x7A, //122
};

enum RTP_CTRL_E
{
    RTP_CTRL_UNKNOWN,
    RTP_CTRL_CHNL_ACTIVE,    //通道激活，保活
    RTP_CTRL_CHNL_CLOSE,     //通道关闭

    RTP_CTRL_DATA_PAUSE = 0x10,
    RTP_CTRL_DATA_CONTINUE,
    RTP_CTRL_DATA_IFRAMEREQ,    //请求只传递I帧
    RTP_CTRL_DATA_IFRAMEACK,    //响应只传递I帧
    RTP_CTRL_DATA_AFRAMEREQ,    //请求传递所有帧
    RTP_CTRL_DATA_AFRAMEACK,    //响应传递所有帧

    RTP_CTRL_CHNL_IDREQ = 0x20, //请求获取分配通道ID
    RTP_CTRL_CHNL_IDACK,        //通道IDACK
    RTP_CTRL_CHNL_ACTREQ,       //通道注册激活，保活
    RTP_CTRL_CHNL_ACTACK,       //响应

    RTP_CTRL_STRM_CONNREQ = 0x30, //建立虚链接
    RTP_CTRL_STRM_CONNACK,
    RTP_CTRL_STRM_CLOSEREQ,       //关闭虚链接
    RTP_CTRL_STRM_CLOSEACK,
    RTP_CTRL_STRM_KPALIVEREQ,     //保活
    RTP_CTRL_STRM_KPALIVEACK,
    RTP_CTRL_STRM_ARQREQ,         //重传请求
    RTP_CTRL_STRM_ARQACK,         //重传请求,NOT IMPLEY
    RTP_CTRL_STRM_DATAACK,        //收包确认

    RTP_CTRL_TASK_TESTCONREQ = 0x50,     //测试连接请求
    RTP_CTRL_TASK_TESTCONACK,     //测试连接响应和报告

    //followed req can be safely ignored if not support yet
    RTP_CTRL_REQ_UNKNOW = 0x100,
    RTP_CTRL_REQ_REPORT,         //统计报告
    RTP_CTRL_REQ_ARQ,            //重传请求
    RTP_CTRL_REQ_KEYFRAME,       //请求关键帧

};

enum RTP_PTCLASS_E
{
    RTP_PTCLASS_VIDEO,
    RTP_PTCLASS_AUDIO,
    RTP_PTCLASS_CTRL,
    RTP_PTCLASS_FEC,
    RTP_PTCLASS_DATA,
    RTP_PTCLASS_UNKNOW,
};

enum RTP_NET_TYPE {
    NET_TYPE_UDP = 1,
    NET_TYPE_TCP = 2,
};

//keyframe mean private data length (even if 0), so don't bind control flag with keyframe
enum RTP_FLAG_E
{
    RTP_FLAG_KEYFRAME   = 0x1,
    RTP_FLAG_VIDEO      = 0x2,
    RTP_FLAG_AUDIO      = 0x4,
    RTP_FLAG_CONTROL    = 0x8,
    RTP_FLAG_RAW        = 0x10,
    RTP_FLAG_RESEND     = 0x20,
};

static inline bool rtp_is_audio(unsigned char pt)
{
    return (pt == ENCODE_TYPE_OPUS) || (pt == ENCODE_TYPE_WMA1) || (pt == ENCODE_TYPE_AAC) || (pt == ENCODE_TYPE_G711A);
}

static inline bool rtp_is_video(unsigned char pt)
{
    return (pt == ENCODE_TYPE_N264) || (pt == ENCODE_TYPE_H264) || (pt == ENCODE_TYPE_H265);
}

static inline bool rtp_is_ctrl(unsigned char pt)
{
    return (pt == ENCODE_TYPE_CTRL);
}

static inline bool rtp_is_fec(unsigned char pt)
{
    return pt == ENCODE_TYPE_FEC;
}

static inline RTP_PTCLASS_E rtp_ptclass_from_pt(unsigned char pt)
{
    if (rtp_is_video(pt))
        return RTP_PTCLASS_VIDEO;
    if (rtp_is_audio(pt))
        return RTP_PTCLASS_AUDIO;
    if (pt == ENCODE_TYPE_CTRL)
        return RTP_PTCLASS_CTRL;
    if (pt == ENCODE_TYPE_FEC)
        return RTP_PTCLASS_FEC;
    return RTP_PTCLASS_UNKNOW;
}

typedef struct
{
    unsigned char cc  : 4;    //表示紧跟在头部后面的CSRC标识的数目
    unsigned char x   : 1;    //如果置"1"，则在RTP头部后面，紧跟一个扩展的头部
    unsigned char p   : 1;    //如果置"1"，则分组结尾填充有一个或多个8位字串
    unsigned char ver : 2;    //=2    
    unsigned char pt  : 7;    //它定义了RTP载荷的类型
    unsigned char m   : 1;
    unsigned short seq;       //序列号,每发送一个RTP分组它就加1,但是它的起始值是随机的
    unsigned timestamp;       //表示RTP包中第一个8位串的采样时刻, OR CHANNEL ID
    unsigned ssrc;            //同步源标识, OR STREAM ID
}rtphdr_t;

typedef struct
{
    unsigned short  cmd;
    unsigned short  cmdlen;
}rtphdr_ctrl_t;

typedef struct
{
    unsigned short    exdef;
    unsigned short    exlen;
    union _data {
        struct _vt {
            unsigned short  checksum;
            unsigned char   framerate : 5;  //frame rate
            unsigned char   formatn : 2;    //format coeff, wont use if (formatn = 11 and framerate == 0, which is old factory code)
            unsigned char   nouse : 1;
            unsigned char   exfmt : 5;
            unsigned char   iframe : 2;
            unsigned char   ishead : 1;
        } vt;
        unsigned vu;
    } data;
}rtphdr_exv1_t;

typedef struct _rtphdr_fullctrl_t
{
    rtphdr_t        _rtphdr;
    rtphdr_exv1_t   _rtpext;
    rtphdr_ctrl_t   _rtpctrl;
}rtphdr_fullctrl_t;

struct rtp_fec_t
{
    unsigned short end_seq;
    unsigned char  seq_num;
    unsigned char  payload;
    rtp_fec_t() :end_seq(0), seq_num(0), payload(0) {}
};


#endif // __rtp_h__
