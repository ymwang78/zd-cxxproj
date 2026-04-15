#pragma once
// ***************************************************************
//  zmpc_storm   version:  1.0   -  date:  2025/08/20
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project libmpc.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************

#include <zce/zdp_storm.h>
#include <zce/zds_schema.h>
#include <zce/zce_singleton.h>
#include "zmpc_pack.h"

namespace zmpc {

class ZmpcStormClient : public ::zce::zdp::StormClient {
    void setLogCallback();
  public:
    ZmpcStormClient(const ::zce::SmartPtr<::zce::Reactor>& reactor, const std::string& project_guid,
                    const std::string& storm_topic, const ::zce::Any& ctx,
                    ::zce::zdp::publish_callback child_cb, ::zce::zdp::set_callback set_cb,
                    std::function<void()> connected_cb,
                    std::function<void()> disconnect_cb);

    ~ZmpcStormClient();
};

}  // namespace zmpc