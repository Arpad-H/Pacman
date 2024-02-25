#pragma once
#include "Camera.h"
#include "BaseShader.h"
#include "TriangleBoxModel.h"
class Skybox : public TriangleBoxModel
{
public:
	Skybox();
	void draw(const BaseCamera& Cam);
	GLuint getCubemapTexture() { return cubemapTexture; };
private:
	void setupSkybox();
	BaseShader* pSkyboxShader;
	GLuint  cubemapTexture;
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	TriangleBoxModel* pSkyboxModel;
};

