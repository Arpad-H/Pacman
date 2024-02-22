#ifndef Maze_hpp
#define Maze_hpp

#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

using namespace std;

struct Cell {
    bool isWall = true;
    bool visited = false;
    int x, y;
    Cell(int x = 0, int y = 0) : x(x), y(y) {}
};

class Maze {
public:
    
    Maze();
    virtual ~Maze();
    Maze(int size);
    void display();
    // get Size of the maze
    int getSize() const;
    vector<vector<Cell>> maze;
private:
    int size;
    stack<Cell> cellStack;
    void initMaze();
    void generateMaze();
    void openMazeEdges();
    Cell getNextCell(Cell& current);
    void removeWall(Cell& current, Cell& next);
};

#endif // Maze_hpp
