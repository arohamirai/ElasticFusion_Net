
// STL Header
#include <iostream>

// 1. include OpenNI Header
#include "OpenNI.h"

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
#include <netinet/in.h>
#include <fstream>
#include <cv.h>
#include <vector>
#include <time.h>
#include <sys/time.h> 

using namespace cv;

#define MAXLINE 640*480*5


int main(int argc, char** argv)
{
	// init network
	const char *servInetAddr = "127.0.0.1";
 	int socketfd;
 	struct sockaddr_in sockaddr;
    uchar sendline[MAXLINE];
	memset(sendline,0, MAXLINE);
	
	socketfd = socket(AF_INET,SOCK_STREAM,0);
 	memset(&sockaddr,0,sizeof(sockaddr));
 	sockaddr.sin_family = AF_INET;
 	sockaddr.sin_port = htons(6666);
 	inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);
 	if((connect(socketfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0 )
 	{
		 printf("connect error %s errno: %d\n",strerror(errno),errno);
 		exit(0);
 	}
	

	
	//1. Setup
	int width = 640,height = 480,fps = 30;
    openni::Status rc = openni::STATUS_OK;
    const char * deviceURI = openni::ANY_DEVICE;
	
	// 2. initialize
    rc = openni::OpenNI::initialize();
	
	//3. open device
	openni::Device devAnyDevice;
	rc = devAnyDevice.open(deviceURI);
	//4. set Mode
    openni::VideoMode depthMode;
    depthMode.setFps(fps);
    depthMode.setPixelFormat(openni::PIXEL_FORMAT_DEPTH_1_MM);
    depthMode.setResolution(width, height);

    openni::VideoMode colorMode;
    colorMode.setFps(fps);
    colorMode.setPixelFormat(openni::PIXEL_FORMAT_RGB888);
    colorMode.setResolution(width, height);

	// 4.1 create streamDepth and setMode
	openni::VideoStream streamDepth;
    rc = streamDepth.create(devAnyDevice, openni::SENSOR_DEPTH);
    streamDepth.setVideoMode(depthMode);
    rc = streamDepth.start();
       
			
	//4.2 create rgbStream and setMode
	openni::VideoStream streamRgb;
    rc = streamRgb.create(devAnyDevice, openni::SENSOR_COLOR);
    streamRgb.setVideoMode(colorMode);
    rc = streamRgb.start();


	// 5. main loop, continue read
	openni::VideoFrameRef frameDepth;
	openni::VideoFrameRef frameRgb;
	
	unsigned char *depthBuf = (unsigned char*)malloc(width*height*2);
	unsigned char *rgbBuf = (unsigned char*)malloc(width*height*3);
	namedWindow("depth",1);
	namedWindow("rgb",1);
	for (; ;)
	{
		// 5.1 get frame
		streamDepth.readFrame(&frameDepth);
		streamRgb.readFrame(&frameRgb);
		
		
		// 5.2 get data array
		memcpy(depthBuf, frameDepth.getData(),width*height*2);
		memcpy(rgbBuf, frameRgb.getData(),width*height*3);
		
		
		// 5.3 send data
		memcpy(sendline,frameDepth.getData(),width*height*2);
		memcpy(sendline+width*height*2,frameRgb.getData(),width*height*3);
		
		int n=send(socketfd,(const char*)(&sendline),MAXLINE,0);
		
		// 5.4 show the image
		Mat depth(height,width,CV_16UC1,depthBuf);
		//normalize(depth,depth,1,0, NORM_MINMAX);
		Mat rgb(height,width,CV_8UC3,rgbBuf);
		
		imshow("depth",depth);
		imshow("rgb",rgb);
		
		waitKey(30);
		
	
	}

	// 6. close
	streamDepth.destroy();
	streamRgb.destroy();
	devAnyDevice.close();

	// 7. shutdown
	openni::OpenNI::shutdown();

	free(depthBuf);
	free(rgbBuf);
	return 0;
}