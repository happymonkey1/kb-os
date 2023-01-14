#include "../include/shell/shell.h"
#include "../include/libc/string.h"
#include "../include/driver/vga_driver.h"
#include "../include/memory/memory.h"
#include "../include/filesystem/filesystem.h"
#include "../include/driver/console.h"

#include "../include/shell/cd.h"
#include "../include/shell/clear.h"
#include "../include/shell/echo.h"
#include "../include/shell/mkdir.h"
#include "../include/shell/ls.h"

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

    // #TODO doesn't this go out of scope?
    char* argv[MAX_ARGV_COUNT];

    size_t cmd_len = strlen(raw_cmd);
    size_t p = 0;
    size_t argc = 0;
    for (int i = 0; i < cmd_len + 1; ++i)
    {
        // #TODO when quotes are encountered, parse as one substr
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
        {
            int ret = kb_cd_exec(cmd.argv[0]);
            if (ret != 0)
                print_string("cd failed to execute.\n");
        }
    }
    else if (strcmp(cmd.cmd, CLEAR_CMD_STR) == 0)
    {
        if (cmd.argc == 0)
            kb_shell_clear();
        else
            print_string("clear command takes no arguments");
    }
    else if (strcmp(cmd.cmd, ECHO_CMD_STR) == 0)
    {
        if (cmd.argc != 1)
            print_string("echo takes 1 argument.\n");
        else
            kb_shell_echo(cmd.argv[0]);
    }
    else if (strcmp(cmd.cmd, MKDIR_CMD_STR) == 0)
    {
        if (cmd.argc != 1)
            print_string("mkdir takes 1 argument.\n");
        else
        {
            mode_t mode;
            kb_shell_mkdir(cmd.argv[0], mode);
        }
    }
    else if (strcmp(cmd.cmd, LS_CMD_STR) == 0)
    {
        if (cmd.argc != 0)
            print_string("ls command does not support arguments right now.");
        else
            kb_shell_ls(get_root_f());
    }
    else
    {
        print_string("unknown command: ");
        print_string(cmd.cmd);
        print_new_line();
    }
    
    kb_shell_print_prompt();

    free_cmd_struct(&cmd);
}

// prints the current path and $ symbol to screen
void kb_shell_print_prompt()
{
    const path_t current_dir = get_current_dir_path();
    print_string(current_dir.raw_path);
    print_string("$ ");
}