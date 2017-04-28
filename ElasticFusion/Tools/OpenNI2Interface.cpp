/*
 * This file is part of ElasticFusion.
 *
 * Copyright (C) 2015 Imperial College London
 * 
 * The use of the code within this file and all code within files that 
 * make up the software that is ElasticFusion is permitted for 
 * non-commercial purposes only.  The full terms and conditions that 
 * apply to the code within this file are detailed within the LICENSE.txt 
 * file and at <http://www.imperial.ac.uk/dyson-robotics-lab/downloads/elastic-fusion/elastic-fusion-license/> 
 * unless explicitly stated.  By downloading this file you agree to 
 * comply with these terms.
 *
 * If you wish to use any of this code for commercial purposes then 
 * please email researchcontracts.engineering@imperial.ac.uk.
 *
 */

#include "OpenNI2Interface.h"
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
#include<pthread.h>

using namespace std ;
using namespace cv;

#define MAXLINE 640*480*5
int listenfd,connfd;
//OpenNI2Interface *pFace = NULL;
void *threadProc(void *param);

OpenNI2Interface::OpenNI2Interface(int inWidth, int inHeight, int fps)
 : width(inWidth),
   height(inHeight),
   fps(fps),
   initSuccessful(true)
{		
///////////////////////////////////////

    for(int i = 0; i < numBuffers; i++)
    {
        uint8_t * newDepth = (uint8_t *)calloc(width * height * 2, sizeof(uint8_t));
        uint8_t * newImage = (uint8_t *)calloc(width * height * 3, sizeof(uint8_t));
        frameBuffers[i] = std::pair<std::pair<uint8_t *, uint8_t *>, int64_t>(std::pair<uint8_t *, uint8_t *>(newDepth, newImage), 0);
    }
    latestDepthIndex.assign(-1);

    pthread_t t1;
    pthread_create( &t1, NULL, &threadProc, this);
}

OpenNI2Interface::~OpenNI2Interface()
{
    if(initSuccessful)
    {
        for(int i = 0; i < numBuffers; i++)
        {
            free(frameBuffers[i].first.first);
            free(frameBuffers[i].first.second);
        }
    }
}

void OpenNI2Interface::setAutoExposure(bool value)
{
   
}

void OpenNI2Interface::setAutoWhiteBalance(bool value)
{
    
}

void *threadProc(void *param)
{
	
	OpenNI2Interface *pFace = (OpenNI2Interface*)param;
	
	char buff[MAXLINE];
	memset(buff,0, MAXLINE);

	const char *servInetAddr = "192.168.1.190";
 	int socketfd;
 	struct sockaddr_in sockaddr;
	
	//socketfd = socket(AF_INET,SOCK_STREAM,0);
 	memset(&sockaddr,0,sizeof(sockaddr));
 	sockaddr.sin_family = AF_INET;
 	sockaddr.sin_port = htons(6666);
 	inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);

    while(1)
    {
	socketfd = socket(AF_INET,SOCK_STREAM,0);
        if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 )
        {
             close(socketfd);
             printf("connect error %s errno: %d\n",strerror(errno),errno);
             usleep(3);
             continue;
             //exit(0);
        }

        int width = 640,height = 480;
        while(true)   //每次循环表示对一帧（图）的数据处理
        {
            int bytes=0;
            int k=0;
            for(;k<MAXLINE;k+=bytes)//将接受的数据全部放入buff数组中
            {
                bytes=recv(socketfd,&buff[0]+k,MAXLINE-k,0);//每次能够接受数据的长度，不同电脑接受数据长度不一样
                if(bytes<=0)
                {
                    break;
                }
            }
	//*
            if(k != MAXLINE)
            {
		close(socketfd);
                break;
            }/**/
            int64_t lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch()).count();

            int bufferIndex = (pFace->latestDepthIndex.getValue() + 1) % pFace->numBuffers;

            memcpy(pFace->frameBuffers[bufferIndex].first.first, buff, width * height * 2);
            memcpy(pFace->frameBuffers[bufferIndex].first.second, buff+width * height * 2, width * height * 3);

            pFace->frameBuffers[bufferIndex].second = lastTime;
            pFace->latestDepthIndex++;
        }
    }
    return 0;
}
