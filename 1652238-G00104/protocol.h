//��׿�� 1652292 �����2��
//ʱ�� 2018/12/05 20:48

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define MAX_PKT 1024
#define FILE_EXIST 0
#define RM_FAILURE -1

typedef unsigned int seq_nr; //�������

typedef struct
{
	unsigned char data[MAX_PKT];
} packet; //���ݰ� ������

typedef enum
{
	frame_arrival
} event_type; //ֻ�н��ն�һ���¼�����

typedef enum
{
	data,	 //���ݰ�
	ack,	  //ȷ�ϰ�
	nak		  //��ȷ�ϰ�
} frame_kind; //֡����ö����

typedef struct
{
	frame_kind kind; //֡����
	seq_nr seq;		 //�������
	seq_nr ack;		 //�������
	packet info;	 //���ݰ�
} frame;			 //֡�ṹ
// ������ͷβ��FLAG�ֽڡ�У���
// kind/seq/ack����֡ͷ�Ŀ�����Ϣ����

void wait_for_event(event_type);

void from_network_layer(packet *);
void to_network_layer(packet *);

void to_physical_layer(frame *);
void from_physical_layer(frame *);
