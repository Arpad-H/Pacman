#ifndef Pacman_hpp
#define Pacman_hpp

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "GameObject.h"
#include "Level.h"

class Pacman : public GameObject
{
public:
	Pacman(const char* ModelFilePath, bool FitSize, Vector initSale);
	virtual ~Pacman();
	void steer(float dir);
	void update(float dtime);
	void draw(const BaseCamera& Cam);
	Model* pacmanModel;
	void setLevel(Level* level);
	bool transitionState = false;
protected:
	Matrix faceAdaptation;
	void snapToGrid(Vector &pos,Vector posOffset);
	void reajust();
	void transition();
	float speed;
	Level* level;
	float dir;
	Vector test = Vector(0,0,-1);
};

#endif  /* Pacman_hpp */