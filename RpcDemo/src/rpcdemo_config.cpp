#include "rpcdemo_inc.h"
#include "rpcdemo_config.h"
#include "rpcdemo_config_pack.h"

int rpcdemo_config::process_content(unsigned char* xml, unsigned length)
{
    int ret = rpcdemo::zxml_unpack(conf_, xml, length, 0);
    return ret;
}
