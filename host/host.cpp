//http://www.itdadao.com/articles/c15a703614p0.html
/********************************
	zhanwang zhang
	keetsky@163.com
	2017.03.20
	电子哨兵项目：视频压缩实时传输
	主机端接受视频
调试原则：分别对主机和客户端前一或二次循环进行数据对比；如果相同表示发送完整
*********************************/

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

 	int listenfd,connfd;
 	struct sockaddr_in sockaddr;
 	memset(&sockaddr,0,sizeof(sockaddr));
 
 	sockaddr.sin_family = AF_INET;
 	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 	sockaddr.sin_port = htons(6666);
 
 	listenfd = socket(AF_INET,SOCK_STREAM,0);
 	bind(listenfd,(struct sockaddr *) &sockaddr,sizeof(sockaddr));
 	listen(listenfd,1);

 
 	printf("Please wait for the client information\n");
 	if((connfd = accept(listenfd,(struct sockaddr*)NULL,NULL))==-1)
 	{
 		printf("accpet socket error: %s errno :%d\n",strerror(errno),errno);
		return 0;	
 	}
	else
		printf("accpet sucess\n");
	
	int width = 640,height = 480;
 	while(true)   //每次循环表示对一帧（图）的数据处理
	 {      
		cout<<"while\n";
		int bytes=0;
		for(int k=0;k<MAXLINE;k+=bytes)//将接受的数据全部放入buff数组中
		{
	 		bytes=recv(connfd,&buff[0]+k,MAXLINE-k,0);//每次能够接受数据的长度，不同电脑接受数据长度不一样	cout<<"for\n"
			if(bytes==0)
			{
			cout<<"recv error"<<endl;
			break;
			}
		}
	 
		// 5.4 show the image
		Mat depth(height,width,CV_16UC1,buff);
		//normalize(depth,depth,1,0, NORM_MINMAX);
		Mat rgb(height,width,CV_8UC3,buff+width*height*2);
		
		imshow("recv_depth",depth);
		imshow("recv_rgb",rgb);
		waitKey(1);
 	}
	close(connfd);
 	close(listenfd);
 }




