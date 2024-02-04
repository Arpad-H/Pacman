#include "Level.h"
#include "model.h"
#include "phongshader.h"
#include "lineplanemodel.h"
#include "constantshader.h"
static float toRad(float deg) { return deg * M_PI / 180.0f; }
Level::Level()
{
	pModel = new BaseModel();
}
Level::~Level()
{
	delete pModel;
}
bool Level::loadLevel(float dimX, float dimY, float segments)
{
	//PhongShader* pPhongShader = new PhongShader();
	ConstantShader* pConstShader;
	ConstantShader* pConstShader2;
	ConstantShader* pConstShader3;
	ConstantShader* pConstShader4;
	ConstantShader* pConstShader5;
	ConstantShader* pConstShader6;

		//bottom
		Matrix m,t,f;
		pModel = new LinePlaneModel(dimX, dimY, segments, segments);
		pConstShader = new ConstantShader();
		pConstShader->color(Color(1, 1, 1));
		pModel->shader(pConstShader, true);
		pModel->transform(m.translation(0, -dimY/2, 0));
		Models.push_back(pModel);

		//top
		pModel = new LinePlaneModel(dimX, dimY, segments, segments);
		pConstShader2 = new ConstantShader();
		pConstShader2->color(Color(1, 0, 0));
		pModel->shader(pConstShader2, true);
		pModel->transform(m.translation(0, dimY/2, 0));
		Models.push_back(pModel);

		//left
		pModel = new LinePlaneModel(dimY, dimX, segments, segments);
		pConstShader3 = new ConstantShader();
		pConstShader3->color(Color(0, 1, 0));
		pModel->shader(pConstShader3, true);
		m.rotationZ(toRad(90));
		t.translation(0, dimX / 2, 0);
		f = m*t;
		pModel->transform(f);
		Models.push_back(pModel);

		//right
		pModel = new LinePlaneModel(dimY, dimX, segments, segments);
		pConstShader4 = new ConstantShader();
		pConstShader4->color(Color(0, 0, 1));
		pModel->shader(pConstShader4, true);
		m.rotationZ(toRad(90));
		t.translation(0, -dimX / 2, 0);
		f = m * t;
		pModel->transform(f);
		Models.push_back(pModel);

		//front
		pModel = new LinePlaneModel(dimY, dimX, segments, segments);
		pConstShader5 = new ConstantShader();
		pConstShader5->color(Color(1, 1, 0));
		pModel->shader(pConstShader5, true);
		
		m.rotationYawPitchRoll(0, toRad(90), 0);
		t.translation(0, 0, dimX/2);
		f =  t*m;
		pModel->transform(f);
		Models.push_back(pModel);

		//back
		pModel = new LinePlaneModel(dimY, dimX, segments, segments);
		pConstShader6 = new ConstantShader();
		pConstShader6->color(Color(0, 1, 1));
		pModel->shader(pConstShader6, true);

		m.rotationYawPitchRoll(0, toRad(90), 0);
		t.translation(0, 0, -dimX / 2);
		f = t * m;
		pModel->transform(f);
		Models.push_back(pModel);
		
		
	return true;
}
void Level::update(float dtime)
{

}
void Level::draw(const BaseCamera& Cam)
{
	for (ModelList::iterator it = Models.begin(); it != Models.end(); it++) {
		(*it)->draw(Cam);
	}
		
}

