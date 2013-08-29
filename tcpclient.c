/* tcpclient.c */
#include<dirent.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char* argv[])

{

	int sock, bytes_recieved;  
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr;  
	int n1;
	host = gethostbyname("127.0.0.1");

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}
	//scanf("%d",&n1);

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(atoi(argv[1]));   
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8); 

	if (connect(sock, (struct sockaddr *)&server_addr,
				sizeof(struct sockaddr)) == -1) 
	{
		perror("Connect");
		exit(1);
	}

	int pid=fork();
	if(pid==0)
	{
		bytes_recieved=recv(sock,recv_data,1024,0);
		recv_data[bytes_recieved] = '\0';
		char temmp[100];
		while(strcmp(recv_data,"q")!=0)
		{
			if(strcmp(recv_data,"deny")==0)
			{
				printf("permission denied\n");
			}
			else
			{
			printf("%s\n",recv_data);
			}
			char *str,*command[100];
			int c=0;
			str=strtok(recv_data," ");
			while(str!=NULL)
			{
				command[c]=str;
				c++;
				str=strtok(NULL," ");
			}
			if(strcmp(command[0],"randd")==0)
			{
				
				send(sock,temmp,strlen(temmp),0);
				temmp[0]='\0';
			}
			if(strcmp(command[0],"FileUpload")==0)
			{
				char perm[100];
				printf("Want %s, enter allow/deny\n",command[1]);
				strcpy(temmp,command[1]);
				//childonly=1;
				//scanf("%s",perm);
				/*strcpy(perm,"allow");
				printf("perm1:: %s\n",perm);
				strcat(perm,"up");
				strcat(perm," ");
				strcat(perm,command[1]);
				printf("perm:: %s\n",perm);
				//send(connected,perm,strlen(perm),0);
*/
			}
			if(strcmp(command[0],"FileDownload")==0)
			{
				char head1[100];
				strcpy(head1,"fdstart");
				//char fname[100];
				send(sock,head1,strlen(head1),0);
				send(sock,command[1],strlen(command[1]),0);
				char str2[100];
				int rr=recv(sock,str2,1024,0);
				str2[rr]='\0';
				if(strcmp(str2,"fnameack")==0)
				{
				int fd=open(command[1],O_RDONLY,"r");
				char buf[1024];
				int x;
				while(x=read(fd,buf,1024))
				{
					send(sock,buf,x,0);
					//printf("bheja\n");
					char ack[100];
					int r2=recv(sock,ack,1024,0);
					ack[r2]='\0';
					if(strcmp(ack,"packack")==0)
					{

					bzero(buf,1024);
					continue;
					}
				}
				close(fd);
				char end1[100];
				strcpy(end1,"fdend");
				send(sock,end1,strlen(end1),0);
				printf("end bheja\n");
				int a=dup(1);
				dup2(sock,1);
				char new[100];
				strcpy(new,"echo $(md5sum ");
				strcat(new,command[1]);
				strcat(new,") $(stat ");
				strcat(new,command[1]);
				strcat(new," | grep Modify | cut -d \" \" -f2-3) ");
				strcat(new,"$(ls -lh | tr -s \" \" |grep ");
				strcat(new,command[1]);
				strcat(new," | cut -d \" \" -f5)");
				system(new);
				dup2(a,1);


				}
			}
			if(strcmp(command[0],"allowup")==0)
			{
				char head1[100];
				strcpy(head1,"fdstart");
				//char fname[100];
				send(sock,"randd",5,0);
				char nm[100];
				int rrr=recv(sock,nm,1024,0);
				send(sock,head1,strlen(head1),0);
				nm[rrr]='\0';
				send(sock,nm,strlen(nm),0);
				char str2[100];
				int rr=recv(sock,str2,1024,0);
				str2[rr]='\0';
				if(strcmp(str2,"fnameack")==0)
				{
				int fd=open(nm,O_RDONLY,"r");
				char buf[1024];
				int x;
				while(x=read(fd,buf,1024))
				{
					send(sock,buf,x,0);
					//printf("bheja\n");
					char ack[100];
					int r2=recv(sock,ack,1024,0);
					ack[r2]='\0';
					if(strcmp(ack,"packack")==0)
					{

					bzero(buf,1024);
					continue;
					}

					//bzero(buf,1024);
				}
				close(fd);
				char end1[100];
				strcpy(end1,"fdend");
				send(sock,end1,strlen(end1),0);
				int a=dup(1);
				dup2(sock,1);
				char new[100];
				strcpy(new,"echo $(md5sum ");
				strcat(new,command[1]);
				strcat(new,") $(stat ");
				strcat(new,command[1]);
				strcat(new," | grep Modify | cut -d \" \" -f2-3) ");
				strcat(new,"$(ls -lh | tr -s \" \" |grep ");
				strcat(new,command[1]);
				strcat(new," | cut -d \" \" -f5)");
				system(new);
				dup2(a,1);

	
				//printf("end bheja\n");
	
			}
			}
			
			if(strcmp(command[0],"fdstart")==0)
			{
				printf("aaya\n");
				char fname[125];
				int r=recv(sock,fname,1024,0);
				fname[r]='\0';
				send(sock,"fnameack",8,0);
				printf("File Downloading: %s\n",fname);
				fflush(stdout);
				int fd;
				FILE *fp1;
				char buf[1024];
				//fd=open(fname,O_CREAT | O_WRONLY,"w");
				fp1=fopen(fname,"w");
				printf("Value of fd:%d\n",fd);
				while(1)
				{
				//receiving file
			//	printf("in the loop\n");
				fflush(stdout);
				r=recv(sock,buf,1024,0);
				buf[r]='\0';
				send(sock,"packack",7,0);
				if(strcmp(buf,"fdend")==0)
					break;
				//write(fd,buf,r);
				//bzero(buf,1024);
				fprintf(fp1,"%s",buf);
				buf[0]='\0';
					
				}
				fclose(fp1);
			}
			if(strcmp(command[0],"IndexGet")==0)
			{

				if(strcmp(command[1],"short")==0)
				{
					if(fork())
					{

						/*int fd;
						  fd=open("temp", O_RDWR | O_CREAT);
						  dup2(fd,1);
						  close(fd);*/
						//execlp("/bin/ls","ls","-lct",NULL);
						system("ls --full-time -t | tr -s \" \" | cut -d \" \" -f 6-7 | tr  -d \"-\" | tr -d \":\"| cut -d \".\" -f 1 | tr -d \" \" > temp");
						_exit(0);
					}
					else
					{
						wait();

					}
					//fflush(stdout);
					FILE *fp1,*fp2;
					fp1=fopen("temp","r");
					char t1[100];
					int var=0,ft1=0,ft2=0;
					int upr,lwr;
					while(fgets(t1,sizeof(t1),fp1) !=NULL)
					{
						int len=strlen(t1)-1;
						if(t1[len]=='\n')
							t1[len]=0;
						printf("\n%s and var:%d",t1,var);
						if(var==0)
						{	var++;
							continue;
						}
						if(strcmp(t1,command[3])<=0 && ft1==0)
						{
							ft1=1;
							upr=var-1;
						}
						else if(strcmp(t1,command[2])<=0 && ft2==0)
						{
							ft2=1;
							lwr=var-1;
						}
						var++;
						

					}
					if(lwr==0)
						lwr=var;
					printf("\nupr:%d and lwr:%d\n",upr,lwr);
					var=0;
					fclose(fp1);
					if(fork())
					{

						system("ls --full-time -t > temp2");
						_exit(0);
					}
					else
					{
						wait();

					}
					fp2=fopen("temp2","r");
					while(fgets(t1,sizeof(t1),fp2)!=NULL)
					{
						if(var>upr && var<=lwr)
						{
							int len=strlen(t1)-1;
							if(t1[len]=='\n')
								t1[len]=0;
							//printf("aayaa\n");
							send(sock,t1,strlen(t1),0);

						}
						var++;
						
					}
					fclose(fp2);


				}
				if(strcmp(command[1],"long")==0)
				{
					
					dup2(sock,1);
					if(fork())
					{
						
						system("ls --full-time -t");
						_exit(0);
					}
					else
						wait();
				}
				if(strcmp(command[1],"RegEx")==0)
				{
					dup2(sock,1);
					if(fork())
					{
						char testing[100];
						strcpy(testing,"ls --full-time -t ");
						strcat(testing,command[2]);
						system(testing);
					}
					else
						wait();
				}
			}
			if(strcmp(command[0],"FileHash")==0)
			{
				if(strcmp(command[1],"Verify")==0)
				{
					//int fd2;
					char fname[100];
					int a=dup(1);
					dup2(sock,1);
					char new[100];
					strcpy(new,"echo $(md5sum ");
					strcat(new,command[2]);
					strcat(new,") $(stat ");
					strcat(new,command[2]);
					strcat(new," | grep Modify | cut -d \" \" -f2-3)");
					system(new);
					dup2(a,1);

					
				}
				if(strcmp(command[1],"CheckAll")==0)
				{
					int a=dup(1);
					dup2(sock,1);
					DIR           *d;
					struct dirent *dir;
					d = opendir(".");
					if (d)
					{
						while ((dir = readdir(d)) != NULL)
						{
							if(dir->d_type!=DT_DIR)
							{
								//printf("%s\n", dir->d_name);
								char new[100];
								strcpy(new,"echo $(md5sum ");
								strcat(new,dir->d_name);
								strcat(new,") $(stat ");
								strcat(new,dir->d_name);
								strcat(new," | grep Modify | cut -d \" \" -f2-3)");
								system(new);

							}
						}

						closedir(d);
					}
					dup2(a,1);
				}

			}

			bytes_recieved=recv(sock,recv_data,1024,0);
			recv_data[bytes_recieved] = '\0';
		}
		//continue;
	}
	else
	{
		gets(send_data);
		while(strcmp(send_data,"q")!=0)
		{
			if(strcmp(send_data,"allow")==0)
			{
				strcat(send_data,"up ");
				printf("aaaayaaaaa\n");
				//strcat(send_data
			}

			send(sock,send_data,strlen(send_data), 0);
			gets(send_data);
		}

		send(sock,send_data,strlen(send_data), 0);


	}
	close(sock);
	return 0;
}
