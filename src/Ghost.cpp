#include "Ghost.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <queue>
#define EPSILON 1e-6
static float toRad(float deg) { return deg * M_PI / 180.0f; }

Ghost::Ghost(const char* ModelFilePath, bool FitSize, Vector initScale, int ghostId)
    : id(ghostId)
{
    ghostModel = new Model();
    loadModels(ModelFilePath, FitSize, initScale, *ghostModel);
    // Initialize other attributes as needed
    speed = 3; // Ghosts might move at a different speed than Pacman
}

Ghost::~Ghost()
{
    // Clean up as necessary
}

void Ghost::setLevel(Level* level)
{
    this->level = level;
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

/*
vector<Cell> Ghost::findPath(const Maze& maze, const Cell& start, const Cell& target) {
    vector<vector<bool>> visited(maze.getSize(), vector<bool>(maze.getSize(), false));
    map<Cell, Cell> predecessor;
    queue<Cell> queue;

    // Lambda to check if a cell is within the maze bounds and not a wall
    auto isAccessible = [&](const Cell& cell) {
        return cell.x >= 0 && cell.x < maze.getSize() && cell.y >= 0 && cell.y < maze.getSize() && !maze.maze[cell.x][cell.y].isWall;
        };

    // Start BFS
    visited[start.x][start.y] = true;
    queue.push(start);

    while (!queue.empty()) {
        Cell current = queue.front();
        queue.pop();

        // Reached target
        if (current.x == target.x && current.y == target.y) {
            return reconstructPath(predecessor, target);
        }

        // Check all 4 directions
        vector<Cell> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
        for (const Cell& dir : directions) {
            Cell next = { current.x + dir.x, current.y + dir.y };
            if (isAccessible(next) && !visited[next.x][next.y]) {
                visited[next.x][next.y] = true;
                predecessor[next] = current;
                queue.push(next);
            }
        }
    }

    // If no path found
    return vector<Cell>();
}
vector<Cell> Ghost::reconstructPath(map<Cell, Cell>& predecessor, const Cell& target) {
    vector<Cell> path;
    for (Cell at = target; predecessor.find(at) != predecessor.end(); at = predecessor[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}*/
