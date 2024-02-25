#pragma once
#include "BaseShader.h"
#include "Texture.h"
class WallShader : public BaseShader
{
public:
    WallShader(bool LoadStaticShaderCode = true);

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
    GLint RoughnessTexLoc;
    GLint BasecolorTexLoc;
    GLint AmbientOcclusionTextureLoc;
    const Texture* roughnessTex;
    const Texture* basecolorTex;
    const Texture* ambientOclusionTex;
};



