#include "WallShader.h"
#include "Texture.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

WallShader::WallShader(bool LoadStaticShaderCode)
{
    if (!LoadStaticShaderCode)
        return;
    load(ASSET_DIRECTORY "wallShader.vert", ASSET_DIRECTORY "wallShader.frag");
    assignLocations();
    roughnessTex = Texture::LoadShared(ASSET_DIRECTORY "Metal_roughness.jpg");
    basecolorTex = Texture::LoadShared(ASSET_DIRECTORY "Metal_basecolor.jpg");
    ambientOclusionTex = Texture::LoadShared(ASSET_DIRECTORY "Metal_ambientOcclusion.jpg");
}

void WallShader::assignLocations()
{  
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
    EnviromentCubeLoc = glGetUniformLocation(ShaderProgram, "EnviromentCube");
    CameraPosLoc = glGetUniformLocation(ShaderProgram, "CameraPos");
    RoughnessTexLoc = glGetUniformLocation(ShaderProgram, "RoughnessTexture");
    BasecolorTexLoc = glGetUniformLocation(ShaderProgram, "BaseColorTexture");
    AmbientOcclusionTextureLoc = glGetUniformLocation(ShaderProgram, "AmbientOcclusionTexture");
}

void WallShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);
  //  glUseProgram(ShaderProgram);
    Matrix cam = Cam.getViewMatrix();
    cam.invert();
    Vector camPos = cam.translation();
    glUniform3f(CameraPosLoc, camPos.X, camPos.Y, camPos.Z);
    Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
   
    glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
  
    //Bind cubemap texture for the environment
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTex);
    glUniform1i(EnviromentCubeLoc, 0);
    
    // Bind base color texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, basecolorTex->getTextureID());
    glUniform1i(BasecolorTexLoc, 1);

    // Bind roughness texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, roughnessTex->getTextureID());
    glUniform1i(RoughnessTexLoc, 2);
    
    //Bind ambient occlusion texture
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ambientOclusionTex->getTextureID());
    glUniform1i(AmbientOcclusionTextureLoc, 3);
   
}

void WallShader::setEnvioromentCube(GLuint cubemapTexture)
{
    CubemapTex = cubemapTexture;
}
