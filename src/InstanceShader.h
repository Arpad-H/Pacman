#pragma once
#pragma once
#include "BaseShader.h"
#include <vector>
class InstanceShader : public BaseShader
{
public:
    InstanceShader(bool LoadStaticShaderCode = true);
    InstanceShader() {}
    virtual ~InstanceShader();
    void setInstanceData(const std::vector<Vector> data);
    virtual void activate(const BaseCamera& Cam) const;
   
protected:

    void assignLocations();

private:
    std::vector<Vector> InstancePosData;
    GLint ModelViewProjLoc;
    GLint ModelMatLoc;
    GLint CameraPosLoc;
    GLint InstancePositionLoc;
    GLint InstanceRotationLoc;
    GLuint ssbo;
};




