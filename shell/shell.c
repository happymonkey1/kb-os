#include "../include/shell/shell.h"
#include "../include/libc/string.h"
#include "../include/driver/vga_driver.h"
#include "../include/shell/cd.h"
#include "../include/memory/memory.h"
#include "../include/filesystem/filesystem.h"
#include "../include/driver/console.h"

// temporary max argv count
// #TODO should be dynamic
#define MAX_ARGV_COUNT 256

// struct for command and arguments
typedef struct
{
    char* cmd;
    size_t argc;
    char** argv;
} cmd_t;

cmd_t initial_cmd_struct()
{
    cmd_t cmd;
    cmd.cmd = NULL;
    cmd.argc = 0;
    cmd.argv = NULL;

    return cmd;
}

void free_cmd_struct(cmd_t* cmd)
{
    free(cmd->cmd);
    for (size_t i = 0; i < cmd->argc; ++i)
        free(cmd->argv[i]);
    
    free(cmd->argv);

    cmd->cmd = NULL;
    cmd->argc = 0;
    cmd->argv = NULL;
}

cmd_t parse_str_to_cmd(const char* raw_cmd)
{
    serial_print_string("parsing raw string to cmd struct.\n");
    cmd_t cmd = initial_cmd_struct();

    char* argv[MAX_ARGV_COUNT];

    size_t cmd_len = strlen(raw_cmd);
    size_t p = 0;
    size_t argc = 0;
    for (int i = 0; i < cmd_len + 1; ++i)
    {
        if (raw_cmd[i] == ' ' || raw_cmd[i] == 0)
        {
            // #TODO argv buffer is 1 too big...

            // get substr copy
            size_t size = i - p;
            char* substr = (char*)malloc(size + 1);
            memcpy(substr, ((const uint8_t*)raw_cmd) + p + (p == 0 ? 0 : 1), size);
            substr[size] = 0;
            p = i;

            if (cmd.cmd == NULL)
                cmd.cmd = substr;
            else
                argv[argc++] = substr;
        }
    }

    cmd.argc = argc;
    cmd.argv = argv;

    return cmd;
}

void kb_exec(const char* raw_cmd)
{
    cmd_t cmd = parse_str_to_cmd(raw_cmd);

    if (strcmp(cmd.cmd, "HALT") == 0)
    {
        free_cmd_struct(&cmd);
        print_string("stopping kernel.");
        asm volatile("hlt");
    }
    else if (strcmp(cmd.cmd, CD_CMD_STR) == 0)
    {
        if (cmd.argc != 1)
            print_string("cd takes 1 argument.\n");
        else
            kb_cd_exec(cmd.argv[0]);
    }
    else
    {
        print_string("unknown command: ");
        print_string(cmd.cmd);
    }
    
    print_new_line();
    path_t current_dir = get_current_dir();
    print_string(current_dir.raw_path);
    print_string("$ ");

    free_cmd_struct(&cmd);
}