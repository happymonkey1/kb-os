#pragma once

#include "../filesystem/filesystem.h"

#include <stddef.h>

#define MKDIR_CMD_STR "mkdir"

// mkdir command
// posix compliant
// https://pubs.opengroup.org/onlinepubs/009695299/functions/mkdir.html
int kb_shell_mkdir(const char* path, mode_t mode);