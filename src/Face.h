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
#include "WallShader.h"
#include "TriangleBoxModel.h"


class Face : public BaseModel

{
	public:
	Face(float dimmensions,Matrix t, Matrix r, GLuint SkyboxTexID);
	virtual ~Face();
	Matrix rotateToMatchFace(Vector objectUp);
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
	ModelList GhostModels;
	bool isWithinBounds(Vector position);

protected:
	
	
	Matrix m_translation;
	Matrix m_rotation;
	ModelList WallModels;
	WallShader* pWallShader;
	void addWalls();
	void initGhosts(int amount);
	// Helper method to find the dominant axis of a vector
	float dominantAxis(const Vector& vec) const;
	GameObjectList GameObjects;
	GLuint SkyboxTexID;
	TriangleBoxModel* pBox;
	std::vector<Vector> InstancePositionData;
};


#endif  /* Face_hpp */