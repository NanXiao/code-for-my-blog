#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ev.h"

static int init_server(uint16_t port);
static void server_callback(EV_P_ ev_io *watcher, int revents);

int init_server(uint16_t port)
{
	int server_fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (server_fd < 0) {
		perror("socket");
		exit(1);
	}

	int on = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	struct sockaddr_in6 addr = {0};
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(port);
	addr.sin6_addr = in6addr_any;
	if (bind(server_fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(server_fd, 10) == -1) {
		perror("listen");
		exit(1);
	}

	return server_fd;
}

static void server_callback(EV_P_ ev_io *watcher, int revents)
{
	struct sockaddr_in6 client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd;
	while (client_fd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_addr_len)) {
		if (client_fd == -1) {
			ev_io_stop (EV_A_ watcher);
			break;
		}
		char client_addr_buf[INET6_ADDRSTRLEN];
		printf("Connection from %s:%d\n", 
			inet_ntop(AF_INET6, &client_addr.sin6_addr, client_addr_buf, sizeof(client_addr_buf)),
			ntohs(client_addr.sin6_port));
		close(client_fd);
	}
}

int main(void)
{
	int server_fd = init_server(60000);

	ev_io server_watcher;
	ev_io_init(&server_watcher, server_callback, server_fd, EV_READ);
	ev_io_start(EV_DEFAULT, &server_watcher);
	ev_run(EV_DEFAULT, 0);

	return 0;
}
