
#ifndef GlowShader_hpp
#define GlowShader_hpp

#include <stdio.h>
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <iostream>
#include <assert.h>
#include "color.h"
#include "vector.h"
#include "matrix.h"
#include "camera.h"
#include "baseshader.h"
#include "texture.h"

class GlowShader : public BaseShader
{
public:
    GlowShader( bool LoadStaticShaderCode = true);
   
    virtual void activate(const BaseCamera& Cam) const;
    void setDiffuseTexture(const Texture* tex);
protected:   
    
    void assignLocations();

private:
    GLint ModelViewProjLoc;
    GLint ModelMatLoc;
    GLint CameraPosLoc;
    GLint DiffuseTexLoc;
    const Texture* DiffuseTexture;
};

#endif /* GlowShader_hpp */
