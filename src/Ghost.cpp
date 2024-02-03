#include "Ghost.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#define EPSILON 1e-6
static float toRad(float deg) { return deg * M_PI / 180.0f; }
Ghost::Ghost(const char* ModelFilePath, bool FitSize, Vector initScale)
{
	pModel = new Model();
	loadModels(ModelFilePath,FitSize,initScale);
	
}

Ghost::~Ghost()
{
	delete pModel;
}

void Ghost::update(float dtime) 
{
     float rotationPeriod = 6.0f; // 6 seconds
    float rotationSpeed = 360.0f / rotationPeriod; // Degrees per second
    float rotationAngle = fmod(dtime * rotationSpeed, 360.0f);
    Matrix mTTop, mOrientation;
   
    Matrix mTBot, offset, rot;
   
    rot.rotationY(toRad(rotationAngle));
    mTBot = rot;
  
    pModel->transform(transform()*mTBot); 
}

void Ghost::draw(const BaseCamera& Cam)
{
		pModel->draw(Cam);
}



