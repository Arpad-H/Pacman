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

	//scale
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
	if (transitionTime > 0)
	{
		transitionTime -= dtime;
		transition(transitionTime);
		return;
	}
	Matrix  mtrans,  mrot,startloc, pcmat,pcmat2;
	//rotate Model so it lies flat on the ground
	mrot.rotationYawPitchRoll(toRad(dir + 180), 0, toRad(90));
	
	//calculate an offset to move the model
	//Vector pos =Vector(-(int)sin(toRad(dir)) * speed * dtime, 0, -(int)cos(toRad(dir)) * speed * dtime);
	Vector posOffset = pacmanModel->transform().forward().toUnitVector() * speed * dtime;
	
	Vector pos;
	Matrix currentLoc;
	//add the new position to the current position
	//Pacmans height is always 16. Half of the level height
	pos = pacmanModel->transform().translation() + posOffset;
	
	//check if the new position is a wall
	int levelbounds = level->isWall(pos + pacmanModel->transform().forward().toUnitVector() * 0.5);
	if (levelbounds == 1)
	{
		pos = pacmanModel->transform().translation();
	}
	else if(levelbounds == -1)
	{	
		// new position is outside the level
		//initiate transition
		transition(1.0f);
		return;
	}


	

	pos.Y = 16;

	//snap pacman to the grid
	if (abs(posOffset.toUnitVector().X) == 1)
	{
		pos.Z = (std::floor(pos.Z)) +0.5;
	}
	else if (abs(posOffset.toUnitVector().Z) == 1)
	{
		pos.X = (std::floor(pos.X)) + 0.5;
	}
	
	

	currentLoc.translation(pos);
	pcmat2 =  currentLoc;
	pacmanModel->transform(pcmat2*mrot*initTransform);
	// print pacmanModel->transform().forward x y and z 
	//std::cout << "forward: " << pacmanModel->transform().forward().X << " " << pacmanModel->transform().forward().Y << " " << pacmanModel->transform().forward().Z << std::endl;
	

}
void Pacman::draw(const BaseCamera& Cam)
{
	pacmanModel->draw(Cam);
}

void Pacman::setLevel(Level* level)
{
	this->level = level;
}

void Pacman::transition(float time)
{
	Matrix mrot;
	mrot.rotationYawPitchRoll(level->activeFace->faceModel->transform().up());
}

