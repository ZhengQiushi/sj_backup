#include <stdio.h>
#include <stdlib.h>
int readconf()
{
	int i;
	FILE *fp;
	fp = fopen("/etc/1652238.conf", "r");
	fscanf(fp, "�ӽ�������=%d", &i);
	return i;
}
