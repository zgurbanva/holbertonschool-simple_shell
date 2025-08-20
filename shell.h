#ifndef SHELL_H
#define SHELL_H

/* Standard libs */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

/* Environment */
extern char **environ;

/* Constants */
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n"
#define PROMPT "($) "

/* Core */
int shell_loop(const char *pname);
char *read_line(void);
char **tokenize_line(char *line);
void free_tokens(char **tokens);

/* PATH / command resolve */
char *_getenv(const char *name);
char *find_command(const char *cmd);
int is_executable(const char *path);
char *build_path(const char *dir, const char *cmd);

/* Exec */
int execute_command(char **args, const char *pname, size_t line_no);
int launch_process(char **args);

/* Builtins */
int is_builtin(const char *cmd);
int run_builtin(char **args, const char *pname, size_t line_no);
int builtin_env(void);
int builtin_exit(char **args);

/* Utils */
void print_not_found(const char *pname, size_t line_no, const char *cmd);

#endif
