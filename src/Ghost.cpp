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
    this->setFace(face);
    // print out
    std::cout << "Ghost " << id << " created" << std::endl;
    // print out the position of the ghost
    std::cout << ghostModel->transform().translation().X << " " << ghostModel->transform().translation().Y << " " << ghostModel->transform().translation().Z << std::endl;
}

Ghost::~Ghost()
{
    // Clean up
}

void Ghost::positionGhost(Vector position)
{
    Matrix startPos;
    // match rotation with face
    rotation = this->associatedFace->rotateToMatchFace(this->ghostModel->transform().up());
    initTransform = ghostModel->transform();
    startPos.translation(position);
    ghostModel->transform( startPos*  initTransform * rotation);// Rotation working
    //ghostModel->transform(startPos * initTransform);
}

void Ghost::setFace(Face* face)
{
    this->associatedFace = face;
    this->activeAxes = associatedFace->determineActiveAxes(); // based on the face's normal
    Vector ghostPos = associatedFace->getInitGhostPosition(); // based on the maze
    this->positionGhost(ghostPos);
}

vector<Vector> Ghost::findPath()
{
    Vector currentPos = this->ghostModel->transform().translation();
    pair<float, float> start = vectorToGrid(currentPos);
    pair<float, float> goal = vectorToGrid(this->target);

    std::cout << "Start: " << start.first << ", " << start.second << std::endl;
    std::cout << "Goal: " << goal.first << ", " << goal.second << std::endl;

    // direction pairs for the four cardinal directions
    pair<float, float> directions[4] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

    // Queue for BFS, holding paths
    queue<vector<pair<float, float>>> q;
    // Start path with the starting position
    q.push({ start });

    // set of visited positions
    set<pair<float, float>> visited;
    visited.insert(start);

    // is q empty?
    while (!q.empty()) {
        // Get the first path from the queue
        vector<pair<float, float>> path = q.front();
        q.pop();

        // Current position is the last element of the path
        pair<float, float> current = path.back();

        // check if the current position is the goal (or close enough)
        if (abs(current.first - goal.first) < 0.5f && abs(current.second - goal.second) < 0.5f) { // make it a bit smaller Epislon
            // Convert the path to a vector of positions and return it
            vector<Vector> positions;
            for (auto& p : path) {
                positions.push_back(gridToVector(p));
            }
            return positions; // valid path found
        }

        // we didn't reach the goalm explore the neighbours, for loop for directions
        for (auto& dir : directions) {
            pair<float, float> next = { current.first + dir.first, current.second + dir.second };
            // check if the next position is within bounds and not a wall
            if (isWithinBounds(next) && !isWall(next)) {
                // check if the next position has been visited, if yes, skip it
                if (visited.find(next) == visited.end()) {
                    // mark the next position as visited
                    visited.insert(next);
                    // create a new path by copying the current path and adding the next position
                    vector<pair<float, float>> new_path = path;
                    new_path.push_back(next);
                    // add the new path to the queue
                    q.push(new_path);
                }
            }
        }
    }
    // path not found, aber eigentlich sollte das nicht passieren oder????
    return {};
}


bool Ghost::isWall(pair<float, float> gridPos)
{
    return this->associatedFace->checkWall(this->gridToVector(gridPos));
}

bool Ghost::isWithinBounds(pair<float, float> gridPos)
{
    Vector pos = this->gridToVector(gridPos);
    return this->associatedFace->isWithinBounds(pos);
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



void Ghost::update(float dtime)
{
    elapsedTime += dtime;
    blinkElapsedTime += dtime;

    // get actual target
    target = this->associatedFace->getTarget();
    // Check if target still init value
    if (target == Vector(0, 0, 0)) {
        return;
    }

    // Update the path every 3 seconds, weil Rechenintensiv
    if (elapsedTime >= 3.0f) {
        cout << "Ghost update" << endl;
        currentPath = this->findPath();
        currentPathIndex = 0;
        elapsedTime = 0.0f; // Reset the path update timer
        cout << "Path size: " << currentPath.size() << endl;
    }

    // Blink to the next position every .3 second
    if (blinkElapsedTime >= 0.3f) {
        if (!currentPath.empty() && currentPathIndex < currentPath.size()) {
            // Update the Ghost's position to the next path point
            Vector currentPosition = currentPath[currentPathIndex];

            // Apply the new position
            Matrix newTransform;
            newTransform.translation(currentPosition);
            //Matrix rotationCorrection;
            //rotationCorrection = this->associatedFace->rotateToMatchFace(this->ghostModel->transform().up());
            //ghostModel->transform(newTransform * initTransform * rotationCorrection);
            ghostModel->transform(newTransform *rotation *initTransform);

            currentPathIndex++; // Prepare to move to the next point on the next blink
        }

        blinkElapsedTime = 0.0f; // Reset the blink timer
    }
    
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