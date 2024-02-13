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
	
	speed = 3;
	Matrix startPos,rotation;
	rotation.rotationYawPitchRoll(toRad( + 180), 0, toRad(90));
	initTransform = rotation*pacmanModel->transform();
	startPos.translation(Vector(0, 16, 0));
	pacmanModel->transform(startPos* initTransform);
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

	Matrix  mtrans,  mrot,startloc, pcmat;
	
	//Handle rotation
	//TODO: now it only rotates correcly on the Y axis (yaw) so it only works on top and bottom face
	mrot.rotationYawPitchRoll(toRad(dir), 0,0);

	//calculate an offset to move the model
	Vector posOffset = pacmanModel->transform().forward().toUnitVector() * speed * dtime;
	
	Vector pos;
	Matrix updatedLoc;
	//add the new position to the current position
	pos = pacmanModel->transform().translation() + posOffset;
	
	//check if in levelbounds 
	Vector v = level->activeFace->faceModel->transform().translation();
	Vector levelboundsVec = v- pos;
	if (abs(levelboundsVec.X) > 16 || abs(levelboundsVec.Y) > 16 || abs(levelboundsVec.Z) > 16)
	{
		cout << "out of bounds" << endl;
		transition(dtime);
		pos = pacmanModel->transform().translation();
		reajust(); //set the active face to the new face

	}else{
		
		//check if the new position is a wall
		int levelbounds = level->isWall(pos + pacmanModel->transform().forward().toUnitVector() * 0.5);
		if (levelbounds == 1)
		{
		pos = pacmanModel->transform().translation();
		}
	}

	
	level->consumeDot(pos);
	
	//TODO
	//snap pacman to the grid
	/*if (abs(posOffset.toUnitVector().X) == 1)
	{
		pos.Z = (std::floor(pos.Z)) +0.5;
	}
	else if (abs(posOffset.toUnitVector().Z) == 1)
	{
		pos.X = (std::floor(pos.X)) + 0.5;
	}*/
	
	

	updatedLoc.translation(pos);
	
	pacmanModel->transform(updatedLoc *mrot*initTransform);

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

void Pacman::reajust()
{
	int index = 0;
	switch ((int)dir)
	{
	case 0:
		index = 0;
		break;
	case 90:
		index = 1;
		break;
	case 180:
		index = 2;
		break;
	case 270:
		index = 3;
		break;
	default:
		break;
	}
	level->activeFace = level->activeFace->neighbouringFaces[index];
}

void Pacman::transition(float time)
{
	
	cout<<"transition"<<endl;
	Matrix mrot;
	mrot.rotationYawPitchRoll(0, toRad(-90), toRad(0));
	initTransform = mrot*initTransform  ;
	
	
}

