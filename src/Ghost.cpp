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

vector<Vector> Ghost::findPath()
{
    //int dimensions = associatedFace->layout->dimensions;
    int dimensions = 32;

    Vector currentPos = this->ghostModel->transform().translation();
    pair<float, float> start = vectorToGrid(currentPos);
    pair<float, float> goal = vectorToGrid(this->target);

    queue<pair<float, float>> queue;
    map<pair<float, float>, pair<float, float>> cameFrom; // To reconstruct the path
    vector<vector<bool>> visited(dimensions, vector<bool>(dimensions, false));

    queue.push(start);
    visited[start.first][start.second] = true;

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (current == goal) {
            break; // Reached the goal
        }

        // Explore neighbors (up, down, left, right)
        for (auto direction : { make_pair(0, 1), make_pair(1, 0), make_pair(0, -1), make_pair(-1, 0) }) {
            pair<float, float> next = { current.first + direction.first, current.second + direction.second };

            if (next.first >= 0 && next.first < dimensions && next.second >= 0 && next.second < dimensions && !visited[next.first][next.second] && !associatedFace->layout->maze[next.first][next.second].isWall) {
                queue.push(next);
                visited[next.first][next.second] = true;
                cameFrom[next] = current;
            }
        }
    }

    // Reconstruct the path from goal to start
    vector<Vector> path;
    for (auto at = goal; at != start; at = cameFrom[at]) {
        path.push_back(gridToVector(make_pair(at.first, at.second)));
    }
    //path.push_back(gridToVector(start.first, start.second));
    path.push_back(gridToVector(make_pair(start.first, start.second)));
    reverse(path.begin(), path.end());

    return path;
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
    if (unusedAxis == 0) position.X = this->Transform.translation().X;
    else if (unusedAxis == 1) position.Y = this->Transform.translation().Y;
    else if (unusedAxis == 2) position.Z = this->Transform.translation().Z;

    return position;
}

// Member variables added to Ghost class
float elapsedTime = 0.0; // Time elapsed since last path update
size_t currentPathIndex = 0; // Index of the current target position in the path
vector<Vector> currentPath; // Current path the Ghost is following

void Ghost::update(float dtime)
{
    elapsedTime += dtime;

    // Every 10 seconds, update the path and reset the timer and path index
    if (elapsedTime >= 10.0f) {
        currentPath = this->findPath();
        currentPathIndex = 0;
        elapsedTime = 0.0f; // Reset the timer
    }

    // If we have a path to follow and we're not at the end
    if (!currentPath.empty() && currentPathIndex < currentPath.size()) {
        Vector currentPosition = this->ghostModel->transform().translation();
        Vector targetPosition = currentPath[currentPathIndex];

        // Calculate direction and move towards the target position
        Vector direction = (targetPosition - currentPosition).normalize();
        float distanceToMove = speed * dtime; // Distance to move based on speed and elapsed time

        // Check if we are close enough to consider having reached the target position
        if ((targetPosition - currentPosition).length() <= distanceToMove) {
            // Set ghost's position to the target position and move to the next point in the path
            currentPosition = targetPosition;
            currentPathIndex++;
        }
        else {
            // Move towards the target position
            currentPosition += direction * distanceToMove;
        }

        // Apply the new position
        Matrix newTransform;
        newTransform.translation(currentPosition);
        this->ghostModel->transform(newTransform * this->initTransform);
    }

    // Additional logic here to end game??
    return;
}

void Ghost::draw(const BaseCamera& Cam)
{
    ghostModel->draw(Cam);
}

void Ghost::setTarget(Vector targetPosition)
{
    target = targetPosition;
}
