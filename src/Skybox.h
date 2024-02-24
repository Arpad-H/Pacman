#pragma once
#include "Camera.h"
#include "BaseShader.h"
#include "TriangleBoxModel.h"
class Skybox : public TriangleBoxModel
{
public:
	Skybox();
	void draw(const BaseCamera& Cam);
private:
	void setupSkybox();
	BaseShader* pSkyboxShader;
	unsigned int cubemapTexture;
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	TriangleBoxModel* pSkyboxModel;
};

