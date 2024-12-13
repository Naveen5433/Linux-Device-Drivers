#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<signal.h>

int fd;
void sig_handler(int n)
{
	close(fd);
	exit(0);
}

void main()
{
	int pre=0,cur=0;
	char s[5];
	fd=open("/dev/pir_device",O_RDWR);
	if(fd==-1)
	{
		perror("open:");
		return;
	}

	signal(2,sig_handler);

	while(1)
	{
		read(fd,s,2);
		cur=atoi(s);
		printf("%d\n",cur);
		if(cur)
			write(fd,"1",2);
		else
			write(fd,"0",2);
		
	}
}

