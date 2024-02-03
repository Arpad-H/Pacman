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
	i.rotationYawPitchRoll(toRad(90),0,toRad(90));
	initTransform = i * pacmanModel->transform();
	speed = 1;
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
   //if (dir ==-1)return;
	
	std::cout << dir << std::endl;
	Matrix  m_trans, m_rot;
	m_trans.translation(speed * dtime, 0, 0);
	//m_rot.rotationY(toRad(dir));
	this->transform(transform() * m_trans);

	pacmanModel->transform(transform()*initTransform);

}
void Pacman::draw(const BaseCamera& Cam)
{
	pacmanModel->draw(Cam);
}