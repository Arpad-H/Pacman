#ifndef Ghost_hpp
#define Ghost_hpp

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "GameObject.h"
#include "Level.h"
#include "vector.h"
#include "Face.h"
#include <set> // Include the set header


class Ghost : public GameObject
{
public:
    Ghost(const char* ModelFilePath, bool FitSize, Vector initScale, int ghostId, Face* face);
    virtual ~Ghost();
    void update(float dtime);
    void draw(const BaseCamera& Cam);
    void setTarget(Vector targetPosition); // Ghosts target Pacman
    void positionGhost(Vector position); // Set the ghost to a specific position
    void setFace(Face* face); // Set the face the ghost is associated with
    Model* ghostModel;
    vector<Vector> findPath();
    void setElapsedTime(float time);
    
protected:
    Face* associatedFace; // Pointer to the face the ghost is associated with
    pair<float, float> activeAxes;
    int id; // To identify different ghosts, shader spaeter?
    // Vector target and initilise it with 0 0 0 
    Vector target = Vector(0, 0, 0);
    pair<float, float> vectorToGrid(Vector position);
    Vector gridToVector(pair<float, float> gridPos);
    bool isWall(pair<float, float> gridPos);
    PhongShader* phongShader;
    bool isWithinBounds(pair<float, float> gridPos);
    float elapsedTime = 0.0; // Time elapsed since last path update
    size_t currentPathIndex = 0; // Index of the current target position in the path
    vector<Vector> currentPath; // Current path the Ghost is following
    float blinkElapsedTime = 0.0; // Time elapsed since last blink
    Matrix rotation;
};

#endif  /* Ghost_hpp */