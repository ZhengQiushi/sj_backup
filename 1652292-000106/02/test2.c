//��׿�� 1652292 �ƿ�2��
#include <stdio.h>
#include <unistd.h>

int main()
{
	daemon(1, 1);
	while (1)
	{
		printf("1652292\n");
		fflush(stdout);
		sleep(5);
	}
	return 0;
}
