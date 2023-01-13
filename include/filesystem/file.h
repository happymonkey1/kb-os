#pragma once

#include <stdint.h>

#include "path.h"

typedef struct file
{
    // name of file
    const char* name;
    // flags
    uint32_t flags;
    // parent file
    path_t path;

} file_t;