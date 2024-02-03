#include "Level.h"
#include "model.h"
#include "phongshader.h"

Level::Level()
{
	pModel = new Model();
}
Level::~Level()
{
	delete pModel;
}
bool Level::loadLevel()
{
	PhongShader* pPhongShader = new PhongShader();
	
	return true;
}
void Level::update(float dtime)
{
}
void Level::draw(const BaseCamera& Cam)
{
	pModel->draw(Cam);
}

