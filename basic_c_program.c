#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
void main(int argc,char **argv)
{
	int fd,num;
	char s[20];
	fd=open("/dev/square_device",O_RDWR);
        perror("open");
	printf("Enter the number\n");
	scanf("%s",s);

	write(fd,s,strlen(s)+1);
//	sleep(2);
	read(fd,s,sizeof(s));
	printf("num=%s\n",s);

}
