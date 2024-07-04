#include "../ft_shield.h"

void	ignore_sigpipe(void)
{
	signal(SIGPIPE, (void(*)(int))&ignore_sigpipe);
}

int	main(void)
{
	if (is_locked())
		return (EXIT_FAILURE);
	ignore_sigpipe();
	create_lock();
	delete_lock_at_exit();
	listen_server();
	return (EXIT_SUCCESS);
}
