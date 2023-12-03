#ifndef __zdl_parser_h
#define __zdl_parser_h

#include <string>
#include <zce/zce_object.h>

class zdl_parser_context;

class zdl_parser : public zce_object
{
    typedef zce_smartptr<zdl_parser_context> zdl_parser_context_ptr;
public:
    zdl_parser();
    ~zdl_parser();

    void error(const char* s);
    void warning(const char* s);
    void read_cr();
    void parse(const char* filename, const zdl_parser_context_ptr& parser_context);
    void info(const char* s);
    zdl_parser_context_ptr context_ptr() const { return parser_context_; };

private:
    zdl_parser_context_ptr parser_context_;
    int line_;
    char* filename_;
};

typedef zce_smartptr<zdl_parser> zdl_parser_ptr;

#endif //__zdl_parser_h
