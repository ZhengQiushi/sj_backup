#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
volatile long int sendbytes = 0;

int main(int argc, char const *argv[])
{
	if (argc < 4)
	{
		printf("ȱ�ٲ���\n");
		return -1;
	}
	else
	{
		printf("ʹ�ö˿� %d\n", atoi(argv[1]));
		printf("����ip %s\n", argv[2]);
		printf("���Ӷ˿� %d\n", atoi(argv[3]));
	}
	struct sockaddr_in address, my_addr;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;

	char send_buffer[20];
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[2]);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, argv[2], &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	int port = atoi(argv[1]);
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = inet_addr("192.168.81.233");
	my_addr.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in)) == 0)
		printf("Binded Correctly\n");
	else
		printf("Unable to bind\n");

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	else
	{
		printf("���ӳɹ�\n");
	}
	//printf("Hello message sent\n");
	strcpy(send_buffer, "ABCDEFGHIJKLMNOPQRS");
	//long int sendbytes = 0;
	while(1)
	{
		write(sock , send_buffer , strlen(send_buffer));
		sendbytes += 20;
		printf("write 20 bytes, %ld total\n", sendbytes);
		//sleep(1);
	}
	printf("socket over\n");
	return 0;
}