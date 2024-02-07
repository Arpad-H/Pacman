#ifndef Pacman_hpp
#define Pacman_hpp

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "GameObject.h"

class Pacman : public GameObject
{
public:
	Pacman(const char* ModelFilePath, bool FitSize, Vector initSale);
	virtual ~Pacman();
	void steer(float dir);
	void update(float dtime);
	void draw(const BaseCamera& Cam);
	Model* pacmanModel;
protected:
	float speed;
	
	float dir;
};

#endif  /* Pacman_hpp */