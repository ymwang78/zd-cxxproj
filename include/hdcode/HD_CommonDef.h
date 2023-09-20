#ifndef __hd_commondef_h__
#define __hd_commondef_h__

#include <string>
#include "ustd.h"
#include "ustd_tlv_serialize.h"

//Ӳ������
enum HD_TYPE
{
	HD_MAC = 10,   //MAC
	HD_DISKDRIVE , //Ӳ��
	HD_MAINBOARD,  //����
	HD_MEMORY,     //�ڴ�        
	HD_SID,        //Administrator�˻���SID
	HD_PCNAME,     //�������         
	HD_PROMOTER,   //����ID
	HD_SITEID ,	   //վ���ID
	HD_PRODUCT_ID, //��ƷID
};

//һ��Ӳ����Ϣ��¼
typedef  struct HD_SLOT
{
	unsigned short	slot_type;//Ӳ������
	std::string     slot_info;//����
}HD_SLOT;

//HD_SLOT �и��ֶε�tlv ��ʶ
enum  TLVtag_HD_SLOT
{
	TLV_HD_SLOT_TYPE =20,
	TLV_HD_SLOT_INFO,
};

//Ӳ�����ݵļ���
typedef struct HD_INFORMATION
{
	std::vector<HD_SLOT> slots;
}HD_INFORMATION;


//HD_INFORMATION �и��ֶε�tlv ��ʶ
enum TLVtag_HD_INFORMATION
{
	TLV_HD_INFORMATION_SLOTS = 30 ,
};

//��󷢸�������������
typedef struct HD_RESULT
{
	unsigned long  AlgorithmVersion;	//�����㷨�İ汾�ű�ʶ���㷨ÿ�θĶ�һ�Σ����ֵ������һ�Σ�,DWORD����
	std::string strEncryptBody;			//�����л������ܹ����ֽ���
	std::string strMd5;					//Ӳ����Ϣ��md5
}HD_RESULT;

//HD_RESULT �и��ֶε�tlv ��ʶ
enum TLVtag_HD_RESULT
{
	TLV_RESULT_Algorithm_Version= 40,
	TLV_RESULT_BODY,
	TLV_RESULT_MD5,
};

//�㷨��ʶ
enum  Algorithm_Version
{
	AES_CBC_V1 = 0x20120829,	//2012/8/29,fangqing������AES�㷨��CBC����ģʽ����һ���汾
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

/********     HD_INFORMATION  �ṹ�����л��ͷ����л� ********/
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


/********	HD_RESULT �ṹ�����л��ͷ����л�    ********/
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
