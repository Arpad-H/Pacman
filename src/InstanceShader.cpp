#include "InstanceShader.h"
#include <vector>
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
   
   // GLuint p= ShaderProgram;
   // std::cout << "InstanceShader:" << p << std::endl;
   
}
void InstanceShader::setInstanceData( std::vector<Offset> data)
{
    InstancePosData = data;
    /*for (const auto& offset : data) {
        this->InstancePosData.push_back({ offset.X, offset.Y, offset.Z });
    }*/
    int instances = data.size();
    Vector v = Vector(0, 0, 0);
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, InstancePosData.size() * sizeof(float) * 4, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo); 
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, instances * sizeof(float) * 4, InstancePosData.data()); // Update buffer 
    GLenum Error = glGetError();
    std::cout << "Error: " << Error << std::endl;
}
InstanceShader::~InstanceShader()
{
	glDeleteBuffers(1, &ssbo);
}

void InstanceShader::assignLocations()
{
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
    //InstancePositionLoc = glGetAttribLocation(ShaderProgram, "InstancePosition");
    //InstanceRotationLoc = glGetAttribLocation(ShaderProgram, "InstanceRotation");
    //CameraPosLoc = glGetUniformLocation(ShaderProgram, "CameraPos");
}

void InstanceShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);
    Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();

    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
    
    
}


