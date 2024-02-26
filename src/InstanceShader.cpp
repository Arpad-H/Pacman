#include "InstanceShader.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
InstanceShader::InstanceShader(bool LoadStaticShaderCode)
{
    if (!LoadStaticShaderCode)
        return;
    load(ASSET_DIRECTORY "shaders/instanced.vert", ASSET_DIRECTORY "shaders/instanced.frag");
    assignLocations();
   
}

void InstanceShader::assignLocations()
{
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
    CameraPosLoc = glGetUniformLocation(ShaderProgram, "CameraPos");
}

void InstanceShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);
    //  glUseProgram(ShaderProgram);
    Matrix cam = Cam.getViewMatrix();
    cam.invert();
    Vector camPos = cam.translation();
    glUniform3f(CameraPosLoc, camPos.X, camPos.Y, camPos.Z);
    Matrix m = modelTransform();
    Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();

    glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);


}

