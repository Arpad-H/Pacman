#ifndef Ghost_hpp
#define Ghost_hpp

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "GameObject.h"
#include "Level.h"

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
protected:
    Face* associatedFace; // Pointer to the face the ghost is associated with
    float speed;
    pair<int, int> activeAxes;
    int id; // To identify different ghosts, shader spaeter?
    Vector direction; // Current movement direction
    Vector target; // Current target position
    void chooseDirection(); // Method to decide the next direction based
    void moveToTarget(float dtime); // Method to move towards the target

private:

};

#endif  /* Ghost_hpp */


