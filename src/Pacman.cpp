#include "Pacman.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#define EPSILON 1e-6
static float toRad(float deg) { return deg * M_PI / 180.0f; }

Pacman::Pacman(const char* ModelFilePath, bool FitSize, Vector initScale)
{
	pacmanModel = new Model();
	loadModels(ModelFilePath, FitSize, initScale, *pacmanModel);
	Matrix i;
	i.rotationYawPitchRoll(toRad(90),0,0);
	
	initTransform =  pacmanModel->transform();
	speed = 3;
}
Pacman::~Pacman()
{

}
void Pacman::steer(float dir)
{
	this->dir = dir;
}
void Pacman::update(float dtime)
{
	if (dir == -1)return;
	/*std::cout << "dir: " << dir << std::endl;
	std::cout << "sin: " << (int)sin(toRad(dir)) << std::endl;
	std::cout << "cos: " << (int)cos(toRad(dir)) << std::endl;*/
	Matrix  m_trans, m_rot,startloc;
	//startloc.translation(0, 16, 0);
	m_trans.translation(-(int)sin(toRad(dir)) *speed*dtime, 0, -(int)cos(toRad(dir)) * speed * dtime);
	this->transform(transform() * m_trans);
	Matrix i,final;
	i.rotationYawPitchRoll(toRad(dir+180), 0, toRad(90));
	final = i* initTransform;
	pacmanModel->transform(transform()*final);
	

}
void Pacman::draw(const BaseCamera& Cam)
{
	pacmanModel->draw(Cam);
}