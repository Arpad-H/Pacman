#ifndef Level_hpp
#define Level_hpp

#include <stdio.h>
#include "model.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <list>
#include "Face.h"
#include "vector.h"

class Level : public BaseModel
{
public:
    typedef std::list<BaseModel*> ModelList;
    typedef std::list<Face*> FacesList;
    Level();
    virtual ~Level();
    bool loadLevel(float dimX, float dimY,float segments );
    void update(float dtime);
    virtual void draw(const BaseCamera& Cam);
    int isWall(Vector pos);
    Face* activeFace;
    Face* forwardFacingFace;
    float size;
    void consumeDot(Vector pos);
    FacesList Faces;
protected:
   
    BaseModel* pModel;
    ModelList Models;
 

};

#endif /* Level_hpp */
