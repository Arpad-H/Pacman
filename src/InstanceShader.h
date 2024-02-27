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
    void updateInstanceData(const std::vector<Offset> data);
    std::vector<Offset> getInstanceData() { return InstancePosData; }
    virtual void activate(const BaseCamera& Cam) const;
    void setEnvioromentCube(GLuint cubemapTexture);
    void setScale(Vector scale) { this->scale = scale; }
    void setColOverride(int col) { colOverride = col; }
protected:
    Vector scale = Vector(1,1,1);
    void assignLocations();

private:
   
    std::vector<Offset> InstancePosData;
    GLint ViewProjLoc;
    GLint ModelMatLoc;
    GLint CameraPosLoc;
    GLint InstancePositionLoc;
    GLint InstanceRotationLoc;
    GLuint ssbo;
    GLint EnviromentCubeLoc;
    GLint ColOverrideLoc;
    GLint CubemapTex = -1;
    int colOverride = 0;
};




