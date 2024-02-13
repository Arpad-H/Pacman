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
	pModel = new BaseModel();
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
	Face* pFace_top = new Face(dimX, t * r);
	Faces.push_back(pFace_top);
	///////////////////////////////////////////////////////
	//bottom
	///////////////////////////////////////////////////////
	r.rotationZ(toRad(180));
	t.translation(0, -dimX/2, 0);
	Face* pFace_bottom = new Face(dimX,t*r);
	Faces.push_back(pFace_bottom);
	///////////////////////////////////////////////////////
	//left
	///////////////////////////////////////////////////////
	r.rotationZ(toRad(90));
	t.translation(-dimX / 2, 0, 0);
	Face* pFace_left = new Face(dimX, t * r);
	Faces.push_back(pFace_left);
	///////////////////////////////////////////////////////
	//right
	///////////////////////////////////////////////////////
	r.rotationZ(toRad(-90));
	t.translation(dimX / 2, 0, 0);
	Face* pFace_right = new Face(dimX, t * r);
	Faces.push_back(pFace_right);
	///////////////////////////////////////////////////////
	//back
	///////////////////////////////////////////////////////
	r.rotationX(toRad(-90));
	t.translation(0, 0, -dimX / 2);
	Face* pFace_back = new Face(dimX, t * r);
	Faces.push_back(pFace_back);
	///////////////////////////////////////////////////////
	//front
	///////////////////////////////////////////////////////
	r.rotationX(toRad(90));
	t.translation(0, 0, dimX / 2);
	Face* pFace_front = new Face(dimX, t * r);
	Faces.push_back(pFace_front);
	//                                 0            90           180          270
	pFace_top->setNeighbouringFaces(pFace_back, pFace_right, pFace_front, pFace_left);
	pFace_bottom->setNeighbouringFaces(pFace_front, pFace_right, pFace_back, pFace_left);
	pFace_front->setNeighbouringFaces(pFace_top, pFace_right, pFace_bottom, pFace_left);
	pFace_back->setNeighbouringFaces(pFace_bottom, pFace_right, pFace_top, pFace_left);
	pFace_left->setNeighbouringFaces(pFace_back, pFace_top, pFace_front, pFace_bottom);
	pFace_right->setNeighbouringFaces(pFace_back, pFace_bottom, pFace_front, pFace_top);
	
	activeFace = pFace_top;
	std::cout << "Level loaded" << std::endl;
	return true;
}
void Level::update(float dtime)
{
	
}
void Level::draw(const BaseCamera& Cam)
{
	for (FacesList::iterator it = Faces.begin(); it != Faces.end(); it++) {
		(*it)->draw(Cam);
	}
}

int Level::isWall(Vector pos)
{
	int z = (int)(pos.Z + (size / 2)) ;
	int x = (int)(pos.X + (size / 2)) ;
	if (z < 0 || z >= size || x < 0 || x >= size) return -1;
	return activeFace->layout->maze[z][x].isWall;
}
