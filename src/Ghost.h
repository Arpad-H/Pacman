#ifndef Ghost_hpp
#define Ghost_hpp

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "GameObject.h"

class Ghost : public GameObject
{
public:
	Ghost(const char* ModelFilePath,bool FitSize, Vector initSale );
    virtual ~Ghost();
	void update(float dtime);
	void draw(const BaseCamera& Cam);
protected:
	Model* ghostModel;
};

#endif  /* Ghost_hpp */


