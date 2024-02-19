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
    Ghost(const char* ModelFilePath, bool FitSize, Vector initScale, int ghostId);
    virtual ~Ghost();
    void update(float dtime);
    void draw(const BaseCamera& Cam);
    void setLevel(Level* level);
    void setTarget(Vector targetPosition); // Ghosts target Pacman
protected:
    Model* ghostModel;
    Level* level;
    float speed;
    int id; // To identify different ghosts, shader spaeter?
    Vector direction; // Current movement direction
    Vector target; // Current target position
    void chooseDirection(); // Method to decide the next direction based
    void moveToTarget(float dtime); // Method to move towards the target
   // vector<Cell> findPath(const Maze& maze, const Cell& start, const Cell& target);

private:
    //vector<Cell> reconstructPath(map<Cell, Cell>& predecessor, const Cell& target);

};

#endif  /* Ghost_hpp */


