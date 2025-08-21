#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* PATH utilities */
char *find_command(char *cmd, char **envp);
char *join_path(const char *dir, const char *file);
char *_getenv_from_envp(char **envp, const char *name);
char *strdup_custom(const char *s);

#endif 
