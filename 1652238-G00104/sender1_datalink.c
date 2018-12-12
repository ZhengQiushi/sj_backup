//��׿�� 1652292 �ƿ�2��
//2018/12/06 16:13
#include "../common/protocol.h"

int main()
{
	ssize_t infile_no = 1, outfile_no = 1;
	int fd, readlen, fd_out;
	frame myframe;
	packet mypacket;
	char infile_name[64];
	char outfile_name[64];
	sprintf(infile_name, "../common/network_datalink.share.%04d", infile_no);

	while (1)
	{
		if ((fd = open(infile_name, O_RDONLY)) != -1)
		{
			readlen = read(fd, myframe.data, MAX_PKT); //��network_datalink.share�ļ��ж����ݸ���֡�����ݶ�

			if (readlen == MAX_PKT) //��֡д��datalink_physical.share�ļ� ȫ������1024+12=1036�ֽ�
			{
				sprintf(outfile_name, "../common/datalink_physical.share.%04d", outfile_no);
				while (access(outfile_name, F_OK) == FILE_EXIST)
					sleep(1);
				fd_out = open(outfile_name, O_WRONLY | O_CREAT);
				writelen = write(fd_out, (char *)&myframe, MAX_PKT + 12);
				if (writelen != MAX_PKT + 12)
				{
					perror("sender1_datalink write error ");
					exit(1);
				}
				close(fd_out); //
				outfile_no = (outfile_no + 1) % 10000;
			}

			close(fd); //�ر�infile��fd
			if (remove(infile_name) == RM_FAILURE)
			{
				perror("sender remove network_datalink error ");
				exit(1);
			} //ɾ��network_datalink.share�ļ�

			infile_no = (infile_no + 1) % 10000;
			sprintf(infile_name, "../common/network_datalink.share.%04d", infile_no);
		}
		else
			sleep(1);
	}

	return 0;
}
