#pragma once

#include <zce/zce_config.h>
#include <zce/zce_types.h>
#include <vector>
#include <stdarg.h>
#include <sstream>

std::string ZCE_API zce_get_path(const char* filename);

void ZCE_API zce_chdir();

bool ZCE_API zce_makedir(const char* dir);

const char* ZCE_API zce_filename(const char* str);

bool ZCE_API zce_filepath_exists(const char* path);