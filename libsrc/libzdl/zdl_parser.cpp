#include <zce/zce_config.h>
#include "zdl/zdl_parser.h"
#include "zdl/zdl_utils.h"
#include "zdl/zdl_parser_context.h"
#include "zdl_lex.h"
#include <zce/zce_dblock.h>

using namespace std;

zdl_parser_ptr g_parser_ptr;

zdl_parser::zdl_parser()
:line_(1), filename_(0)
{
}

zdl_parser::~zdl_parser()
{
}

void zdl_parser::error(const char* s)
{
    fprintf(stderr, "error: [%s][%d] %s\n", filename_, line_, s);
}

void zdl_parser::warning(const char* s)
{
    fprintf(stderr, "warn: [%s][%d] %s\n", filename_, line_, s);
}

void zdl_parser::read_cr()
{
    line_++;
}

typedef size_t yy_size_t;
typedef void* yyscan_t;
typedef struct yy_buffer_state* YY_BUFFER_STATE; 
extern int yylex_init(yyscan_t* ptr_yy_globals);
extern YY_BUFFER_STATE yy_scan_buffer(char* base, yy_size_t size, yyscan_t yyscanner);
extern YY_BUFFER_STATE yy_scan_bytes(const char* yybytes, int  _yybytes_len, yyscan_t yyscanner);
extern int yylex_destroy(yyscan_t yyscanner);
extern int yyparse(yyscan_t scanner, zdl_parser* parser);

void zdl_parser::parse(const char* filename, const zdl_parser_context_ptr& parser_context)
{
    parser_context_ = parser_context;
    filename_ = (char*)filename;
    line_ = 1;

    zce_dblock mb;
    int ret = zce_dblock_from_file(mb, filename);

    mb.wr_ptr()[0] = 0;
    mb.wr_ptr()[1] = 0;
    mb.wr_ptr(2);
    yyscan_t scanner;
    yylex_init(&scanner);
    yy_scan_buffer((char*)mb.rd_ptr(), mb.length(), scanner);
    yyparse(scanner, this);
    yylex_destroy(scanner);
}

