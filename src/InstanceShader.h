#pragma once
#pragma once
#include "BaseShader.h"

class InstanceShader : public BaseShader
{
public:
    InstanceShader(bool LoadStaticShaderCode = true);

    virtual void activate(const BaseCamera& Cam) const;
   
protected:

    void assignLocations();

private:
    GLint ModelViewProjLoc;
    GLint ModelMatLoc;
    GLint CameraPosLoc;
};




