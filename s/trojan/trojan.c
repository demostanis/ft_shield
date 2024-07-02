#include "../ft_shield.h"

int	main(void)
{
	if (is_locked())
		return (EXIT_FAILURE);
	create_lock();
	delete_lock_at_exit();
	pause();
	return (EXIT_SUCCESS);
}
