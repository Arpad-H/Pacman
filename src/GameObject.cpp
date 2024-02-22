
#include "GameObject.h"
#include "phongshader.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#define EPSILON 1e-6
static float toRad(float deg) { return deg * M_PI / 180.0f; }

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

bool GameObject::loadModels(const char* ModelFilePath,bool FitSize, Vector initScale, Model& pModel)
{
    PhongShader* pPhongShader = new PhongShader();
    pModel.load(ModelFilePath, FitSize,initScale);
   // pModel.shader(pPhongShader, true);
    return true;
}

void GameObject::update(float dtime)
{
  
}

void GameObject::draw(const BaseCamera& Cam)
{
  
}
