#include		<signal.h>
#include		<poll.h>
#include		<stdio.h>
#include		<sys/types.h>
#include		<sys/socket.h>
#include		<sys/ioctl.h>
#include		<stdlib.h>
#include		<arpa/inet.h>
#include		<unistd.h>
#include		<string.h>

#define			BUFFER_SIZE 4096

int			init_server(struct sockaddr_in *addr, int port)
{
  int			s, on, rc, len;

  on = 1;
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0)
    perror("socket");
  rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
  rc = ioctl(s, FIONBIO, (char *)&on);
  memset(addr, 0, sizeof(addr));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(port);
  addr->sin_addr.s_addr = INADDR_ANY;
  rc = bind(s, (struct sockaddr *)addr, (socklen_t)sizeof(struct sockaddr));
  if (rc < 0)
    perror("bind");
  rc = listen(s, 32);
  if (rc < 0)
    perror("listen");
  return (s);
}

int			main(int argc, char *argv[])
{
  struct pollfd		fds[2];
  nfds_t		nfds;
  int			timeout;
  int			s;
  int			res;
  int			new_s;
  struct sockaddr	client;
  struct sockaddr_in	addr;
  char			*buffer[BUFFER_SIZE];

  nfds = 1;
  if (argc < 2)
    {
      printf("Usage: ./poll [port]\n");
      exit(EXIT_FAILURE);
    }
  s = init_server(&addr, atoi(argv[1]));
  printf("Server socket: %d\n", s);
  memset(fds, 0, sizeof(fds));
  fds[0].fd = s;
  fds[0].events = POLLIN;
  timeout = 3000;
  while (1)
    {
      res = poll(fds, nfds, timeout);
      for (int i = 0; i < 2; i++)
	{
	  if (fds[i].revents == POLLIN)
	    {
	      if (fds[i].fd == s)
		{
		  printf("%d\n", fds[i].revents);
		  new_s = accept(s, NULL, NULL);
		  printf("%d\n", new_s);
		  fds[i + 1].fd = new_s;
		  fds[i + 1].events = POLLOUT;
		}
	      else if (fds[i].fd > 0)
		{
		  printf("New client\n");
		}
	    }
	  else if (fds[i].revents == POLLOUT)
	    {
	      res = read(0, buffer, BUFFER_SIZE);
	      write(fds[i].fd, buffer, res);
	    }
	}
      usleep(1000);
      nfds++;
    }
   
  return (0);
}
