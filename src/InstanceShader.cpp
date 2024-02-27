#include "InstanceShader.h"
#include <vector>
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

//shader for instanced rendering
//uses a shader storage buffer object to store instance data

InstanceShader::InstanceShader(bool LoadStaticShaderCode)
{
   
    if (!LoadStaticShaderCode)
        return;
    load(ASSET_DIRECTORY "shaders/instanced.vert", ASSET_DIRECTORY "shaders/instanced.frag");
    assignLocations();   
   // GLuint p= ShaderProgram;
   // std::cout << "InstanceShader:" << p << std::endl;
   
}

void InstanceShader::setInstanceData( std::vector<Offset> data)
{
    InstancePosData = data;
    int instances = data.size();
    Vector v = Vector(0, 0, 0);
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, InstancePosData.size() * sizeof(float) * 4, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo); 
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, instances * sizeof(float) * 4, InstancePosData.data()); // initial buffer data on level creation
    GLenum Error = glGetError();
    std::cout << "Error: " << Error << std::endl;
    std::cout << "SSBO: " << ssbo << std::endl;
}
void InstanceShader::updateInstanceData(const std::vector<Offset> data)
{
    InstancePosData = data;
    int instances = data.size();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, instances * sizeof(float) * 4, InstancePosData.data()); // Update buffer (when orbs get consumed)
}
InstanceShader::~InstanceShader()
{
	glDeleteBuffers(1, &ssbo);
}

void InstanceShader::assignLocations()
{
    ViewProjLoc = glGetUniformLocation(ShaderProgram, "ViewProjMat");
    ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
    EnviromentCubeLoc = glGetUniformLocation(ShaderProgram, "EnviromentCube");
    CameraPosLoc = glGetUniformLocation(ShaderProgram, "CameraPos");
     ColOverrideLoc = glGetUniformLocation(ShaderProgram, "override");
}

void InstanceShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);
    Matrix cam = Cam.getViewMatrix();
    cam.invert();
    Vector camPos = cam.translation();
    glUniform3f(CameraPosLoc, camPos.X, camPos.Y, camPos.Z);
    Matrix ViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix();
    glUniformMatrix4fv(ViewProjLoc, 1, GL_FALSE, ViewProj.m);

    Matrix m = modelTransform();
    Matrix scale;
    scale.scale(this->scale);
    m = m * scale;

    glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, m);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
    
    if (CubemapTex != -1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTex);
        glUniform1i(EnviromentCubeLoc, 0);
    }
    glUniform1i(ColOverrideLoc, colOverride);
    
}
void InstanceShader::setEnvioromentCube(GLuint cubemapTexture)
{
    CubemapTex = cubemapTexture;
}


