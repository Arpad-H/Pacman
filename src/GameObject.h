#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include "model.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

class GameObject : public BaseModel
{
public:
    GameObject();
    virtual ~GameObject();
    bool loadModels(const char* ModelFilePath,bool Fitsize = 1, Vector initScale = Vector(1,1,1));
    virtual void update(float dtime);
    virtual void draw(const BaseCamera& Cam);
protected:
    Model* pModel;
};

#endif /* GameObject_hpp */
