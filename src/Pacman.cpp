#include "Pacman.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "GlowShader.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
#define EPSILON 1e-6
static float toRad(float deg) { return deg * M_PI / 180.0f; }

Pacman::Pacman(const char* ModelFilePath, bool FitSize, Vector initScale)
{
	//GlowShader* pGlowShader;
	//EXPERIMENTAL GLOW
	pGlowShader = new GlowShader();

	pacmanModel = new Model();
	loadModels(ModelFilePath, FitSize, initScale, *pacmanModel);

	//EXPERIMENTAL GLOW
	pacmanModel->shader(pGlowShader, true);

	//scale
	scale = pacmanModel->transform();

	speed = 4;
	Matrix startPos,rotation;
	faceAdaptation.identity();
	rotation.rotationYawPitchRoll(toRad(  180), 0, 0);
	initTransform = rotation*pacmanModel->transform();
	startPos.translation(Vector(-15, 16, -15));
	pacmanModel->transform(startPos* initTransform);
	camRefrencePoint.translation(Vector(0, 0, 16));
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
	pos = pacmanModel->transform().translation() + posOffset;
	Vector activeFaceCenter = level->activeFace->faceModel->transform().translation();
	Vector levelboundsVec =  pos- activeFaceCenter;
	if (abs(levelboundsVec.X) > 16 || abs(levelboundsVec.Y) > 16|| abs(levelboundsVec.Z) > 16)
	{
		pos = pos - posOffset;
		pos.X = (int)(pos.X);
	    pos.Y = (int)(pos.Y);
		pos.Z = (int)(pos.Z);
		pos = pacmanModel->transform().translation() - pacmanModel->transform().up().toUnitVector() + posOffset.toUnitVector()*0.5 ;
		transition();
		reajust();
		updatedLoc.translation(pos);
		pacmanModel->transform(updatedLoc * initTransform * faceAdaptation * mrot);
		
		return;
	}else{
		float levelSize = level->size;
		
		Vector planeOfMovement = activeFaceCenter.notVector();
		int zeroIndex = planeOfMovement.zeroIndex();
		Vector locationToCheck = pos + pacmanModel->transform().forward().toUnitVector()*0.5;
		locationToCheck = locationToCheck + Vector(levelSize, levelSize, levelSize) * 0.5; // adjusted location
		locationToCheck = locationToCheck.componentwiseMult(planeOfMovement); //to 2d plane
		//cout << "pos: " << pos.X << " " << pos.Y << " " << pos.Z << endl;
		//cout << "locationToCheck: " << locationToCheck.X << " " << locationToCheck.Y << " " << locationToCheck.Z << endl;
		locationToCheck.X = (int)locationToCheck.X;
		locationToCheck.Y = (int)locationToCheck.Y;
		locationToCheck.Z = (int)locationToCheck.Z;

		//we now hav a location in the 2d plane of the active face adjusted to fit the maze structure

		int row=99;
		int col=99;
		if (zeroIndex == 0) //left or right
		{
			//has issues with row 0
			row = locationToCheck.Z;
			col =  abs(locationToCheck.Y - (levelSize - 1));
			if (activeFaceCenter.at(0) < 0)
			{
				row = locationToCheck.Z;
				col = locationToCheck.Y;
			}
		}
		else if (zeroIndex == 1)//top or bottom
		{
			
			row = locationToCheck.Z;
			col = locationToCheck.X;
			
			if (activeFaceCenter.at(1) < 0)
			{
				row = abs(locationToCheck.Z - (levelSize-1));
				col = locationToCheck.X;
			}
		}
		else if (zeroIndex == 2)//front or back
		{
			//has issues with col 0
			row = locationToCheck.Y;
			col = locationToCheck.X;
			if (activeFaceCenter.at(2) > 0)
			{
				row = abs(locationToCheck.Y - (levelSize - 1));
				col = locationToCheck.X;

			}
		}
		//cout << "row: " << row << " col: " << col << endl;
		if (row < levelSize && col < levelSize && row > 0 && col > 0)
		{
			if (level->isWall(row, col))
			{
				pos = pacmanModel->transform().translation();
			}
		}
		
	}
	snapToGrid(pos, posOffset);
	
	level->consumeDot(pos, score);
	
	if(level->checkGhostCollision(pos)){
				pos = pacmanModel->transform().translation();
				hitState = true;
				lives--;
	}
	updatedLoc.translation(pos);
	
	pacmanModel->transform(updatedLoc*initTransform*faceAdaptation* mrot);
	 
	
	//std::cout << "forward: " << pacmanModel->transform().forward().toUnitVector().X << " " << pacmanModel->transform().forward().toUnitVector().Y << " " << pacmanModel->transform().forward().toUnitVector().Z << std::endl;
	//std::cout << "right: " << pacmanModel->transform().right().toUnitVector().X << " " << pacmanModel->transform().right().toUnitVector().Y << " " << pacmanModel->transform().right().toUnitVector().Z << std::endl;
	//std::cout << "up: " << pacmanModel->transform().up().toUnitVector().X << " " << pacmanModel->transform().up().toUnitVector().Y << " " << pacmanModel->transform().up().toUnitVector().Z << std::endl;
	

}
void Pacman::draw(const BaseCamera& Cam)
{
	//EXPERIMENTAL GLOW
	//GlowShader* Shader = dynamic_cast<GlowShader*>(pacmanModel->shader());
	
//	Shader->mixTex(&MixTex);
	//GlowShader Shader = (GlowShader*)(pacmanModel->shader());
	//Shader.lightPos(pacmanModel->transform().up()*20 + pacmanModel->transform().translation());
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
	Vector pacmanLoc = pacmanModel->transform().translation();
	Face* closest = *level->Faces.begin();
	float closestDistance = 10000;
	for (Face* face : level->Faces) {
		Vector faceCenter = face->faceModel->transform().translation();
		Vector faceToPacman = pacmanLoc - faceCenter;
		float distance = faceToPacman.length();
		if (distance < closestDistance && face != level->activeFace) {
			closest = face;
			closestDistance = distance;
		}
	}
	level->activeFace = closest;


	Vector refrencePoint = camRefrencePoint.translation();
	closest = *level->Faces.begin();
	closestDistance = 10000;
	for (Face* face : level->Faces) {
		Vector faceCenter = face->faceModel->transform().translation();
		Vector faceToRefrencePoint = refrencePoint - faceCenter;
		float distance = faceToRefrencePoint.length();
		if (distance < closestDistance && face != level->activeFace) {
			closest = face;
			closestDistance = distance;
		}
	}
	level->forwardFacingFace = closest;
}

void Pacman::transition()
{
	
	transitionState = true;
	Matrix mrot1,mrot2;

	Vector left = pacmanModel->transform().left().toUnitVector()*90;
	if (left.X)
	{
		mrot1.rotationX(toRad(left.X));
		mrot2.rotationX(toRad(-left.X));
	}
	else if (left.Y)
	{
		mrot1.rotationY(-toRad(left.Y));
		mrot2.rotationY(-toRad(left.Y));
	}
	else if (left.Z)
	{
		mrot1.rotationZ(toRad(left.Z));
		mrot2.rotationZ(toRad(-left.Z));
	}

	faceAdaptation = mrot1*faceAdaptation;
	camRefrencePoint = mrot2* camRefrencePoint;
}

