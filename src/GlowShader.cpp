

#include "GlowShader.h"



const char* vsCode =
"#version 400\n"
"layout(location=0) in vec4 VertexPos;"
"layout(location=1) in vec4 VertexNormal;"
"layout(location=2) in vec2 VertexTexcoord;"
"out vec3 Position;"
"out vec3 Normal;"
"out vec2 Texcoord;"
"uniform mat4 ModelMat;"
"uniform mat4 ModelViewProjMat;"
"void main()"
"{"
"    Position = (ModelMat * VertexPos).xyz;"
"    Normal =  (ModelMat * VertexNormal).xyz;"
"    Texcoord = VertexTexcoord;"
"    gl_Position = ModelViewProjMat * VertexPos;"
"}";


const char* fsCode =
"#version 400\n"
"uniform vec3 EyePos;"
"uniform vec3 LightPos;"
"uniform vec3 LightColor;"
"uniform vec3 DiffuseColor;"
"uniform vec3 SpecularColor;"
"uniform vec3 AmbientColor;"
"uniform float SpecularExp;"
"uniform sampler2D DiffuseTexture;"
"in vec3 Position;"
"in vec3 Normal;"
"in vec2 Texcoord;"
"out vec4 FragColor;"
"float sat( in float a)"
"{"
"    return clamp(a, 0.0, 1.0);"
"}"
"void main()"
"{"
"    vec4 DiffTex = texture( DiffuseTexture, Texcoord);"
"    if(DiffTex.a <0.3f) discard;"
"    vec3 N = normalize(Normal);"
"    vec3 L = normalize(LightPos-Position);"
"    vec3 E = normalize(EyePos-Position);"
"    vec3 R = reflect(-L,N);"
"    vec3 DiffuseComponent = LightColor * DiffuseColor * sat(dot(N,L));"
"    vec3 SpecularComponent = LightColor * SpecularColor * pow( sat(dot(R,E)), SpecularExp);"
"    FragColor = vec4((DiffuseComponent + AmbientColor)*DiffTex.rgb + SpecularComponent ,DiffTex.a);"
"}";

GlowShader::GlowShader(bool LoadStaticShaderCode) :
    DiffuseColor(0.8f, 0.8f, 0.8f),
    SpecularColor(0.5f, 0.5f, 0.5f),
    AmbientColor(0.2f, 0.2f, 0.2f),
    SpecularExp(20.0f),
    LightPos(20.0f, 20.0f, 20.0f),
    LightColor(1, 1, 1),
    DiffuseTexture(Texture::defaultTex()),
    UpdateState(0xFFFFFFFF)
{
    if (!LoadStaticShaderCode)
        return;
    ShaderProgram = createShaderProgram(vsCode, fsCode);
    assignLocations();
}
void GlowShader::assignLocations()
{
    DiffuseColorLoc = glGetUniformLocation(ShaderProgram, "DiffuseColor");
    AmbientColorLoc = glGetUniformLocation(ShaderProgram, "AmbientColor");
    SpecularColorLoc = glGetUniformLocation(ShaderProgram, "SpecularColor");
    SpecularExpLoc = glGetUniformLocation(ShaderProgram, "SpecularExp");
    DiffuseTexLoc = glGetUniformLocation(ShaderProgram, "DiffuseTexture");
    LightPosLoc = glGetUniformLocation(ShaderProgram, "LightPos");
    LightColorLoc = glGetUniformLocation(ShaderProgram, "LightColor");
    EyePosLoc = glGetUniformLocation(ShaderProgram, "EyePos");
    ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
    ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
}
void GlowShader::activate(const BaseCamera& Cam) const
{
    BaseShader::activate(Cam);

   
 // Set shader uniforms for material properties
    glUniform3f(DiffuseColorLoc, DiffuseColor.R, DiffuseColor.G, DiffuseColor.B);
    glUniform3f(AmbientColorLoc, AmbientColor.R, AmbientColor.G, AmbientColor.B);
    glUniform3f(SpecularColorLoc, SpecularColor.R, SpecularColor.G, SpecularColor.B);
    glUniform1f(SpecularExpLoc, SpecularExp);

    // Activate the diffuse texture
    DiffuseTexture->activate(0);
    if (UpdateState & DIFF_TEX_CHANGED && DiffuseTexture)
        glUniform1i(DiffuseTexLoc, 0);

    // Set shader uniforms for light properties
    glUniform3f(LightColorLoc, LightColor.R, LightColor.G, LightColor.B);
    if (UpdateState & LIGHT_POS_CHANGED)
        glUniform3f(LightPosLoc, LightPos.X, LightPos.Y, LightPos.Z);

    // Always update matrices
    Matrix ModelViewProj = Cam.getProjectionMatrix() * Cam.getViewMatrix() * modelTransform();
    glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
    glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);

    // Set eye position
    Vector EyePos = Cam.position();
    glUniform3f(EyePosLoc, EyePos.X, EyePos.Y, EyePos.Z);

    // Update the state to indicate that all modified parameters have been synchronized with the GPU
    UpdateState = 0x0;
}


void GlowShader::lightPos(const Vector& pos)
{
    LightPos = pos;
    UpdateState |= LIGHT_POS_CHANGED;
}


void GlowShader::diffuseTexture(const Texture* pTex)
{
    DiffuseTexture = pTex;
    if (!DiffuseTexture)
        DiffuseTexture = Texture::defaultTex();

    UpdateState |= DIFF_TEX_CHANGED;
}





