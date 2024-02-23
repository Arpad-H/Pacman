#include "Face.h"
#include "Maze.h"
#include "TrianglePlaneModel.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
Face::Face(float dimmensions, Matrix m)
{
	buildM = m;
	this->dimmensions = dimmensions;
	addWalls();
	//faceModel = new BaseModel();
	ConstantShader* pConstShader;
	pConstShader = new ConstantShader();
	pConstShader->color(Color(0.5, 0.5, 0.5)+Color(m.up().X, m.up().Y, m.up().Z));
	faceModel = new TrianglePlaneModel(dimmensions, dimmensions, 1, 1);
	faceModel->shader(pConstShader, true);
	faceModel->transform(buildM);
	
}

Face::~Face()
{
}

void Face::setNeighbouringFaces(Face* f1, Face* f2, Face* f3, Face* f4)
{
	neighbouringFaces[0] = f1;
	neighbouringFaces[1] = f2;
	neighbouringFaces[2] = f3;
	neighbouringFaces[3] = f4;
}

pair<int, int> Face::determineActiveAxes() const
{
	Vector forwardVec = this->buildM.forward();
	Vector upVec = this->buildM.up();
	Vector rightVec = this->buildM.right();

	// Determine the dominant global axis for each direction vector
	int dominantAxisForward = dominantAxis(forwardVec);
	int dominantAxisUp = dominantAxis(upVec);
	int dominantAxisRight = dominantAxis(rightVec);

	// the active axes are the ones not aligned with the forward vector
	array<int, 3> allAxes = { 0,1,2 }; // x,y,z
	array<int, 2> activeAxes;

	auto it = copy_if(allAxes.begin(), allAxes.end(), activeAxes.begin(),
		[dominantAxisForward](int axis) { return axis != dominantAxisForward; });

	// Return the two active axes
	return make_pair(activeAxes[0], activeAxes[1]);
}

Vector Face::getInitGhostPosition() const
{
	for (int i = 0; i < layout->getSize(); ++i) {
		for (int j = 0; j < layout->getSize(); ++j) {
			if (!layout->maze[i][j].isWall) {
				// Calculate the 3D position based on face orientation and maze position
				float localX = -dimmensions / 2 + j + 0.5f; // Adjust for maze cell size if necessary
				float localZ = -dimmensions / 2 + i + 0.5f; // Adjust for maze cell size if necessary

				// Calculate localY based on the face's orientation
				float localY = calculateYBasedOnOrientation();

				// Transform the local position to world space
				Vector localPos(localX, localY, localZ);
				Vector worldPos = buildM * localPos;

				return worldPos;
			}
		}
	}

	// If no empty space is found, return a default position
	return Vector(0, 0, 0);
}

int Face::dominantAxis(const Vector& vec) const
{
	float maxComponent = max({ abs(vec.X), abs(vec.Y), abs(vec.Z) });
	if (maxComponent == abs(vec.X)) return 0; // X-axis
	if (maxComponent == abs(vec.Y)) return 1; // Y-axis
	return 2; // Z-axis
}

float Face::calculateYBasedOnOrientation() const
{
	// The Y position calculation now depends on the face orientation
	// This function assumes the face's up vector points in the global Y direction when the face is "front" or "back"

	Vector up = buildM.up(); // Get the up vector of the face
	Vector forward = buildM.forward(); // Get the forward vector of the face

	// Determine the dominant direction of the forward vector to infer face orientation
	float maxComponent = std::max({ std::abs(forward.X), std::abs(forward.Y), std::abs(forward.Z) });

	if (maxComponent == std::abs(forward.Y)) {
		// Top or bottom face, use the up vector's direction to adjust Y
		return (forward.Y > 0 ? -0.5f : 0.5f) * dimmensions;
	}
	else {
		// For side faces, we consider them lying on the XZ plane, so their Y is determined by the up vector
		return up.Y > 0 ? 0.5f : -0.5f;
	}
}

void Face::addWalls()
{
	PhongShader* pPhongshader = new PhongShader();
	//Maze* maze;
	layout = new Maze(dimmensions);
	layout->display();
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	//layout = maze;
	Model* pWall;
	Matrix m,f;
	
	for (int i = 0; i < dimmensions; ++i) {
		for (int j = 0; j < dimmensions; ++j) {
			if (layout->maze[i][j].isWall) {
				pWall = new Model(ASSET_DIRECTORY "cube.dae", true, Vector(1, 1, 1));
				pWall->shader(pPhongshader, true);
				f = m.translation(-dimmensions / 2 + j + 0.5, 0.5, -dimmensions / 2 + i + 0.5) * pWall->transform();
				pWall->transform(buildM * f);
				WallModels.push_back(pWall);
			}
			/*else {
				pWall = new Model(ASSET_DIRECTORY "orbdae.dae", true, Vector(0.2, 0.2, 0.2));
				pWall->shader(pPhongshader, true);
				f = m.translation(-dimmensions / 2 + j + 0.5, 0.5, -dimmensions / 2 + i + 0.5) * pWall->transform();
				pWall->transform(buildM * f);
				DotModels.push_back(pWall);
			}*/
			
		}
	}
	
}



void Face::update(float dtime)
{
}

void Face::draw(const BaseCamera& Cam)
{
	for (ModelList::iterator it = WallModels.begin(); it != WallModels.end(); it++) {
		(*it)->draw(Cam);
	}
	for (ModelList::iterator it = DotModels.begin(); it != DotModels.end(); it++) {
		//(*it)->draw(Cam);
	}
	faceModel->draw(Cam);
}
