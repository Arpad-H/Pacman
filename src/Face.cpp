#include "Face.h"
#include "Maze.h"
#include "TrianglePlaneModel.h"
#include "Ghost.h"
#include "WallShader.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

Face::Face(float dimmensions, Matrix m, GLuint SkyboxTexID)
{
	buildM = m;
	this->dimmensions = dimmensions;
	
	//faceModel = new BaseModel();
	ConstantShader* pConstShader;
	pConstShader = new ConstantShader();
	pWallShader = new WallShader();
	pWallShader->setEnvioromentCube(SkyboxTexID);
	addWalls();
	pConstShader->color(Color(0.5, 0.5, 0.5)+Color(m.up().X, m.up().Y, m.up().Z));
	faceModel = new TrianglePlaneModel(dimmensions, dimmensions, 1, 1);
	faceModel->shader(pWallShader, true);
	faceModel->transform(buildM);
	initGhosts(1);
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

// Helper function to convert degrees to radians
inline float degToRad(float deg) {
	return deg * (M_PI / 180.0f);
}

Matrix Face::rotateToMatchFace(Vector objectUp) {
	Vector faceUp = this->buildM.up();
	// Normalize both vectors to ensure proper calculations
	//faceUp.normalize();
	faceUp.toUnitVector();
	objectUp.normalize();
	// Check if the vectors are parallel and pointing in the same direction or opposite directions
	if (faceUp == objectUp) {
		// No rotation needed
		return Matrix().identity();
	}
	else if (faceUp == -objectUp) {
		// 180 degrees rotation around an arbitrary axis perpendicular to faceUp
		// Choosing an axis perpendicular to faceUp or objectUp. For simplicity, if faceUp is not parallel to the x-axis, use the x-axis as a rotation axis
		Vector axis = Vector(1, 0, 0);
		if (faceUp == Vector(1, 0, 0) || faceUp == Vector(-1, 0, 0)) { // If faceUp is parallel to the x-axis, choose the y-axis
			axis = Vector(0, 1, 0);
		}
		return Matrix().rotationAxis(axis, degToRad(180)); // Rotating 180 degrees around the chosen axis
	}
	else {
		// General case: Calculate axis of rotation and angle
		Vector axis = objectUp.cross(faceUp).normalize(); // Axis perpendicular to both vectors
		float angle = acos(objectUp.dot(faceUp)); // Angle between vectors in radians

		// Create rotation matrix around the axis by the calculated angle
		return Matrix().rotationAxis(axis, angle);
	}
}

pair<float, float> Face::determineActiveAxes() const
{
	Vector forwardVec = this->buildM.forward();
	Vector upVec = this->buildM.up();
	Vector rightVec = this->buildM.right();

	// Determine the dominant global axis for each direction vector
	float dominantAxisForward = dominantAxis(forwardVec);
	float dominantAxisUp = dominantAxis(upVec); // Now using the up vector
	float dominantAxisRight = dominantAxis(rightVec);

	// the active axes are the ones not aligned with the UP vector (which represents the normal)
	array<float, 3> allAxes = { 0,1,2 }; // x,y,z
	array<float, 2> activeAxes;

	auto it = copy_if(allAxes.begin(), allAxes.end(), activeAxes.begin(),
		[dominantAxisUp](float axis) { return axis != dominantAxisUp; }); // Compare with dominantAxisUp

	// Return the two active axes
	return make_pair(activeAxes[0], activeAxes[1]);
}

Vector Face::getInitGhostPosition() const
{
	vector<Vector> emptySpaces;

	// Iterate through the maze layout to find empty spaces
	for (int i = 0; i < dimmensions; i++) {
		for (int j = 0; j < dimmensions; j++) {
			if (!layout->maze[i][j].isWall) {
				// Convert the maze grid position to the face's local 3D space
				Vector localPosition(-dimmensions / 2 + j + 0.5, 0.5, -dimmensions / 2 + i + 0.5);

				// Transform the local position to the world position using the face's buildM matrix
				Vector worldPosition = buildM * localPosition;

				// Store the world position of the empty space
				emptySpaces.push_back(worldPosition);
			}
		}
	}

	// Select a random empty space to spawn the ghost
	if (!emptySpaces.empty()) {
		int randomIndex = rand() % emptySpaces.size();
		return emptySpaces[randomIndex];
	}

	// Return a default position if no empty spaces are found (should be handled appropriately)
	return Vector(0, 0, 0); // Adjust this based on your game's requirements
}

void Face::setTarget(Vector t)
{
	target = t;
}

Vector Face::getTarget() const
{
	return target;
}

bool Face::checkWall(Vector pos)
{
	// iterate through wallPositions and check if pos is in the list
	for (int i = 0; i < wallPositions.size(); i++)
	{
		if (pos == wallPositions[i]) return true;
	}
	return false; // Ensure this line is present
}


bool Face::isWithinBounds(Vector position) {
	// Get the "up" vector to determine orientation
	Vector upVec = this->buildM.up();

	// Calculate half dimension for bounds checking
	float halfDim = dimmensions / 2.0f;

	// Initialize withinBounds to false
	bool withinBounds = false;

	// Horizontal Orientation (Floor or Ceiling)
	if (std::abs(upVec.Y) > std::abs(upVec.X) && std::abs(upVec.Y) > std::abs(upVec.Z)) {
		bool withinXBounds = position.X >= -halfDim && position.X <= halfDim;
		bool withinZBounds = position.Z >= -halfDim && position.Z <= halfDim;
		withinBounds = withinXBounds && withinZBounds;
	}
	// Vertical Orientation, Wall facing along the X direction
	else if (std::abs(upVec.X) > std::abs(upVec.Y) && std::abs(upVec.X) > std::abs(upVec.Z)) {
		bool withinYBounds = position.Y >= -halfDim && position.Y <= halfDim;
		bool withinZBounds = position.Z >= -halfDim && position.Z <= halfDim;
		withinBounds = withinYBounds && withinZBounds;
	}
	// Vertical Orientation, Wall facing along the Z direction
	else if (std::abs(upVec.Z) > std::abs(upVec.X) && std::abs(upVec.Z) > std::abs(upVec.Y)) {
		bool withinXBounds = position.X >= -halfDim && position.X <= halfDim;
		bool withinYBounds = position.Y >= -halfDim && position.Y <= halfDim;
		withinBounds = withinXBounds && withinYBounds;
	}

	return withinBounds;
}

float Face::dominantAxis(const Vector& vec) const
{
	float maxComponent = max({ abs(vec.X), abs(vec.Y), abs(vec.Z) });
	if (maxComponent == abs(vec.X)) return 0.f; // X-axis
	if (maxComponent == abs(vec.Y)) return 1.f; // Y-axis
	return 2.f; // Z-axis
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

				// extract the position of the wall
				Vector pos= f.translation();
				wallPositions.push_back(buildM*pos);
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

void Face::initGhosts(int amount)
{
	const char* ghosts[4] =
	{
		ASSET_DIRECTORY "Pinky.dae",
		ASSET_DIRECTORY "Blinky.dae",
		ASSET_DIRECTORY "Inky.dae",
		ASSET_DIRECTORY "Clyde.dae",
	};

	for(int i =0; i< amount; i++)
	{
		int random = rand() % 4;
		Ghost* temp = new Ghost(ghosts[random], true, Vector(5, 5, 5), i, this);
		
		GameObjects.push_back(temp);
		GhostModels.push_back(temp);
	}
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
	//wallShader* ws = (WallShader*)faceModel->shader();
	//ws->setEnvioromentCube(SkyboxTexID);
	faceModel->draw(Cam);
}




