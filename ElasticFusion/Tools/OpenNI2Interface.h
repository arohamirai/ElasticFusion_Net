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

#ifndef OPENNI2INTERFACE_H_
#define OPENNI2INTERFACE_H_

#include <OpenNI.h>
#include <PS1080.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>

#include "ThreadMutexObject.h"
#include "CameraInterface.h"

class OpenNI2Interface : public CameraInterface
{
    public:
        OpenNI2Interface(int inWidth = 640, int inHeight = 480, int fps = 30);
        virtual ~OpenNI2Interface();

        const int width, height, fps;

        virtual void setAutoExposure(bool value);
        virtual void setAutoWhiteBalance(bool value);

        virtual bool ok()
        {
            return initSuccessful;
        }
	virtual std::string error()
        {
	    return "error fun";
        }
		
    private:
	int64_t lastRgbTime;
        int64_t lastDepthTime;
        bool initSuccessful;
		
};

#endif /* OPENNI2INTERFACE_H_ */
