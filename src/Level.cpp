#include "Level.h"
#include "model.h"
#include "phongshader.h"
#include "trianglePlanemodel.h"
#include "constantshader.h"
#include "linePlanemodel.h"
#include <stack>
#include "vector.h"
#include "TriangleBoxModel.h"
#include "Maze.h"
#include "Face.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
static float toRad(float deg) { return deg * M_PI / 180.0f; }
using namespace std;
Level::Level()
{
}
Level::Level( GLuint SkyboxTexID)
{
	pModel = new BaseModel();
	this->SkyboxTexID = SkyboxTexID;
}
Level::~Level()
{
}
bool Level::loadLevel(float dimX, float dimY, float segments)
{
	size = dimX;
	Matrix r,t;
	///////////////////////////////////////////////////////
	//top
	///////////////////////////////////////////////////////
	r.rotationZ(toRad(0));
	t.translation(0, dimX / 2, 0);
	Face* pFace_top = new Face(dimX, t , r,SkyboxTexID);
	Faces.push_back(pFace_top);
	///////////////////////////////////////////////////////
	//bottom
	///////////////////////////////////////////////////////
	r.rotationX(toRad(180));
	t.translation(0, -dimX/2, 0);
	Face* pFace_bottom = new Face(dimX,t,r, SkyboxTexID);
	Faces.push_back(pFace_bottom);
	///////////////////////////////////////////////////////
	//left
	///////////////////////////////////////////////////////
	r.rotationZ(toRad(90));
	
	t.translation(-dimX / 2, 0, 0);
	Face* pFace_left = new Face(dimX, t , r, SkyboxTexID);
	Faces.push_back(pFace_left);
	///////////////////////////////////////////////////////
	//right
	///////////////////////////////////////////////////////
	r.rotationZ(toRad(-90));
	
	t.translation(dimX / 2, 0, 0);
	Face* pFace_right = new Face(dimX, t , r, SkyboxTexID);
	Faces.push_back(pFace_right);
	///////////////////////////////////////////////////////
	//back
	///////////////////////////////////////////////////////
	r.rotationX(toRad(-90));
	t.translation(0, 0, -dimX / 2);
	Face* pFace_back = new Face(dimX, t , r, SkyboxTexID);
	Faces.push_back(pFace_back);
	///////////////////////////////////////////////////////
	//front
	///////////////////////////////////////////////////////
	r.rotationX(toRad(90));
	t.translation(0, 0, dimX / 2);
	Face* pFace_front = new Face(dimX, t , r, SkyboxTexID);
	Faces.push_back(pFace_front);
	/*                                0            90           180          270
	pFace_top->setNeighbouringFaces(pFace_back, pFace_left, pFace_front, pFace_right);
	pFace_bottom->setNeighbouringFaces(pFace_front, pFace_right, pFace_back, pFace_left);
	pFace_front->setNeighbouringFaces(pFace_top, pFace_right, pFace_bottom, pFace_left);
	pFace_back->setNeighbouringFaces(pFace_bottom, pFace_right, pFace_top, pFace_left);
	pFace_left->setNeighbouringFaces(pFace_back, pFace_bottom, pFace_front, pFace_top);
	pFace_right->setNeighbouringFaces(pFace_back, pFace_bottom, pFace_front, pFace_top);*/
	
	activeFace = pFace_top;
	forwardFacingFace = pFace_front;
	std::cout << "Level loaded" << std::endl;
	return true;
}

void Level::tellFaceLocation(Vector pos) {
	activeFace->setTarget(pos);
}

void Level::update(float dtime)
{
	activeFace->update(dtime);
}
void Level::draw(const BaseCamera& Cam)
{
	for (FacesList::iterator it = Faces.begin(); it != Faces.end(); it++) {
		//(*it)->draw(Cam);
	}
	activeFace->draw(Cam);
}

int Level::isWall(float row, float col)
{
	return activeFace->layout->maze[row][col].isWall;
}
void Level::consumeDot(Vector pos, int &score)
{

	for (ModelList::iterator it = activeFace->DotModels.begin(); it != activeFace->DotModels.end(); it++) {
		if (((*it)->transform().translation() - pos).length() < 0.8f) {
			activeFace->DotModels.erase(it);
			score += 10;
			return;
		}
	}
}
bool Level::checkGhostCollision(Vector pos)
{
	for (ModelList::iterator it = activeFace->GhostModels.begin(); it != activeFace->GhostModels.end(); it++) {
		if (((*it)->transform().translation() - pos).length() < 0.8f) {
			return true;
		}
	}
	return false;
}

