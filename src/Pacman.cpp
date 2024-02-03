#include "Pacman.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#define EPSILON 1e-6
static float toRad(float deg) { return deg * M_PI / 180.0f; }

Pacman::Pacman(const char* ModelFilePath, bool FitSize, Vector initScale)
{
	loadModels(ModelFilePath, FitSize, initScale);
	initTransform = pModel->transform();
}
Pacman::~Pacman()
{

}
void Pacman::steer(float forwardBackward, float leftRight)
{
	ForwardBackward = forwardBackward;
	LeftRight = leftRight;
}
void Pacman::update(float dtime)
{
	Matrix trans,final;
	trans.translation(ForwardBackward*dtime, 0, 0);
	final = pModel->transform()* initTransform;
	pModel->transform(final);
}
void Pacman::draw(const BaseCamera& Cam)
{
	pModel->draw(Cam);
}