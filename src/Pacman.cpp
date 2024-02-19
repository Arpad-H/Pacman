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
	scale = pacmanModel->transform();

	speed = 4;
	Matrix startPos,rotation;
	faceAdaptation.identity();
	rotation.rotationYawPitchRoll(toRad(  180), 0, 0);
	initTransform = rotation*pacmanModel->transform();
	startPos.translation(Vector(0, 16, 0));
	pacmanModel->transform(startPos* initTransform);
	std::cout << "forward: " << pacmanModel->transform().forward().toUnitVector().X << " " << pacmanModel->transform().forward().toUnitVector().Y << " " << pacmanModel->transform().forward().toUnitVector().Z << std::endl;
	std::cout << "right: " << pacmanModel->transform().right().toUnitVector().X << " " << pacmanModel->transform().right().toUnitVector().Y << " " << pacmanModel->transform().right().toUnitVector().Z << std::endl;
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
	//mrot.rotationYawPitchRoll(toRad(dir), 0,0);
	Vector up = pacmanModel->transform().up().toUnitVector() * 90;
	if (up.X)
	{
		mrot.rotationX(toRad(up.X));
	}
	else if (up.Y)
	{
		mrot.rotationY(toRad(up.Y));
	}
	else if (up.Z)
	{
		mrot.rotationZ(toRad(up.Z));
	}
	mrot.rotationYawPitchRoll(toRad(dir), 0,0);
	//mrot.rotationY(toRad(dir));

	//calculate an offset to move the model
	Vector posOffset = pacmanModel->transform().forward().toUnitVector() * speed * dtime;
	
	Vector pos;
	Matrix updatedLoc;
	//add the new position to the current position
	//cout << pacmanModel->transform().translation().X << " " << pacmanModel->transform().translation().Y << " " << pacmanModel->transform().translation().Z << endl;
	pos = pacmanModel->transform().translation() + posOffset;
	
	//TODO cuz this aint working with snaping to grid
	//check if in levelbounds 
	Vector v = level->activeFace->faceModel->transform().translation();
	Vector levelboundsVec =  pos-v;
	if (abs(levelboundsVec.X) > 16 || abs(levelboundsVec.Y) > 16|| abs(levelboundsVec.Z) > 16)
	//if (false)
	{
		cout << "out of bounds" << endl;
		pos = pos - posOffset;
		pos.X = (int)(pos.X);
	    pos.Y = (int)(pos.Y);
		pos.Z = (int)(pos.Z);
		pos = pacmanModel->transform().translation() - pacmanModel->transform().up().toUnitVector() + posOffset.toUnitVector()*0.5 ;
		reajust(); //set the active face to the new face
		transition();
		updatedLoc.translation(pos);
		pacmanModel->transform(updatedLoc * initTransform * faceAdaptation * mrot);
		
		return;
	}else{
		
		//check if the new position is a wall
		int levelbounds = level->isWall(pos + pacmanModel->transform().forward().toUnitVector() * 0.5);
		if (levelbounds == 1)
		{
		pos = pacmanModel->transform().translation();
		}
	}
	//snapToGrid(pos, posOffset);
	
	level->consumeDot(pos);
	
	updatedLoc.translation(pos);
	
	pacmanModel->transform(updatedLoc*initTransform*faceAdaptation* mrot);
	 
	
	//std::cout << "forward: " << pacmanModel->transform().forward().toUnitVector().X << " " << pacmanModel->transform().forward().toUnitVector().Y << " " << pacmanModel->transform().forward().toUnitVector().Z << std::endl;
	//std::cout << "right: " << pacmanModel->transform().right().toUnitVector().X << " " << pacmanModel->transform().right().toUnitVector().Y << " " << pacmanModel->transform().right().toUnitVector().Z << std::endl;
	//std::cout << "up: " << pacmanModel->transform().up().toUnitVector().X << " " << pacmanModel->transform().up().toUnitVector().Y << " " << pacmanModel->transform().up().toUnitVector().Z << std::endl;
	

}
void Pacman::draw(const BaseCamera& Cam)
{
	pacmanModel->draw(Cam);
}

void Pacman::setLevel(Level* level)
{
	this->level = level;
}

void Pacman::registerCamera(Camera* camera)
{
	this->camera = camera;
}

void Pacman::snapToGrid(Vector &pos, Vector posOffset)
{
	
	if (abs(posOffset.toUnitVector().X) == 1)
	{
		pos.Z = (std::floor(pos.Z)) +0.5;
		pos.Y = (std::floor(pos.Y)) + 0.5;
	}
	else if (abs(posOffset.toUnitVector().Z) == 1)
	{
		pos.X = (std::floor(pos.X)) + 0.5;
		pos.Y = (std::floor(pos.Y)) + 0.5;
	}
	else if (abs(posOffset.toUnitVector().Y) == 1)
	{
		pos.X = (std::floor(pos.X)) + 0.5;
		pos.Z = (std::floor(pos.Z)) + 0.5;
	}
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

	Camera cam = *camera;
	Matrix temp = cam.getViewMatrix();
	temp.invert();
	temp = faceAdaptation*temp;
	Vector camPos = temp.translation();
 // biggest value of camPos	

	Vector offestToCenter = level->activeFace->faceModel->transform().translation() - pacmanModel->transform().translation();
	camPos = camPos + offestToCenter;
	Face* closest = *level->Faces.begin();
	float closestDistance = 10000;
	for (Face* face : level->Faces) {
		Vector faceCenter = face->faceModel->transform().translation();
		Vector faceToCam = camPos - faceCenter;
		float distance = faceToCam.length();
		if (distance < closestDistance && face != level->activeFace) {
			closest = face;
			closestDistance = distance;
		}
	}
	level->forwardFacingFace = closest;

	
	
}

void Pacman::transition()
{
	
	cout<<"transition"<<endl;
	transitionState = true;
	Matrix mrot1;

	Vector right = pacmanModel->transform().left().toUnitVector()*90;
	if (right.X)
	{
		mrot1.rotationX(toRad(right.X));
	}
	else if (right.Y)
	{
		mrot1.rotationY(toRad(right.Y));
	}
	else if (right.Z)
	{
		mrot1.rotationZ(toRad(right.Z));
	}
	
	

	faceAdaptation = mrot1*faceAdaptation;
}

