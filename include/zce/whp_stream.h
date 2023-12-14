#pragma once

#include <zce/zce_handler.h>
#include <zce/zdp_schema.h>
#include <zce/zce_timer.h>

class zce_reactor;

class ZCE_API whp_stream : public zce_istream
{
	zce_smartptr<zce_reactor> reactor_ptr_;

    zce_dblock dblock_;

	unsigned char m_cbSendRound;

	unsigned char m_cbRecvRound;

	unsigned m_dwSendXorKey;

	unsigned m_dwRecvXorKey;

	unsigned m_dwSendPacketCount;

	int encrypt(zce_dblock& dblock_ptr);

	int CrevasseBuffer(unsigned char checkcode, 
		zce_dblock& dblock_ptr);

	unsigned short SeedRandMap(unsigned short wSeed);

	unsigned char MapSendByte(unsigned char cbData);

	unsigned char MapRecvByte(unsigned char cbData);

	int proc_next(zce_dblock& dblock_ptr, const zce_any& ctx);

public:

	whp_stream(const zce_smartptr<zce_reactor>& reactor_ptr, unsigned preserv = 0);

	virtual int write(const zce_dblock& dblock_ptr, ERV_ISTREAM_WRITEOPT opt);

	virtual void on_read(const zce_dblock& dblock, const zce_any&);
};

//////////////////////////////////////////////////////////////////////////

class ZCE_API idip_stream : public zce_istream
{
    zce_smartptr<zce_reactor> reactor_ptr_;

    zce_dblock dblock_;

public:

    idip_stream(const zce_smartptr<zce_reactor>& reactor_ptr, unsigned preserv = 0);

    //virtual int write(const zce_dblock& dblock_ptr, ERV_ISTREAM_WRITEOPT opt);

    virtual void on_read(const zce_dblock& dblock, const zce_any&);
};