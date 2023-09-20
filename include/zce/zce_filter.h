#pragma once

#include <zce/zce_config.h>
#include <zce/zce_smartptr.h>
#include <zce/zce_conf.h>
#include <map>
#include <set>

class ZCE_API zce_filter : public zce_filemon
{
public:
    enum ERV_FILTER
    {
        ERV_FILTER_MASK,
        ERV_FILTER_BLOCK,
    };

private:

    struct filter_string
    {
        unsigned pos;       //��str_��λ��
        unsigned len;       //����
        ERV_FILTER level;   //����

        filter_string(unsigned p, unsigned l, ERV_FILTER lev);
    };

    struct match_context
    {
        struct filter_string* filter;   //ƥ���е��ַ���

        unsigned start_pos;             //��ƥ���ַ�����ʼ����λ��
        unsigned last_pos;              //���һ��ƥ���λ��
        unsigned input_mask;            //����ʼλ�ÿ�ʼƥ�䣬ÿ��BIT��ʾ1�������Ҫ��32����
        unsigned match_cnt;             //ƥ��ĸ���

        match_context(struct filter_string* f, unsigned pos);

        int feed(const wchar_t* const_str, const wchar_t* str, unsigned pos);
    };

    typedef std::multimap<wchar_t, filter_string> key_string_map;
    typedef std::multimap <wchar_t, match_context>::const_iterator key_pos_iter;
    typedef std::pair<key_pos_iter, key_pos_iter> key_pos_range;

    wchar_t*    str_ptr_;
    unsigned    str_len_;
    unsigned    str_size_;
    key_string_map key_pos_map_;

    std::set<std::wstring> str_set_;

    int add_tofilter(const char* content, int size);

public:
    zce_filter();

    ~zce_filter();

    void clear();

    int add(const wchar_t* str, unsigned size, ERV_FILTER level);

    //if have block, return 1; normal ,return 0; err < 0;
    int replace(wchar_t* str, unsigned size, wchar_t c);

    const std::set<std::wstring>& str_set() {
        return str_set_;
    }

    virtual int process_content(unsigned char* data, unsigned len);

    static std::string simple_filter(const std::string& source, int* filtercnt = 0);
};
