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
    void setFace(Face* face); // Set the face the ghost is associated with)
    Model* ghostModel;
    vector<Vector> findPath();
protected:
    Face* associatedFace; // Pointer to the face the ghost is associated with
    float speed;
    vector<Vector> maze;
    pair<float, float> activeAxes;
    int id; // To identify different ghosts, shader spaeter?
    Vector direction; // Current movement direction
    // Vector target and initilise it with 0 0 0 
    Vector target = Vector(0, 0, 0);
    Vector nextGoal;
    bool hasGoal;
    pair<float, float> vectorToGrid(Vector position);
    Vector gridToVector(pair<float, float> gridPos);
    bool isWall(pair<float, float> gridPos);

};

#endif  /* Ghost_hpp */


