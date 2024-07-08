#include "LabyrinthMapGenerator.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#define LABYRINTH_MAIN

using namespace std;


struct IntVector2
{
    int X;
    int Y;

    IntVector2(int x, int y)
        : X(x), Y(y)
    {
    }

    IntVector2(const IntVector2& other)
        : X(other.X), Y(other.Y)
    {
    }

    friend bool operator==(const IntVector2& a, const IntVector2& b) {
        return a.X == b.X && a.Y == b.Y;
    }
};

template<> struct std::hash<IntVector2> {
    std::size_t operator()(IntVector2 const& s) const noexcept
    {
        return s.X ^ s.Y;
    }
};

typedef std::unordered_map<IntVector2, bool> Grid;

bool get(Grid& grid, int x, int y)
{
    auto it = grid.find(IntVector2(x, y));
    if (it == grid.end())
        return false;

    return it->second;
}

void set(Grid& grid, int x, int y, bool val)
{
    grid.insert_or_assign(IntVector2(x, y), val);
}

bool isInBounds(int x, int y, int width, int height)
{
    return x > 0 && x < width - 1 && y > 0 && y < height - 1;
}

vector<IntVector2> getNeighbors(IntVector2 cell)
{
    return { IntVector2(cell.X + 2, cell.Y), IntVector2(cell.X - 2, cell.Y),
             IntVector2(cell.X, cell.Y + 2), IntVector2(cell.X, cell.Y - 2) };
}

void generateMaze(Grid& maze, int width, int height)
{
    stack<IntVector2> cellStack;
    int totalCells = (width / 2) * (height / 2);
    int visitedCells = 1;

    IntVector2 current(1, 1);
    set(maze, current.X, current.Y, true);

    while (visitedCells < totalCells)
    {
        vector<IntVector2> neighbors;
        for (auto& neighbor : getNeighbors(current))
        {
            if (isInBounds(neighbor.X, neighbor.Y, width, height) && !get(maze, neighbor.X, neighbor.Y))
            {
                neighbors.push_back(neighbor);
            }
        }

        if (!neighbors.empty())
        {
            IntVector2 next = neighbors[rand() % neighbors.size()];
            set(maze, (current.X + next.X) / 2, (current.Y + next.Y) / 2, true); // Remove wall
            set(maze, next.X, next.Y, true);
            cellStack.push(current);
            current = next;
            visitedCells++;
        }
        else if (!cellStack.empty())
        {
            current = cellStack.top();
            cellStack.pop();
        }
    }
}

void recursiveMazeGenerator(Grid& maze, IntVector2 current, int width, int height)
{
    set(maze, current.X, current.Y, true);

    vector<IntVector2> neighbors = getNeighbors(current);

    std::random_shuffle(neighbors.begin(), neighbors.end());

    for (auto& neighbor : neighbors)
    {
        if (isInBounds(neighbor.X, neighbor.Y, width, height) && !get(maze, neighbor.X, neighbor.Y))
        {
            set(maze, (current.X + neighbor.X) / 2, (current.Y + neighbor.Y) / 2, true);
            recursiveMazeGenerator(maze, neighbor, width, height);
        }
    }
}

void fractalTessellation(Grid& maze, int x, int y, int size)
{
    if (size <= 1)
    {
        set(maze, x, y, true);
        return;
    }

    int newSize = size / 2;
    fractalTessellation(maze, x, y, newSize);
    fractalTessellation(maze, x + newSize, y, newSize);
    fractalTessellation(maze, x, y + newSize, newSize);
    fractalTessellation(maze, x + newSize, y + newSize, newSize);

    // remove 3 walls between the 4 smaller mazes
    set(maze, x + newSize / 2, y + newSize / 2, true);
    set(maze, x + newSize + newSize / 2, y + newSize / 2, true);
    set(maze, x + newSize / 2, y + newSize + newSize / 2, true);
}

void printMaze(Grid& maze, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (get(maze, x, y))
                cout << "  ";
            else
                cout << "[]";
        }
        cout << "\n";
    }
}

#ifdef LABYRINTH_MAIN
int main()
{
    srand(time(0));

    int width = 21; // odd
    int height = 31; // odd

    Grid maze;

    


    int mazeType = 1; //0, 1 - recursive, 2 - fractal

    if (mazeType == 0) {
        generateMaze(maze, width, height);
    }
    else if (mazeType == 1) {
        set(maze, 1, 0, true);
        set(maze, (int)(width / 2), 0, true);
        // set(maze, width - 2, height - 1, true);
        set(maze, (int)(width / 2), height - 1, true);
        IntVector2 start(1, 1);
        recursiveMazeGenerator(maze, start, width, height);
    }
    else {

        fractalTessellation(maze, 0, 0, width); // width == height
    }

    printMaze(maze, width, height);

    return 0;
}
#endif
