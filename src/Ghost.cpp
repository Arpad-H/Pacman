#include "Ghost.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#define EPSILON 1e-6
static float toRad(float deg) { return deg * M_PI / 180.0f; }
Ghost::Ghost(const char* ModelFilePath, bool FitSize, Vector initScale)
{
	loadModels(ModelFilePath,FitSize,initScale);
	initTransform = pModel->transform();
}

Ghost::~Ghost()
{
}

void Ghost::update(float dtime) 
{
     Matrix rot;
	 rot.translation(3,0,0);
	 rot.rotationY(0.5);
	 Matrix m = pModel->transform();
	 pModel->transform(initTransform*rot);

}

void Ghost::draw(const BaseCamera& Cam)
{
		pModel->draw(Cam);
}



