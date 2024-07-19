#include "LabyrinthMapGenerator.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#define LABYRINTH_MAIN

using namespace std;
#define M_PI 	                (3.1415926f)

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

bool get(const Grid& grid, int x, int y)
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


// Fractals: Fibonacci and haxagons structures
void generateFibonacciSpiral(Grid& maze, int centerX, int centerY, int length, int turns)
{
    double angle = 0.0;
    for (int i = 0; i < turns; ++i)
    {
        int x = centerX + static_cast<int>(length * cos(angle));
        int y = centerY + static_cast<int>(length * sin(angle));
        set(maze, x, y, true);
        angle += M_PI / 2; // 90 degrees
        length = static_cast<int>(length * 1.618); // Fibonacci ratio
    }
}
void generateHexagonFractal(Grid& maze, int centerX, int centerY, int size, int depth)
{
    if (depth <= 0)
    {
        return;
    }

    for (int i = 0; i < 9; ++i)
    {
        double angle = i * M_PI / 3; // 60 degrees
        int x = centerX + static_cast<int>(size * cos(angle));
        int y = centerY + static_cast<int>(size * sin(angle));
        set(maze, x, y, true);

        generateHexagonFractal(maze, x, y, size / 2, depth - 1);
    }
}


// Branched fractal
struct Point
{
    double x;
    double y;
};
void generateCurvedBranches(Grid& maze, int centerX, int centerY, int numBranches, int numPoints, int width, int height)
{
    // angles for branches and connecting points
    double angleStepBranch = 2 * M_PI / numBranches;
    double angleStepPoint = 2 * M_PI / numPoints;

    // end points - where branches strive to connect (end)
    std::vector<Point> endPoints;
    for (int i = 0; i < numPoints; ++i)
    {
        double angle = i * angleStepPoint;
        int endX = centerX + static_cast<int>((width / 2 - 1) * cos(angle));
        int endY = centerY + static_cast<int>((height / 2 - 1) * sin(angle));
        endPoints.push_back({ static_cast<double>(endX), static_cast<double>(endY) });
    }

    // branches
    for (int i = 0; i < numBranches; ++i)
    {
        double angle = i * angleStepBranch;
        double currentX = centerX;
        double currentY = centerY;
        double branchAngle = angle;
        double angleIncrement = angleStepBranch; // / 20.0;

        while (true)
        {
            int x = static_cast<int>(currentX);
            int y = static_cast<int>(currentY);

            if (x < 0 || x >= width || y < 0 || y >= height)
                break;

            set(maze, x, y, true);

            currentX += cos(branchAngle);
            currentY += sin(branchAngle);
            branchAngle += angleIncrement;

            if (branchAngle >= 2 * M_PI)
                branchAngle -= 2 * M_PI;
        }
    }

    // Connect branches to end points if possible
    for (int i = 0; i < numBranches && i < numPoints; ++i)
    {
        Point& endPoint = endPoints[i];
        double deltaX = endPoint.x - centerX;
        double deltaY = endPoint.y - centerY;
        double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        double step = 1.0 / distance;
        double t = 0.0;

        while (t < 1.0)
        {
            int x = static_cast<int>(centerX + t * deltaX);
            int y = static_cast<int>(centerY + t * deltaY);

            set(maze, x, y, true);

            t += step;
        }
    }
}


// calculate the dimension of the fractal (or possible fractal)
int countNonEmptyBoxes(const Grid& maze, int boxSize, int width, int height)
{
    int count = 0;
    for (int y = 0; y < height; y += boxSize)
    {
        for (int x = 0; x < width; x += boxSize)
        {
            bool hasContent = false;
            for (int j = 0; j < boxSize && !hasContent; ++j)
            {
                for (int i = 0; i < boxSize && !hasContent; ++i)
                {
                    if (get(maze, x + i, y + j))
                    {
                        hasContent = true;
                    }
                }
            }
            if (hasContent)
            {
                ++count;
            }
        }
    }
    return count;
}

double calculateFractalDimension(const Grid& maze, int width, int height)
{
    std::vector<int> boxSizes = { 1, 2, 4, 8, 16, 32 };
    std::vector<double> logN;
    std::vector<double> logR;

    for (int boxSize : boxSizes)
    {
        int n = countNonEmptyBoxes(maze, boxSize, width, height);
        logN.push_back(log(n));
        logR.push_back(log(1.0 / boxSize));
    }

    // calculate the slope of the line using linear regression
    int n = logN.size();
    double sumLogN = 0.0, sumLogR = 0.0, sumLogNLogR = 0.0, sumLogRSquared = 0.0;

    for (int i = 0; i < n; ++i)
    {
        sumLogN += logN[i];
        sumLogR += logR[i];
        sumLogNLogR += logN[i] * logR[i];
        sumLogRSquared += logR[i] * logR[i];
    }

    double slope = (n * sumLogNLogR - sumLogN * sumLogR) / (n * sumLogRSquared - sumLogR * sumLogR);
    return slope;
}

