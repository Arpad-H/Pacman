#ifndef Face_hpp
#define Face_hpp

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "Model.h"
#include <list>
#include "PhongShader.h"
#include "Maze.h"


class Face : public BaseModel

{
	public:
	Face(float dimmensions,Matrix m);
	virtual ~Face();
	
	BaseModel* faceModel;
	typedef std::list<BaseModel*> ModelList;
	//typedef std::list<Face*> FacesList;
	void update(float dtime);
	virtual void draw(const BaseCamera& Cam);
	float dimmensions;
	Face* neighbouringFaces[4];
	Maze* layout;
	void setNeighbouringFaces(Face* f1, Face* f2, Face* f3, Face* f4);
	ModelList DotModels;
protected:
	Matrix buildM;
	ModelList WallModels;
	ModelList GhostModels;
	void addWalls();
	//FacesList neighbouringFaces;
	
};


#endif  /* Face_hpp */