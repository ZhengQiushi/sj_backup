#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	if (argc < 3)
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
	int sock = 0, valread;
	struct sockaddr_in serv_addr;

	char *hello = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	else
	{
		printf("连接成功\n");
	}
	int flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags|O_NONBLOCK);


	fd_set rfds, wfds;
	struct timeval tv;

	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);
	/* set select() time out */
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	int selres = select(sock + 1, &rfds, NULL, NULL, &tv);

	printf("select returns %d\n", selres);
	printf("start reading...\n");
	int read = recv(sock, buffer, sizeof(buffer), 0);
	printf("recv returned: %d\n", read);
	selres = select(sock + 1, NULL, NULL, NULL, &tv);

	return 0;
}