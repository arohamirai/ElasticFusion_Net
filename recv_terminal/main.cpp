#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <fstream>
#include <cv.h>
#include <vector>
#include<iostream>
#include<time.h>
#include<sys/time.h>
#define MAXLINE 640*480*5
using namespace std ;
using namespace cv;


 int main(int argc,char **argv)
 {
	char buff[MAXLINE];
	memset(buff,0, MAXLINE);

	const char *servInetAddr = "192.168.1.200";
 	struct sockaddr_in sockaddr;
 	memset(&sockaddr,0,sizeof(sockaddr));
 	sockaddr.sin_family = AF_INET;
 	sockaddr.sin_port = htons(6666);
 	inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);
	int socketfd;

	while(1)
{
	socketfd = socket(AF_INET,SOCK_STREAM,0);
 	if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 )
 	{
		 printf("connect error %s errno: %d\n",strerror(errno),errno);
		close(socketfd);
 		continue;
 	}

	int width = 640,height = 480;
 	while(true)   //每次循环表示对一帧（图）的数据处理
	 {      
		cout<<"while\n";
		int bytes=0;
		int k=0;
		for(;k<=MAXLINE;k+=bytes)//将接受的数据全部放入buff数组中
		{
	 		bytes=recv(socketfd,&buff[0]+k,MAXLINE-k,0);//每次能够接受数据的长度，不同电脑接受数据长度不一样
			if(bytes==0)
			break;
		}
		if(k != MAXLINE)
		{
			close(socketfd);
			break;
		}

	 
		// 5.4 show the image
		Mat depth(height,width,CV_16UC1,buff);
		//normalize(depth,depth,1,0, NORM_MINMAX);
		Mat rgb(height,width,CV_8UC3,buff+width*height*2);
		
		imshow("recv_depth",depth);
		imshow("recv_rgb",rgb);

		// test reconnect
		
		waitKey(1);
 	}
}


	close(socketfd);
 }




