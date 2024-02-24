#pragma once
#include "Camera.h"
#include "BaseShader.h"
class Skybox
{
public:
	Skybox();
	void draw(const BaseCamera& Cam);
private:
	void setupSkybox();
	BaseShader* pSkyboxShader;
	unsigned int cubemapTexture;
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
};

