#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include "model.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "BaseModel.h"

class GameObject : public BaseModel
{
public:
    GameObject();
    virtual ~GameObject();
    bool loadModels(const char* ModelFilePath,bool Fitsize, Vector initScale,Model& model);
    virtual void update(float dtime);
    virtual void draw(const BaseCamera& Cam);
    Matrix initTransform;
};

#endif /* GameObject_hpp */
