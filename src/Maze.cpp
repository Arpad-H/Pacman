#include "Maze.h"

using namespace std;

Maze::Maze() {
	size = 0;
}

Maze::~Maze()
{
}
Maze::Maze(int size) {
	this->size = size;
	srand(time(0)); // Seed for random number generation
	initMaze();
	generateMaze();
	openMazeEdges(); // Open edges after generating the maze
}

void  Maze::display() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << (maze[i][j].isWall ? 'X' : ' ') << ' ';
		}
		cout << '\n';
	}
}

int Maze::getSize() const
{
	return this->size;
}

void Maze::initMaze() {
	maze = vector<vector<Cell>>(size, vector<Cell>(size));
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			maze[i][j] = Cell(i, j);
}

void Maze::generateMaze() {
	Cell current = maze[rand() % size][rand() % size];
	current.visited = true;
	current.isWall = false;
	cellStack.push(current);

	while (!cellStack.empty()) {
		Cell next = getNextCell(current);

		if (next.x != -1) { // If a valid next cell is found
			next.visited = true;
			next.isWall = false;
			maze[next.x][next.y] = next;
			cellStack.push(next);
			removeWall(current, next);
			current = next;
		}
		else {
			current = cellStack.top();
			cellStack.pop();
		}
	}
}

void Maze::openMazeEdges() {
	// Open random points on the edges to ensure some paths extend to all edges
	// This can be adjusted to ensure more sophisticated or specific edge openings
	for (int i = 0; i < size; i += size - 1) { // Top and bottom edges
		int randomOpen = rand() % (size / 2) * 2; // Ensure an odd index for opening
		maze[i][randomOpen].isWall = false;
	}
	for (int j = 0; j < size; j += size - 1) { // Left and right edges
		int randomOpen = rand() % (size / 2) * 2; // Ensure an odd index for opening
		maze[randomOpen][j].isWall = false;
	}
}

Cell Maze::getNextCell(Cell& current) {
	vector<Cell> neighbors;

	// Lambda to check cell validity
	auto isValid = [&](int x, int y) {
		return x >= 0 && x < size && y >= 0 && y < size && !maze[x][y].visited;
		};

	// Check all four directions
	if (isValid(current.x - 2, current.y)) neighbors.push_back(maze[current.x - 2][current.y]);
	if (isValid(current.x + 2, current.y)) neighbors.push_back(maze[current.x + 2][current.y]);
	if (isValid(current.x, current.y - 2)) neighbors.push_back(maze[current.x][current.y - 2]);
	if (isValid(current.x, current.y + 2)) neighbors.push_back(maze[current.x][current.y + 2]);

	if (!neighbors.empty()) {
		return neighbors[rand() % neighbors.size()];
	}

	return Cell(-1, -1); // Return invalid cell if no unvisited neighbors
}

void Maze::removeWall(Cell& current, Cell& next) {
	int xDiff = next.x - current.x;
	int yDiff = next.y - current.y;

	if (xDiff == 2) maze[current.x + 1][current.y].isWall = false;
	if (xDiff == -2) maze[current.x - 1][current.y].isWall = false;
	if (yDiff == 2) maze[current.x][current.y + 1].isWall = false;
	if (yDiff == -2) maze[current.x][current.y - 1].isWall = false;

	maze[next.x][next.y].visited = true; // Mark the new cell as visited
}

