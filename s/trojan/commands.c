#include "../ft_shield.h"

extern int	g_shelling;
extern int	g_shells[BACKLOG];

void	prompt(int fd)
{
	if (!g_shells[fd])
		write(fd, PROMPT, sizeof PROMPT);
}

void	handle_command(int epfd, int cfd)
{
	char	buf[4096];
	ssize_t	n;

	n = read(cfd, buf, sizeof buf);
	if (n <= 0)
		return ;
	buf[n] = 0;
	if (strcmp(buf, "?\n") == 0)
		write(cfd, HELP, sizeof HELP);
	else if (strcmp(buf, "shell\n") == 0)
	{
		write(cfd, SHELL_MSG, sizeof SHELL_MSG);
		g_shelling = 1;
		exit_client(epfd, cfd);
	}
	else if (strcmp(buf, "\n"))
	{
		write(cfd, UNKNOWN_CMD_MSG, sizeof UNKNOWN_CMD_MSG);
	}
	prompt(cfd);
}
