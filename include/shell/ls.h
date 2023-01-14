#pragma once

#include "../filesystem/file.h"

#define LS_CMD_STR "ls"

// ls command
// posix compliant
// https://pubs.opengroup.org/onlinepubs/009696899/utilities/ls.html
int kb_shell_ls(file_t* file_f);