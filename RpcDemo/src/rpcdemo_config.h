#pragma once

#include <string>
#include <zce/zce_object.h>
#include <zce/zce_conf.h>
#include <zce/zce_singleton.h>
#include "rpcdemo_config_proto.h"

class rpcdemo_config : public zce_conf<rpcdemo::rpcdemo_server>
{
    virtual int process_content(unsigned char* xml, unsigned length);

public:

};

typedef zce_singleton<rpcdemo_config> rpcdemo_config_sigt;
