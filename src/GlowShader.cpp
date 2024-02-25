
#include "GlowShader.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
GlowShader::GlowShader(bool LoadStaticShaderCode)
{
    if (!LoadStaticShaderCode)
        return;
    load(ASSET_DIRECTORY "glowshader.vert", ASSET_DIRECTORY "glowshader.frag");
    assignLocations();
}
void GlowShader::assignLocations()
{
  //  CameraPosLoc = glGetUniformLocation(ShaderProgram, "CameraPos");
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
    DiffuseTexLoc = glGetUniformLocation(ShaderProgram, "diffuseTex");
}
void GlowShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);
    //glUseProgram(ShaderProgram);
    Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
   
    glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
  //  glActiveTexture(GL_TEXTURE0);
   // glBindTexture(GL_TEXTURE_2D, DiffuseTexture->getTextureID());
    //glUniform1i(DiffuseTexLoc, 0);
}

void GlowShader::setDiffuseTexture(const Texture* tex)
{
    DiffuseTexture = tex;
}




