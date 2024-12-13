#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

void main(int argc,char**argv)
{
	if(argc!=2)
	{
		printf("usage ./a.out file\n");
		return;
	}

	int op,led,fd;
	char s[5];
	
	fd=open(argv[1],O_RDWR);
	if(fd<0)
	{
		perror("open");
		return;
	}
	while(1)
	{
	printf("Enter the LED.NO to do operations\n1)LED1\n2)LED2\n");
	scanf("%d",&op);
	s[0]=op+48;
	s[1]=',';
	if(op==1)
	{
		printf("Enter the operation\n0)OFF\n1)ON\n");
		scanf("%d",&led);
	        s[2]=led+48;
	        s[3]='\0';
	        write(fd,s,strlen(s)+1);
	        printf("\n%s\n\n",s);	
	}
	else if(op==2)
	{
		printf("Enter the operation\n0)OFF\n1)ON\n");
		scanf("%d",&led);
	        s[2]=led+48;
	        s[3]='\0';
	        write(fd,s,strlen(s)+1);
		printf("\n%s\n\n",s);
	}
	else
		printf("Unknown option\n");
	}
}

