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
	delete pModel;
}
bool Level::loadLevel(float dimX, float dimY, float segments)
{
	//PhongShader* pPhongShader = new PhongShader();
	ConstantShader* pConstShader;
	ConstantShader* pConstShader2;
	ConstantShader* pConstShader3;
	ConstantShader* pConstShader4;
	ConstantShader* pConstShader5;
	ConstantShader* pConstShader6;
	PhongShader* pPhongshader = new PhongShader();
	
	//bottom
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	Matrix m, t, f;
	pModel = new TrianglePlaneModel(dimX, dimY, 1, 1);
	pConstShader = new ConstantShader();
	pConstShader->color(Color(1, 1, 1));
	pModel->shader(pConstShader, true);
	//generate maze
	Maze* maze;
	maze = new Maze(dimX);
	Model* pWall;
	for (int i = 0; i < dimX; ++i) {
		for (int j = 0; j < dimY; ++j) {
			if(maze->maze[i][j].isWall){
				pWall = new Model(ASSET_DIRECTORY "cube.dae",true,Vector(1,1,1));
				pWall->shader(pPhongshader, true);
				pWall->transform(m.translation(dimX/2-i-0.5, -dimY/2-0.5, dimY / 2-j-0.5)* pWall->transform());
				Models.push_back(pWall);
				
			}
		}
	}
	pModel->transform(m.translation(0, -dimY / 2, 0));
	Models.push_back(pModel);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	//top
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	pModel = new TrianglePlaneModel(dimX, dimY, 1, 1);
	pConstShader2 = new ConstantShader();
	pConstShader2->color(Color(1, 0, 0));
	pModel->shader(pConstShader, true);
	maze = new Maze(dimX);
	for (int i = 0; i < dimX; ++i) {
		for (int j = 0; j < dimY; ++j) {
			if (maze->maze[i][j].isWall) {
				pWall = new Model(ASSET_DIRECTORY "cube.dae", true, Vector(1, 1, 1));
				pWall->shader(pPhongshader, true);
				pWall->transform(m.translation(dimX / 2 - i - 0.5, dimY / 2 + 0.5, dimY / 2 - j - 0.5) * pWall->transform());
				Models.push_back(pWall);

			}
		}
	}
	pModel->transform(m.translation(0, dimY / 2, 0));
	Models.push_back(pModel);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	//left
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	pModel = new TrianglePlaneModel(dimX, dimY, 1, 1);
	pConstShader3 = new ConstantShader();
	pConstShader3->color(Color(0, 1, 0));
	pModel->shader(pConstShader, true);
	m.rotationZ(toRad(90));
	t.translation(0, dimX / 2, 0);
	f = m * t;
	maze = new Maze(dimX);
	for (int i = 0; i < dimX; ++i) {
		for (int j = 0; j < dimY; ++j) {
			if (maze->maze[i][j].isWall) {
				pWall = new Model(ASSET_DIRECTORY "cube.dae", true, Vector(1, 1, 1));
				pWall->shader(pPhongshader, true);
				pWall->transform(m.translation(-dimY / 2 - 0.5,dimX / 2 - i - 0.5, dimY / 2 - j - 0.5 ) * pWall->transform());
				Models.push_back(pWall);

			}
		}
	}
	pModel->transform(f);
	Models.push_back(pModel);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	//right
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	pModel = new TrianglePlaneModel(dimX, dimY, 1, 1);
	pConstShader4 = new ConstantShader();
	pConstShader4->color(Color(0, 0, 1));
	pModel->shader(pConstShader, true);
	m.rotationZ(toRad(90));
	t.translation(0, -dimX / 2, 0);
	f = m * t;
	maze = new Maze(dimX);
	for (int i = 0; i < dimX; ++i) {
		for (int j = 0; j < dimY; ++j) {
			if (maze->maze[i][j].isWall) {
				pWall = new Model(ASSET_DIRECTORY "cube.dae", true, Vector(1, 1, 1));
				pWall->shader(pPhongshader, true);
				pWall->transform(m.translation(dimY / 2 + 0.5, dimX / 2 - i - 0.5, dimY / 2 - j - 0.5) * pWall->transform());
				Models.push_back(pWall);

			}
		}
	}
	pModel->transform(f);
	Models.push_back(pModel);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	//front
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	pModel = new TrianglePlaneModel(dimX, dimY, 1, 1);
	pConstShader5 = new ConstantShader();
	pConstShader5->color(Color(1, 1, 0));
	pModel->shader(pConstShader, true);

	m.rotationYawPitchRoll(0, toRad(90), 0);
	t.translation(0, 0, dimX / 2);
	f = t * m;
	maze = new Maze(dimX);
	for (int i = 0; i < dimX; ++i) {
		for (int j = 0; j < dimY; ++j) {
			if (maze->maze[i][j].isWall) {
				pWall = new Model(ASSET_DIRECTORY "cube.dae", true, Vector(1, 1, 1));
				pWall->shader(pPhongshader, true);
				pWall->transform(m.translation(dimX / 2 - i - 0.5, dimY / 2 - j - 0.5, dimY / 2 + 0.5) * pWall->transform());
				Models.push_back(pWall);

			}
		}
	}
	pModel->transform(f);
	Models.push_back(pModel);
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	//back
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	pModel = new TrianglePlaneModel(dimX, dimY, 1, 1);
	pConstShader6 = new ConstantShader();
	pConstShader6->color(Color(0, 1, 1));
	pModel->shader(pConstShader, true);

	m.rotationYawPitchRoll(0, toRad(90), 0);
	t.translation(0, 0, -dimX / 2);
	f = t * m;
	maze = new Maze(dimX);
	for (int i = 0; i < dimX; ++i) {
		for (int j = 0; j < dimY; ++j) {
			if (maze->maze[i][j].isWall) {
				pWall = new Model(ASSET_DIRECTORY "cube.dae", true, Vector(1, 1, 1));
				pWall->shader(pPhongshader, true);
				pWall->transform(m.translation(dimX / 2 - i - 0.5, dimY / 2 - j - 0.5, -dimY / 2 - 0.5) * pWall->transform());
				Models.push_back(pWall);

			}
		}
	}
	pModel->transform(f);
	Models.push_back(pModel);


	return true;
}
void Level::update(float dtime)
{

}
void Level::draw(const BaseCamera& Cam)
{
	for (ModelList::iterator it = Models.begin(); it != Models.end(); it++) {
		(*it)->draw(Cam);
	}
}
