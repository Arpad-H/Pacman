#pragma once
#include "BaseShader.h"

class EnviromentReflectionShader : public BaseShader
{
    EnviromentReflectionShader(bool LoadStaticShaderCode = true);
    virtual void activate(const BaseCamera& Cam) const;
    void setEnvioromentCube(GLuint cubemapTexture);
protected:

    void assignLocations();

private:
    GLint CameraPosLoc;
    GLint EnviromentCubeLoc;
    GLint CubemapTex;
    GLint ModelViewProjLoc;
    GLint ModelMatLoc;
    
};

