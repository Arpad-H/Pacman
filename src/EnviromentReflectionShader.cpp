#include "EnviromentReflectionShader.h"

#include "Texture.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

EnviromentReflectionShader::EnviromentReflectionShader(bool LoadStaticShaderCode)
{
    if (!LoadStaticShaderCode)
        return;
    load(ASSET_DIRECTORY "wallShader.vert", ASSET_DIRECTORY "wallShader.frag");
    assignLocations();
    
}

void EnviromentReflectionShader::assignLocations()
{
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
    EnviromentCubeLoc = glGetUniformLocation(ShaderProgram, "EnviromentCube");
    CameraPosLoc = glGetUniformLocation(ShaderProgram, "CameraPos");
   
}

void EnviromentReflectionShader::activate(const BaseCamera& Cam) const
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


   

}

void EnviromentReflectionShader::setEnvioromentCube(GLuint cubemapTexture)
{
    CubemapTex = cubemapTexture;
}