#include "../ft_shield.h"

int	is_locked(void)
{
	int	fd;

	fd = open(LOCKLOC, O_RDONLY);
	return (close(fd), fd >= 0);
}

void	create_lock(void)
{
	int	fd;

	fd = creat(LOCKLOC, 0644);
	flock(fd, LOCK_EX);
	close(fd);
}

void	delete_lock(void)
{
	unlink(LOCKLOC);
}

void	delete_lock_at_exit(void)
{
	atexit(&delete_lock);
	signal(SIGINT, (void (*)(int))&delete_lock);
}
