#ifndef SHELL_H
#define SHELL_H

/* find_command:
 *  Searches for a command in PATH or returns full path if it contains '/'
 *  Returns a malloc’ed string with the executable path if found,
 *  or NULL if the command cannot be found or is not executable.
 */
char *find_command(char *cmd, char **envp);

#endif

