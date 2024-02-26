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
    ghostModel->shader(new PhongShader(), true);
    scale = ghostModel->transform();
    speed = 3; // Ghosts move at a different speed than Pacman
    this->setFace(face);
    // print out
std::cout << "Ghost " << id << " created" << std::endl;
    // print out the position of the ghost
	std::cout <<  ghostModel->transform().translation().X << " " << ghostModel->transform().translation().Y << " " << ghostModel->transform().translation().Z << std::endl;

    // test target
    // this->setTarget(Vector(12.5f, 16.5f, 4.5f));
}

Ghost::~Ghost()
{
    // Clean up as necessary
}

void Ghost::positionGhost(Vector position)
{
    Matrix startPos, rotation;
    //rotation.rotationYawPitchRoll(toRad(180), 0, 0);
    initTransform = ghostModel->transform();
    startPos.translation(position);
    ghostModel->transform(startPos* initTransform);
}

void Ghost::setFace(Face* face)
{
	this->associatedFace = face;
    this->activeAxes = associatedFace->determineActiveAxes();
    Vector ghostPos = associatedFace->getInitGhostPosition();
    this->positionGhost(ghostPos);
    this->maze = associatedFace->wallPositions;
}

vector<Vector> Ghost::findPath()
{
    int dimensions = 32; // Assuming a 32x32 grid for the maze
    Vector currentPos = this->ghostModel->transform().translation();
    pair<float, float> start = vectorToGrid(currentPos);
    pair<float, float> goal = vectorToGrid(this->target);

    std::cout << "Start: " << start.first << ", " << start.second << std::endl;
    std::cout << "Goal: " << goal.first << ", " << goal.second << std::endl;

    queue<pair<float, float>> q;
    map<pair<float, float>, pair<float, float>> prev;
    vector<pair<float, float>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} }; // Possible directions to move

    q.push(start);
    prev[start] = { -1, -1 }; // Use an invalid grid position to mark the start

    while (!q.empty()) {
        pair<float, float> current = q.front();
        q.pop();

        std::cout << "Current: " << current.first << ", " << current.second << " | Queue Size: " << q.size() << std::endl;

        if (current == goal) {
            std::cout << "Goal reached." << std::endl;
            break; // Goal reached
        }

        for (auto dir : directions) {
            pair<float, float> next = { current.first + dir.first, current.second + dir.second };

            if (next.first >= 0 && next.first < dimensions && next.second >= 0 && next.second < dimensions) {
                if (!isWall(next) && prev.find(next) == prev.end()) { // this line is always false
                    q.push(next);
                    prev[next] = current;
                    std::cout << "Adding: " << next.first << ", " << next.second << " to Queue" << std::endl;
                }
            }
        }
    }

    // Reconstruct path
    vector<Vector> path;
    for (pair<float, float> at = goal; at != start; at = prev[at]) {
        if (at.first == 0 && at.second == 0) {
            std::cout << "No path found." << std::endl;
            break; // Check for no path found
        }
        path.push_back(gridToVector(at));
    }
    std::cout << "Path size: " << path.size() << std::endl;
    reverse(path.begin(), path.end());

    return path;
}


bool Ghost::isWall(pair<float, float> gridPos)
{
    // return true 50% of the time
    //return rand() % 2 == 0;
    //cout << "Checking wall at: " << gridPos.first << ", " << gridPos.second << endl;
   // cout << "Wall status: " << this->associatedFace->checkWall(this->gridToVector(gridPos)) << endl;
    return this->associatedFace->checkWall(this->gridToVector(gridPos));
}

pair<float, float> Ghost::vectorToGrid(Vector position)
{
    float firstComponent = 0.0f, secondComponent = 0.0f;

    // Extract the relevant components based on activeAxes
    if (activeAxes.first == 0) firstComponent = position.X;
    else if (activeAxes.first == 1) firstComponent = position.Y;
    else if (activeAxes.first == 2) firstComponent = position.Z;

    if (activeAxes.second == 0) secondComponent = position.X;
    else if (activeAxes.second == 1) secondComponent = position.Y;
    else if (activeAxes.second == 2) secondComponent = position.Z;

    return { firstComponent, secondComponent };
}

Vector Ghost::gridToVector(pair<float, float> gridPos)
{
    Vector position(0.0f, 0.0f, 0.0f); // Initialize with zeros

    // We need to find out which axis is not used and set its value to the ghost's relevant attribute
    int unusedAxis = 3 - (activeAxes.first + activeAxes.second);

    // Set the values based on activeAxes and the unused axis
    if (activeAxes.first == 0) position.X = gridPos.first;
    else if (activeAxes.first == 1) position.Y = gridPos.first;
    else if (activeAxes.first == 2) position.Z = gridPos.first;

    if (activeAxes.second == 0) position.X = gridPos.second;
    else if (activeAxes.second == 1) position.Y = gridPos.second;
    else if (activeAxes.second == 2) position.Z = gridPos.second;

    // Set the value of the unused axis
    if (unusedAxis == 0) position.X = this->ghostModel->transform().translation().X;
    else if (unusedAxis == 1) position.Y = this->ghostModel->transform().translation().Y;
    else if (unusedAxis == 2) position.Z = this->ghostModel->transform().translation().Z;

    return position;
}

// Member variables added to Ghost class
float elapsedTime = 0.0; // Time elapsed since last path update
size_t currentPathIndex = 0; // Index of the current target position in the path
vector<Vector> currentPath; // Current path the Ghost is following

void Ghost::update(float dtime)
{
    elapsedTime += dtime;

    target = this->associatedFace->getTarget();
    // check if target is null
    if (target == Vector(0, 0, 0)) {
        return;
    }

    // Every 10 seconds, update the path and reset the timer and path index
    if (elapsedTime >= 5.0f) {
        cout << "Ghost update" << endl;
        currentPath = this->findPath();
        currentPathIndex = 0;
        elapsedTime = 0.0f; // Reset the timer
    }

    // If we have a path to follow and we're not at the end
    if (!currentPath.empty() && currentPathIndex < currentPath.size()) {
        Vector currentPosition = this->ghostModel->transform().translation();

        // Use the current path point as the target, not the final target
        Vector targetPosition = currentPath[currentPathIndex];

        // Calculate direction and move towards the current path point
        Vector direction = (targetPosition - currentPosition).toUnitVector();
        float distanceToMove = speed * dtime; // Distance to move based on speed and elapsed time

        // Check if close enough to the current path point before calculating movement
        if ((targetPosition - currentPosition).length() <= distanceToMove) {
            // Set ghost's position to the current path point and move to the next point in the path
            currentPosition = targetPosition;
            currentPathIndex++;
        }
        else {
            // Move towards the current path point
            currentPosition += direction * distanceToMove;
        }

        // Apply the new position
        Matrix newTransform;
        newTransform.translation(currentPosition);
        ghostModel->transform(newTransform * initTransform);
    }

   
    return;
}


void Ghost::draw(const BaseCamera& Cam)
{
    ghostModel->draw(Cam);
}

void Ghost::setTarget(Vector targetPosition)
{
    target = targetPosition;
    cout << "Target set to: " << targetPosition.X << ", " << targetPosition.Y << ", " << targetPosition.Z << endl;
}
