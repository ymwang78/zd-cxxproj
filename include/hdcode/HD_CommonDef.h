#ifndef __hd_commondef_h__
#define __hd_commondef_h__

#include <string>
#include "ustd.h"
#include "ustd_tlv_serialize.h"

//硬件类型
enum HD_TYPE
{
	HD_MAC = 10,   //MAC
	HD_DISKDRIVE , //硬盘
	HD_MAINBOARD,  //主板
	HD_MEMORY,     //内存        
	HD_SID,        //Administrator账户的SID
	HD_PCNAME,     //计算机名         
	HD_PROMOTER,   //联盟ID
	HD_SITEID ,	   //站点的ID
	HD_PRODUCT_ID, //产品ID
};

//一条硬件信息记录
typedef  struct HD_SLOT
{
	unsigned short	slot_type;//硬件类型
	std::string     slot_info;//数据
}HD_SLOT;

//HD_SLOT 中各字段的tlv 标识
enum  TLVtag_HD_SLOT
{
	TLV_HD_SLOT_TYPE =20,
	TLV_HD_SLOT_INFO,
};

//硬件数据的集合
typedef struct HD_INFORMATION
{
	std::vector<HD_SLOT> slots;
}HD_INFORMATION;


//HD_INFORMATION 中各字段的tlv 标识
enum TLVtag_HD_INFORMATION
{
	TLV_HD_INFORMATION_SLOTS = 30 ,
};

//最后发给服务器的数据
typedef struct HD_RESULT
{
	unsigned long  AlgorithmVersion;	//加密算法的版本号标识（算法每次改动一次，这个值必须变更一次）,DWORD类型
	std::string strEncryptBody;			//被序列化并加密过的字节流
	std::string strMd5;					//硬件信息的md5
}HD_RESULT;

//HD_RESULT 中各字段的tlv 标识
enum TLVtag_HD_RESULT
{
	TLV_RESULT_Algorithm_Version= 40,
	TLV_RESULT_BODY,
	TLV_RESULT_MD5,
};

//算法标识
enum  Algorithm_Version
{
	AES_CBC_V1 = 0x20120829,	//2012/8/29,fangqing，采用AES算法，CBC加密模式，第一个版本
};

namespace ustd
{
    template<>
    inline std::string ToString(const HD_SLOT& value)
    {
	    ustd::TLVWriter writer;
	    writer.AppendAttribute(TLV_HD_SLOT_TYPE, value.slot_type);
	    writer.AppendAttribute(TLV_HD_SLOT_INFO, value.slot_info);
	    return writer.GetStream();
    }

    template<>
    inline bool FromString(const std::string& source, HD_SLOT& value)
    {
	    ustd::TLVReader reader;
	    reader.Parse(source);
	    return (reader.GetAttribute(TLV_HD_SLOT_TYPE, value.slot_type) &&
			    reader.GetAttribute(TLV_HD_SLOT_INFO, value.slot_info));
    }
}

/********     HD_INFORMATION  结构的序列化和反序列化 ********/
std::string HDInformationToString(const HD_INFORMATION & value)
{
	ustd::TLVWriter tlvWriter;
	tlvWriter.AppendAttributes(TLV_HD_INFORMATION_SLOTS,value.slots);
	return tlvWriter.GetStream();
}

bool HDInformationFromString(const std::string & source, HD_INFORMATION & value)
{
	ustd::TLVReader tlvReader;
	tlvReader.Parse(source);
    return tlvReader.GetAttributes<std::vector<HD_SLOT>, HD_SLOT>(TLV_HD_INFORMATION_SLOTS,value.slots);
}


/********	HD_RESULT 结构的序列化和反序列化    ********/
std::string  HDResultToString(const HD_RESULT &slot_result )
{
	ustd::TLVWriter tlvWriter;
	tlvWriter.AppendAttribute(TLV_RESULT_Algorithm_Version,slot_result.AlgorithmVersion);
	tlvWriter.AppendAttribute(TLV_RESULT_BODY,slot_result.strEncryptBody);
	tlvWriter.AppendAttribute(TLV_RESULT_MD5,slot_result.strMd5);
	return tlvWriter.GetStream();
}


inline bool HDResultFromString(const std::string &source,HD_RESULT &result)
{
	ustd::TLVReader tlvReader;
	tlvReader.Parse(source);
	return (tlvReader.GetAttribute(TLV_RESULT_Algorithm_Version,result.AlgorithmVersion)&&
		   tlvReader.GetAttribute(TLV_RESULT_BODY,result.strEncryptBody) && 
		   tlvReader.GetAttribute(TLV_RESULT_MD5,result.strMd5));
}
#endif //_hd_commondef_h_
