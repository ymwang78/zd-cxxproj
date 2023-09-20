// ***************************************************************
//  websocket   version:  1.0   -  date: 11/14/2015
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once
#ifndef __webs_h__
#define __webs_h__

#include <zce/zce_handler.h>
#include <zce/zdp_schema.h>

class zce_dblock;
class zce_reactor;

namespace zdp
{
	class websocket : public zce_istream
	{
		enum { READ_HEAD, READ_FRAME_HEAD, READ_FRAME_BODY } status_;

		zce_smartptr<zce_dblock> dblock_ptr_;			// websocket ��Ϣ֡��

		zce_smartptr<zce_dblock> dblock_head_ptr_;		// websocket ��Ϣ֡ͷ

		zce_atomic_long seq_;

		bool mpassive;
		char mpeerip[64];
		zce_uint16 mpeerport;

		std::string m_method;
		std::string m_uri;
		std::string m_version;
		std::map<std::string, std::string> fields;
		std::map<std::string, std::string> resfields;

		// ���websocket��Ϣͷ�е���Ϣ������ͷ��Ϣ�õ���Ϣ������ݶ�---RFC6455
		int websocket_split(zce_byte* buf, unsigned size, uint8_t& opcode);

	public:
		websocket();

		~websocket();

		zce_uint16 next_seq();

		//////////////////////////////////////////////////////////////////////////
		// on�ӿڣ����²����ϲ����
		virtual void on_open(bool passive, char* peerip, zce_uint16 peerport);

		virtual void on_read(zce_smartptr<zce_dblock>& dblock, zce_uint32 cur_read);

		virtual void on_close();
		// ��on�ӿڣ����ϲ����²����
		virtual int write(const zce_smartptr<zce_dblock>& dblock_ptr);

		virtual void close();

		//////////////////////////////////////////////////////////////////////////
	private:
		bool Parse(char* buf);
		void ProcessHandshake();
		std::string GetField(std::string key);
		void SetField(std::string key, std::string val);
		void SetResField(std::string key, std::string val);
		std::string Serialize(int state);

		int write_frame(const zce_smartptr<zce_dblock>& dblock_ptr, zce_smartptr<zce_dblock>& out_ptr, uint8_t opcode);
	};
};

#endif // __zdp_tcp_h__
