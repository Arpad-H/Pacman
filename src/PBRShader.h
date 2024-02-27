#pragma once
#include "BaseShader.h"
#include "Texture.h"
class PBRShader : public BaseShader
{
public:
    PBRShader(bool LoadStaticShaderCode = true, Vector lightPos = Vector(0,32,0));

    virtual void activate(const BaseCamera& Cam) const;
    void setEnvioromentCube(GLuint cubemapTexture);
protected:

    void assignLocations();

private:
    Vector LightPos;
    GLint CameraPosLoc;
    GLint EnviromentCubeLoc;
    GLint CubemapTex;
    GLint ModelViewProjLoc;
    GLint ModelMatLoc;
    GLint RoughnessTexLoc;
    GLint BasecolorTexLoc;
    GLint AmbientOcclusionTextureLoc;
    GLint NormalTextureLoc;
    GLint MetallicTextureLoc;
    GLint LightPosLoc;
    GLint TilingFactorLoc;
    GLint EnviromentEffectLoc;

    const Texture* roughnessTex;
    const Texture* basecolorTex;
    const Texture* ambientOclusionTex;
    const Texture* normalTex;
    const Texture* metallicTex;
};



