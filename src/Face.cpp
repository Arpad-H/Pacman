#include "Face.h"
#include "Maze.h"
#include "TrianglePlaneModel.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
Face::Face(float dimmensions, Matrix m)
{
	buildM = m;
	this->dimmensions = dimmensions;
	addWalls();
	//faceModel = new BaseModel();
	ConstantShader* pConstShader;
	pConstShader = new ConstantShader();
	pConstShader->color(Color(0.5, 0.5, 0.5)+Color(m.up().X, m.up().Y, m.up().Z));
	faceModel = new TrianglePlaneModel(dimmensions, dimmensions, 1, 1);
	faceModel->shader(pConstShader, true);
	faceModel->transform(buildM);
	
}

Face::~Face()
{
}

void Face::setNeighbouringFaces(Face* f1, Face* f2, Face* f3, Face* f4)
{
	neighbouringFaces[0] = f1;
	neighbouringFaces[1] = f2;
	neighbouringFaces[2] = f3;
	neighbouringFaces[3] = f4;
}

void Face::addWalls()
{
	PhongShader* pPhongshader = new PhongShader();
	//Maze* maze;
	layout = new Maze(dimmensions);
	layout->display();
	//layout = maze;
	Model* pWall;
	Matrix m,f;
	for (int i = 0; i < dimmensions; ++i) {
		for (int j = 0; j < dimmensions; ++j) {
			if (layout->maze[i][j].isWall) {
				pWall = new Model(ASSET_DIRECTORY "cube.dae", true, Vector(1, 1, 1));
				pWall->shader(pPhongshader, true);
				f = m.translation(-dimmensions / 2 + j + 0.5, 0.5, -dimmensions / 2 + i + 0.5) * pWall->transform();
				pWall->transform(buildM * f);
				WallModels.push_back(pWall);
			}
			/*else {
				pWall = new Model(ASSET_DIRECTORY "orbdae.dae", true, Vector(0.2, 0.2, 0.2));
				pWall->shader(pPhongshader, true);
				f = m.translation(-dimmensions / 2 + j + 0.5, 0.5, -dimmensions / 2 + i + 0.5) * pWall->transform();
				pWall->transform(buildM * f);
				DotModels.push_back(pWall);
			}*/
			
		}
	}
	
}

void Face::update(float dtime)
{
}

void Face::draw(const BaseCamera& Cam)
{
	for (ModelList::iterator it = WallModels.begin(); it != WallModels.end(); it++) {
		//(*it)->draw(Cam);
	}
	for (ModelList::iterator it = DotModels.begin(); it != DotModels.end(); it++) {
		//(*it)->draw(Cam);
	}
	faceModel->draw(Cam);
}
