#include "PBRShader.h"
#include "Texture.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

// The PBR calculations in the shader code are taken from https://learnopengl.com/PBR/Lighting
//we added some small extra functionality and the enviroment mapping (enviroment sampling got moved to the instace shader to. Functionality is still there tho)
PBRShader::PBRShader(bool LoadStaticShaderCode,Vector lightPos)
{
    LightPos = lightPos;
    if (!LoadStaticShaderCode)
        return;
    load(ASSET_DIRECTORY "pbrShader.vert", ASSET_DIRECTORY "pbrShader.frag");
    assignLocations();
    roughnessTex = Texture::LoadShared(ASSET_DIRECTORY "tex/Sci_fi_Metal_Panel_005_roughness.jpg");
    basecolorTex = Texture::LoadShared(ASSET_DIRECTORY "tex/Sci_fi_Metal_Panel_005_basecolor.jpg");
    ambientOclusionTex = Texture::LoadShared(ASSET_DIRECTORY "tex/Sci_fi_Metal_Panel_005_ambientOcclusion.jpg");
    normalTex = Texture::LoadShared(ASSET_DIRECTORY "tex/Sci_fi_Metal_Panel_005_normal.jpg");
    metallicTex = Texture::LoadShared(ASSET_DIRECTORY "tex/Sci_fi_Metal_Panel_005_metallic.jpg");
}

void PBRShader::assignLocations()
{  
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
    ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
    //EnviromentCubeLoc = glGetUniformLocation(ShaderProgram, "EnviromentCube");
    CameraPosLoc = glGetUniformLocation(ShaderProgram, "CameraPos");
    RoughnessTexLoc = glGetUniformLocation(ShaderProgram, "RoughnessTexture");
    BasecolorTexLoc = glGetUniformLocation(ShaderProgram, "BaseColorTexture");
    AmbientOcclusionTextureLoc = glGetUniformLocation(ShaderProgram, "AmbientOcclusionTexture");
    NormalTextureLoc = glGetUniformLocation(ShaderProgram, "NormalTexture");
    MetallicTextureLoc = glGetUniformLocation(ShaderProgram, "MetallicTexture");
    LightPosLoc = glGetUniformLocation(ShaderProgram, "LightPos");
    TilingFactorLoc = glGetUniformLocation(ShaderProgram, "tilingFactor");
    EnviromentEffectLoc = glGetUniformLocation(ShaderProgram, "enviromentEffect");

    glUseProgram(ShaderProgram);
    glUniform1f(TilingFactorLoc, 6.0f);
    glUniform1f(EnviromentEffectLoc, 0.1);
    glUniform3f(LightPosLoc, LightPos.X, LightPos.Y, LightPos.Z);
    glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);//model mat dosent change since the floors dont move
}

void PBRShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);
  //  glUseProgram(ShaderProgram);
    Matrix cam = Cam.getViewMatrix();
    cam.invert();
    Vector camPos = cam.translation();
    glUniform3f(CameraPosLoc, camPos.X, camPos.Y, camPos.Z);
   
    Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);
   
    
  /*  //Bind cubemap texture for the environment
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, CubemapTex);
    glUniform1i(EnviromentCubeLoc, 0);*/
    
    // Bind base color texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, basecolorTex->getTextureID());
    glUniform1i(BasecolorTexLoc, 0);

    // Bind roughness texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, roughnessTex->getTextureID());
    glUniform1i(RoughnessTexLoc, 1);
    
    //Bind ambient occlusion texture
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ambientOclusionTex->getTextureID());
    glUniform1i(AmbientOcclusionTextureLoc, 2);

    //Bind normal texture
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, normalTex->getTextureID());
    glUniform1i(NormalTextureLoc, 3);

    //Bind metallic texture
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, metallicTex->getTextureID());
    glUniform1i(MetallicTextureLoc, 4);
   
}

void PBRShader::setEnvioromentCube(GLuint cubemapTexture)
{
    CubemapTex = cubemapTexture;
}