// generate branches which goes from top points of some figure (5 points - pentagone, 6 - hexagone, etc)
void generateBranchesFromPoints(Grid& maze, int width, int height, int numPoints, int numBranches)
{

    std::vector<IntVector2> points;
    double angleStep = 2 * M_PI / numPoints;
    double radiusX = width / 2 - 1;
    double radiusY = height / 2 - 1;
    int centerX = width / 2;
    int centerY = height / 2;

    for (int i = 0; i < numPoints; ++i)
    {
        double angle = i * angleStep;
        int x = centerX + static_cast<int>(radiusX * cos(angle));
        int y = centerY + static_cast<int>(radiusY * sin(angle));
        points.emplace_back(x, y);
    }

    double angleStepBranch = 2 * M_PI / numBranches;

    for (const auto& point : points)
    {
        for (int i = 0; i < numBranches; ++i)
        {
            double angle = i * angleStepBranch;
            double currentX = point.X;
            double currentY = point.Y;
            double branchAngle = angle;
            double angleIncrement = angleStepBranch / 10.0; //curvature

            while (true)
            {
                int x = static_cast<int>(currentX);
                int y = static_cast<int>(currentY);

                if (x < 0 || x >= width || y < 0 || y >= height)
                    break;

                if (get(maze, x, y) && std::find(points.begin(), points.end(), IntVector2(x, y)) == points.end())
                {
                    break;
                }

                set(maze, x, y, true);

                currentX += cos(branchAngle);
                currentY += sin(branchAngle);
                branchAngle += angleIncrement;

                if (branchAngle >= 2 * M_PI)
                    branchAngle -= 2 * M_PI;
            }
        }
    }
}

// Sierpinski like pattern generator
void generateSierpinskiLikeFigure(Grid& maze, int centerX, int centerY, int size, int figureType, int steps)
{
    auto drawPolygon = [&](int centerX, int centerY, int radius, int sides)
        {
            std::vector<IntVector2> vertices;
            double angleStep = 2 * M_PI / sides;

            for (int i = 0; i < sides; ++i)
            {
                double angle = i * angleStep;
                int x = centerX + static_cast<int>(radius * cos(angle));
                int y = centerY + static_cast<int>(radius * sin(angle));
                vertices.emplace_back(x, y);
            }

            for (size_t i = 0; i < vertices.size(); ++i)
            {
                IntVector2 start = vertices[i];
                IntVector2 end = vertices[(i + 1) % vertices.size()];
                int dx = abs(end.X - start.X);
                int dy = abs(end.Y - start.Y);
                int sx = start.X < end.X ? 1 : -1;
                int sy = start.Y < end.Y ? 1 : -1;
                int err = dx - dy;

                while (true)
                {
                    set(maze, start.X, start.Y, true);
                    if (start.X == end.X && start.Y == end.Y)
                        break;
                    int e2 = 2 * err;
                    if (e2 > -dy)
                    {
                        err -= dy;
                        start.X += sx;
                    }
                    if (e2 < dx)
                    {
                        err += dx;
                        start.Y += sy;
                    }
                }
            }
        };

    auto sierpinskiRecursive = [&](int centerX, int centerY, int size, int sides, int step, auto& sierpinskiRecursive) -> void
        {
            if (step == 0)
            {
                drawPolygon(centerX, centerY, size, sides);
                return;
            }

            double angleStep = 2 * M_PI / sides;
            for (int i = 0; i < sides; ++i)
            {
                double angle = i * angleStep;
                int newX = centerX + static_cast<int>((size / 2) * cos(angle));
                int newY = centerY + static_cast<int>((size / 2) * sin(angle));
                sierpinskiRecursive(newX, newY, size / 2, sides, step - 1, sierpinskiRecursive);
            }
        };

    sierpinskiRecursive(centerX, centerY, size, figureType, steps, sierpinskiRecursive);
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

    int width  = 37; // odd
    int height = 37; // odd

    Grid maze;

    //0, 1 - recursive, 2 - fractal, 3 - Fibonacci spiral, 4 - Hexagon fractal, 5 - Curved branches
    int mazeType = 7; 



    // generateHexagonFractal(maze, width / 2, height / 2, 8, 4);
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
    else if (mazeType == 3) {
        generateFibonacciSpiral(maze, width / 2, height / 2, 3, 25);
    }
    else if (mazeType == 4) {
        generateHexagonFractal(maze, width / 2, height / 2, 7, 5);
    }
    else if (mazeType == 5) {
        generateCurvedBranches(maze, width / 2, height / 2, 5, 7, width, height); // NUM branches, NUM end_points
    }
    else if (mazeType == 6) {
        generateBranchesFromPoints(maze, width, height, 5, 3);  // NUM points, NUM branches
        // generateBranchesFromPoints(maze, width, height, 9, 9);  // NUM points, NUM branches
        // generateBranchesFromPoints(maze, width, height, 6, 6);  // NUM points, NUM branches
    }
    else if (mazeType == 7) {
        int centerX = width / 2;
        int centerY = height / 2;
        int size = width / 2;
        int figureType = 9;
        int steps = 1;

		generateSierpinskiLikeFigure(maze, centerX, centerY, int(width / 1.5), 4, 1); // centerX, centerY, size, sides, steps
		generateSierpinskiLikeFigure(maze, centerX, centerY, int(width / 2), 8, 1);
	}
    else {

        fractalTessellation(maze, 1, 5, width); // width == height
    }

    printMaze(maze, width, height);

    double fractalDimension = calculateFractalDimension(maze, width, height);
    std::cout << "Estimated Fractal Dimension: " << fractalDimension << std::endl;

    return 0;
}
#endif
