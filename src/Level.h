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
    Level( GLuint SkyboxTexID);
    virtual ~Level();
    bool loadLevel(float dimX, float dimY,float segments );
    void update(float dtime);
    virtual void draw(const BaseCamera& Cam);
    virtual void drawOutlines(const BaseCamera& Cam, int pass);
    int isWall(float row, float col);
    Face* activeFace;
    Face* forwardFacingFace;
    float size;
    void consumeDot(Vector pos, int &score);
    bool checkGhostCollision(Vector pos);
    FacesList Faces;
    void tellFaceLocation(Vector pos);
protected:
   
    BaseModel* pModel;
    ModelList Models;
    GLuint SkyboxTexID;

};

#endif /* Level_hpp */
