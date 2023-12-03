/* ***************************************************************
 *  Copyright (C) 2006  Yongming Wang(wangym@gmail.com)
 *  All Rights Reserved
 *
 *  This file is part of Ubeda project (http://www.ubeda.cn).
 *
 *  This copy of file is licensed to you under Ubeda License.
 *  You should have received a copy of the Ubeda License
 *  along with this program, if not, get it from  
 *      http://www.ubeda.cn
 * ***************************************************************/
/*****************************************************************
 * @file zdl_ofstream.h
 * @brief
 *****************************************************************/

#ifndef __zdl_ofstream_h__
#define __zdl_ofstream_h__
#pragma once

#include <string>
#include <boost/lexical_cast.hpp>

template<typename T, typename autoident_checker>
class zdl_ofstream
{
public:
    zdl_ofstream(T& ostm, autoident_checker& checker);
    zdl_ofstream& operator= (const zdl_ofstream& rhs);
    zdl_ofstream& operator<< (const char* str);
    zdl_ofstream& operator<< (const std::string& str);
    zdl_ofstream& operator<< (unsigned int val);
    zdl_ofstream& operator<< (std::ostream& (* func)(std::ostream &));
    const T& ofstream_impl();

private:
    T*          ofstream_;
    autoident_checker* ident_checker_;
};

template<typename T, typename autoident_checker>
zdl_ofstream<T, autoident_checker>::zdl_ofstream(T& ostm, autoident_checker& checker)
:ofstream_(&ostm), ident_checker_(&checker)
{
}

template<typename T, typename autoident_checker>
zdl_ofstream<T, autoident_checker>& zdl_ofstream<T, autoident_checker>::operator= (const zdl_ofstream& rhs)
{
    this->ofstream_ = rhs.ofstream_;
    this->ident_checker_ = rhs.ident_checker_;
    return *this;
}

template<typename T, typename autoident_checker>
zdl_ofstream<T, autoident_checker>& zdl_ofstream<T, autoident_checker>::operator<< (const char* str)
{
    if(str == 0)
        return *this;
    int cur_ident = 0;
    while(str[0]){
        (*ofstream_) << (*ident_checker_)(str[0]);
        ++str;
    };
    return *this;   
};

template<typename T, typename autoident_checker>
zdl_ofstream<T, autoident_checker>& zdl_ofstream<T, autoident_checker>::operator<< (const std::string& str)
{
    return operator<<(str.c_str());
};

template<typename T, typename autoident_checker>
zdl_ofstream<T, autoident_checker>& zdl_ofstream<T, autoident_checker>::operator<< (unsigned int val)
{
    return operator<<(boost::lexical_cast<std::string>(val));
};

template<typename T, typename autoident_checker>
zdl_ofstream<T, autoident_checker>& zdl_ofstream<T, autoident_checker>::operator<< (std::ostream& (* func)(std::ostream &))
{
    return operator<<("\n");
}


#endif /*__zdl_ofstream_h__*/
