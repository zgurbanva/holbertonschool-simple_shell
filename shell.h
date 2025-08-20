#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT_STR "#cisfun$ "
#define DELIMS " \t\r\n"

extern char **environ;

/* Prototypes */
void print_prompt(void);
char *first_token(char *line);
int run_command(const char *cmd, const char *pname);
int shell_loop(const char *pname);

#endif
