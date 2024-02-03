#ifndef Level_hpp
#define Level_hpp

#include <stdio.h>
#include "model.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

class Level : public BaseModel
{
public:
    Level();
    virtual ~Level();
    bool loadLevel();
    void update(float dtime);
    virtual void draw(const BaseCamera& Cam);
protected:
    Model* pModel;
};

#endif /* Levelk_hpp */
