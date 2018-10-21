// test5-2.c
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/prctl.h>

static void sig_child(int signo);

int main(int argc, char* argv[]){
	int pid;
	int pid_max = 1;
	long int i;
	if(argc == 1){
		printf("ȱ�ٲ��� �˳�\n");
		exit(0);
	}
	int times = atoi(argv[1]);
	if(times == -1)
		printf("ѭ���������޸��ӽ���\n",times);
	else
		printf("ѭ������%d���ӽ���\n",times);

	fflush(stdout);
	signal(SIGCHLD,sig_child);
	for(i=0;i<times||times==-1;i++){
		pid = fork();
		if(pid > pid_max)
			pid_max = pid;
		if(pid == 0){
			prctl(PR_SET_PDEATHSIG, SIGHUP);
			break;
		}
		else if(pid == -1){
			printf("�޷����� �ȴ�\n");
			fflush(stdout);
			sleep(1);
			//return 0;
		}
		else{
			printf("�ѷ��� %d ������\n",i+1);
			fflush(stdout);
		}
	}
	int counter = 0;
	while(1){
		
		if(pid == 0){
			char str[1024];
			int j;
			for(j=0;j<1024;j++)
				str[j]='0';
			sleep(1);
			return 0;
			//printf("%d %d 1652238 sub\n",getpid(),getppid());
			//fflush(stdout);
			//sleep(15);
			//counter++;

			// if(counter == 2){
			// 	printf("sub %d exiting\n",getpid());
			// 	fflush(stdout);
			// 	break;
			// }
		}else{
			printf("pid=%d ppid=%d 1652238 main biggest pid is %d\n",getpid(),getppid(),pid_max);
			fflush(stdout);
			sleep(5);
			
		} 
	};
	return 0;
}

static void sig_child(int signo){
	pid_t pid;
	int stat;
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0){
		printf("child pid=%d exited with signal %d\n",pid,signo);
		fflush(stdout);
	}
}  