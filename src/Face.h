#ifndef Face_hpp
#define Face_hpp

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "Model.h"
#include <list>
#include "PhongShader.h"
#include "Maze.h"
#include <array>
#include <algorithm>
#include <cmath>
#include "GameObject.h"


class Face : public BaseModel

{
	public:
	Face(float dimmensions,Matrix m);
	virtual ~Face();
	
	BaseModel* faceModel;
	typedef std::list<BaseModel*> ModelList;
	typedef std::list<GameObject*> GameObjectList;
	Vector target = Vector(0, 0, 0);
	//typedef std::list<Face*> FacesList;
	void update(float dtime);
	virtual void draw(const BaseCamera& Cam);
	float dimmensions;
	Face* neighbouringFaces[4];
	Maze* layout;
	void setNeighbouringFaces(Face* f1, Face* f2, Face* f3, Face* f4);
	ModelList DotModels;
	pair<float, float> determineActiveAxes() const;
	Vector getInitGhostPosition() const;
	void setTarget(Vector t);
	Vector getTarget() const;
	bool checkWall(Vector pos);
	vector<Vector> wallPositions;
protected:
	Matrix buildM;
	ModelList WallModels;
	ModelList GhostModels;
	void addWalls();
	void initGhosts(int amount);
	// Helper method to find the dominant axis of a vector
	float dominantAxis(const Vector& vec) const;
	GameObjectList GameObjects;
	
};


#endif  /* Face_hpp */