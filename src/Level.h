#ifndef Level_hpp
#define Level_hpp

#include <stdio.h>
#include "model.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <list>

class Level : public BaseModel
{
public:
    typedef std::list<BaseModel*> ModelList;
    Level();
    virtual ~Level();
    bool loadLevel(float dimX, float dimY,float segments );
    void update(float dtime);
    virtual void draw(const BaseCamera& Cam);
protected:
    BaseModel* pModel;
    ModelList Models;
};

#endif /* Level_hpp */
