#pragma once
#pragma once
#include "BaseShader.h"
#include <vector>
struct Offset {
    float x;
    float y;
    float z;
    float w;
};
class InstanceShader : public BaseShader
{
public:
    
    InstanceShader(bool LoadStaticShaderCode = true);
    InstanceShader() {}
    virtual ~InstanceShader();
    void setInstanceData(const std::vector<Offset> data);
    virtual void activate(const BaseCamera& Cam) const;
   
protected:

    void assignLocations();

private:
   
    std::vector<Offset> InstancePosData;
    GLint ModelViewProjLoc;
    GLint ModelMatLoc;
    GLint CameraPosLoc;
    GLint InstancePositionLoc;
    GLint InstanceRotationLoc;
    GLuint ssbo;
};




