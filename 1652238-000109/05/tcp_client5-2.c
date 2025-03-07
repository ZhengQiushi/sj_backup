#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>

int write_flag = 1;

static void sig_alarm(int signal)
{
	write_flag = 1;
}

int main(int argc, char const *argv[])
{
	if (argc < 4)
	{
		printf("缺少参数\n");
		return -1;
	}
	else
	{
		printf("连接ip %s\n", argv[1]);
		printf("连接端口 %d\n", atoi(argv[2]));
	}
	struct sockaddr_in address;
	int sock, sock2, valread;
	struct sockaddr_in serv_addr;
	struct sockaddr_in serv_addr2;

	char *hello = "Hello from client";
	char buffer[100] = {0};
	char buffer_send[15] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}
	if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	int flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);

	flags = fcntl(sock2, F_GETFL, 0);
	fcntl(sock2, F_SETFL, flags | O_NONBLOCK);

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

	serv_addr2.sin_family = AF_INET;
	serv_addr2.sin_port = htons(atoi(argv[3]));
	serv_addr2.sin_addr.s_addr = inet_addr(argv[1]);

	fd_set rfds, wfds;
	struct timeval tv;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_SET(sock, &rfds);
	FD_SET(sock, &wfds);

	/* set select() time out */
	tv.tv_sec = 10;
	tv.tv_usec = 0;

	printf("connecting to %s\n", argv[2]);
	int res = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (0 == res)
	{
		printf("connect success immediately\n");
	}
	else
	{
		if (errno == EINPROGRESS)
		{
			int selres = select(sock + 1, &rfds, &wfds, NULL, &tv);

			switch (selres)
			{
			case -1:
				printf("select error\n");
				break;
			case 0:
				printf("select time out\n");
				break;
			default:
				printf("select return %d\n", selres);
				if (FD_ISSET(sock, &rfds) && FD_ISSET(sock, &wfds))
				{
					perror("connect error\n");
				}
				if (FD_ISSET(sock, &rfds) || FD_ISSET(sock, &wfds))
				{

					int errinfo;
					socklen_t errlen = sizeof(int);
					if (-1 == getsockopt(sock, SOL_SOCKET, SO_ERROR, &errinfo, &errlen))
					{
						printf("getsockopt return -1.\n");
						perror("getsocketopt error:");
						break;
					}
					else if (0 != errinfo)
					{
						printf("getsockopt return errinfo = %d.\n", errinfo);
						break;
					}

					printf("1 connect ok?\n");

					FD_SET(sock2, &wfds);
					FD_SET(sock2, &wfds);
					printf("2 connecting to %s\n", argv[3]);
					res = connect(sock2, (struct sockaddr *)&serv_addr2, sizeof(serv_addr2));

					alarm(3);
					signal(SIGALRM, sig_alarm);
					if (0 == res)
					{
						printf("2 connect success immediately\n");
					}
					else
					{
						if (errno == EINPROGRESS)
						{
							int selres = select(sock2 + 1, &rfds, &wfds, NULL, &tv);

							switch (selres)
							{
							case -1:
								printf("select error\n");
								break;
							case 0:
								printf("select time out\n");
								break;
							default:
								printf("select return %d\n", selres);
								if (FD_ISSET(sock2, &rfds) && FD_ISSET(sock2, &wfds))
								{
									perror("connect error\n");
								}
								if (FD_ISSET(sock2, &rfds) || FD_ISSET(sock2, &wfds))
								{

									int errinfo;
									socklen_t errlen = sizeof(int);
									if (-1 == getsockopt(sock2, SOL_SOCKET, SO_ERROR, &errinfo, &errlen))
									{
										printf("getsockopt return -1.\n");
										perror("getsocketopt error:");
										break;
									}
									else if (0 != errinfo)
									{
										printf("getsockopt return errinfo = %d.\n", errinfo);
										break;
									}

									printf("2 connect ok?\n");

									while (1)
									{
										if (write_flag)
										{
											int send_bytes = send(sock, buffer_send, 15, 0);
											printf("1 %d bytes send\n", send_bytes);
											send_bytes = send(sock2, buffer_send, 15, 0);
											printf("2 %d bytes send\n", send_bytes);
											write_flag = 0;
											alarm(3);
										}

										int read = recv(sock, buffer, 100, 0);
										if (read == 0)
										{
											perror("Connect closed");
											break;
										}
										else if (read < 0)
										{
											continue;
											printf("recv return : %d\n", read);
											if (errno == EPIPE || errno == EWOULDBLOCK || errno == EAGAIN)
											{
												perror("");
												continue;
											}
											else
											{
												perror("");
												break;
											}
										}
										else
										{
											printf("2 %d bytes read\n", read);
										}

										read = recv(sock2, buffer, 100, 0);
										if (read == 0)
										{
											perror("Connect closed");
											break;
										}
										else if (read < 0)
										{
											continue;
											printf("recv return : %d\n", read);
											if (errno == EPIPE || errno == EWOULDBLOCK || errno == EAGAIN)
											{
												perror("");
												continue;
											}
											else
											{
												perror("");
												break;
											}
										}
										else
										{
											printf("2 %d bytes read\n", read);
										}
									}
									printf("2 socket over\n");
								}
							}
						}

						else
						{
							perror("2 connect error");
						}
					}
					printf("1 socket over\n");
				}
			}
		}
		else
		{
			perror("1 connect error");
		}
	}
	printf("return\n");
	return 0;
}