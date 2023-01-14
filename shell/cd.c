#include "../include/shell/cd.h"
#include "../include/filesystem/filesystem.h"

int kb_cd_exec(const char* args)
{
    path_t path = parse_str_to_path(args);

    if (is_valid_path(&path))
    {
        kb_set_current_dir(path);
        return 0;
    }

    return -1;
}