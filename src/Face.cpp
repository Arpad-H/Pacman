#include "Face.h"
#include "Maze.h"
#include "TrianglePlaneModel.h"
#include "Ghost.h"
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
	addGhosts(2);
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

pair<float, float> Face::determineActiveAxes() const
{
	Vector forwardVec = this->buildM.forward();
	Vector upVec = this->buildM.up();
	Vector rightVec = this->buildM.right();

	// Determine the dominant global axis for each direction vector
	float dominantAxisForward = dominantAxis(forwardVec);
	float dominantAxisUp = dominantAxis(upVec);
	float dominantAxisRight = dominantAxis(rightVec);

	// the active axes are the ones not aligned with the forward vector
	array<float, 3> allAxes = { 0.f,1.f,2.f }; // x,y,z
	array<float, 2> activeAxes;

	auto it = copy_if(allAxes.begin(), allAxes.end(), activeAxes.begin(),
		[dominantAxisForward](float axis) { return axis != dominantAxisForward; });

	// Return the two active axes
	return make_pair(activeAxes[0], activeAxes[1]);
}

Vector Face::getInitGhostPosition() const
{
	vector<Vector> emptySpaces;
	for (int i = 0; i < layout->getSize(); ++i) {
		for (int j = 0; j < layout->getSize(); ++j) {
			if (!layout->maze[i][j].isWall) {
				// Calculate the 3D position based on face orientation and maze position
				float localX = -dimmensions / 2 + j + 0.5f; // Adjusted for maze cell size
				float localZ = -dimmensions / 2 + i + 0.5f; 

				// Calculate localY based on the face's orientation
				float localY = calculateYBasedOnOrientation();

				// Transform the local position to world space
				Vector localPos(localX, localY, localZ);
				Vector worldPos = buildM * localPos;

				//return worldPos;
				emptySpaces.push_back(worldPos);
			}
		}
	}

	// so that we have a random spot each time we try to spawn a ghost
	if (!emptySpaces.empty()) {
		srand(static_cast<unsigned int>(time(nullptr)));
		int randomIndex = rand() % emptySpaces.size();
		return emptySpaces[randomIndex];
	}

	// If no empty space is found, return a default position
	return Vector(0, 0, 0);
}

float Face::dominantAxis(const Vector& vec) const
{
	float maxComponent = max({ abs(vec.X), abs(vec.Y), abs(vec.Z) });
	if (maxComponent == abs(vec.X)) return 0.f; // X-axis
	if (maxComponent == abs(vec.Y)) return 1.f; // Y-axis
	return 2.f; // Z-axis
}

float Face::calculateYBasedOnOrientation() const
{
	// The Y position calculation depends on the face orientation
	// Tthe face's up vector points in the global Y direction when the face is "front" or "back"

	Vector up = buildM.up(); // Get the up vector of the face
	Vector forward = buildM.forward(); // Get the forward vector of the face

	// Determine the dominant direction of the forward vector to infer face orientation
	float maxComponent = max({ abs(forward.X), abs(forward.Y), abs(forward.Z) });

	if (maxComponent == abs(forward.Y)) {
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
	
	for (int i = 0; i < dimmensions; i++) {
		for (int j = 0; j < dimmensions; j++) {
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

void Face::addGhosts(int amount)
{
	// eine For Loop gibt uns iwie die selbe Position immer und die Geister sind nicht mal sichtbar ?? I really dont know
	//create ghosts
	Ghost* temp = new Ghost(ASSET_DIRECTORY "Pinky.dae", true, Vector(1, 1, 1), 1, this);
	GameObjects.push_back(temp);
	GhostModels.push_back(temp);
}



void Face::update(float dtime)
{
	for (GameObjectList::iterator it = GameObjects.begin(); it != GameObjects.end(); it++) {
		(*it)->update(dtime);
	}
}

void Face::draw(const BaseCamera& Cam)
{
	for (ModelList::iterator it = WallModels.begin(); it != WallModels.end(); it++) {
		(*it)->draw(Cam);
	}
	for (ModelList::iterator it = DotModels.begin(); it != DotModels.end(); it++) {
		//(*it)->draw(Cam);
	}
	for (ModelList::iterator it = GhostModels.begin(); it != GhostModels.end(); it++) {
		(*it)->draw(Cam);
	}
	faceModel->draw(Cam);
}
