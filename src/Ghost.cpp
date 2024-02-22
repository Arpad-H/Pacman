#include "Ghost.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <queue>
#define EPSILON 1e-6
static float toRad(float deg) { return deg * M_PI / 180.0f; }

Ghost::Ghost(const char* ModelFilePath, bool FitSize, Vector initScale, int ghostId, Face* face)
    : id(ghostId)
{
    ghostModel = new Model();
    loadModels(ModelFilePath, FitSize, initScale, *ghostModel);
    scale = ghostModel->transform();
    speed = 3; // Ghosts move at a different speed than Pacman
    this->setFace(face);
    // print out
std::cout << "Ghost " << id << " created" << std::endl;
    // print out the position of the ghost
	std::cout <<  ghostModel->transform().translation().X << ghostModel->transform().translation().Y << ghostModel->transform().translation().Z << std::endl;
}

Ghost::~Ghost()
{
    // Clean up as necessary
}

void Ghost::positionGhost(Vector position)
{
    Matrix startPos, rotation;
    rotation.rotationYawPitchRoll(toRad(180), 0, 0);
    initTransform = rotation*ghostModel->transform();
    startPos.translation(position);
    ghostModel->transform(startPos* initTransform);
}

void Ghost::setFace(Face* face)
{
	this->associatedFace = face;
    this->activeAxes = associatedFace->determineActiveAxes();
    Vector ghostPos = associatedFace->getInitGhostPosition();
    this->positionGhost(ghostPos);
}

void Ghost::update(float dtime)
{
    // AI to decide movement, targeting, etc.
    //chooseDirection();
   // moveToTarget(dtime);
}

void Ghost::draw(const BaseCamera& Cam)
{
    ghostModel->draw(Cam);
}

void Ghost::setTarget(Vector targetPosition)
{
    target = targetPosition;
}



void Ghost::chooseDirection()
{
    // logic here to choose the next direction based on the current target, maze layout, etc.
}

void Ghost::moveToTarget(float dtime)
{
    // Calculate movement towards the target within the confines of the current level
    // Movement logic can be adapted from Pacman::update, with modifications for ghost-specific behavior
}
