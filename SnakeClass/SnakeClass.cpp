#include "SnakeClass.h"
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include "../Variables/Variables.h"
#include "../GraphClass/GraphClass.h"
#include <chrono>
#include <iostream>
#include <climits>
#include <random>
#include <iterator>

using namespace std;


deque <Vector2> Snake::SetInitialBody() {
    float x = ceil(gridWidth / 2.0f);
    float y = ceil(gridHeight / 2.0f) - 1;

    if (x - 3 == -1) {
        return {
            Vector2{x, y},
            Vector2{x - 1, y},
            Vector2{x - 2, y}
        };
    } 
    else if (x - 2 == -1) {
        return {
            Vector2{x + 1, y},
            Vector2{x, y},
            Vector2{x - 1, y}
        };
    }

    return {
        Vector2{x - 1, y},
        Vector2{x - 2, y},
        Vector2{x - 3, y}
    };
}


Vector2 Snake::CenterOf(Vector2 seg, int horizontalGamePadding) {
    return {
        horizontalGamePadding + seg.x * cellSize + cellSize / 2,
        verticalGamePadding + seg.y * cellSize + cellSize / 2
    };
}


Vector2 Snake::CornerOf(Vector2 seg, int horizontalGamePadding) {
    return {
        horizontalGamePadding + seg.x * cellSize,
        verticalGamePadding + seg.y * cellSize
    };
}


void Snake::Draw(int horizontalGamePadding) {
    float snakeThickness = cellSize*0.75f;

    for (int i = 1; i < (int)body.size(); i++) {
        Vector2 a = CenterOf(body[i], horizontalGamePadding);
        Vector2 b = CenterOf(body[i - 1], horizontalGamePadding);

        Vector2 dir = Vector2Normalize(Vector2Subtract(b, a));
        Vector2 extendedA = Vector2Subtract(a, Vector2Scale(dir, snakeThickness / 2.0f));
        Vector2 extendedB = Vector2Add(b, Vector2Scale(dir, snakeThickness / 2.0f));

        DrawLineEx(extendedA, extendedB, snakeThickness, snakeColor);
    }
}


void Snake::Update() {
    Vector2 newHead = Vector2Add(body[0], direction);

    body.push_front(newHead);
    occupied.insert(newHead);
}


void Snake::Grow() {
    if (!addSegment) {
        Vector2 tail = body.back();

        body.pop_back();
        occupied.erase(tail);
    }
    else {
        addSegment = false;
    }
}


void Snake::Reset() {
    body = SetInitialBody();
    occupied = unordered_set<Vector2, Vector2Hash>(body.begin(), body.end());
    direction = {1, 0};
    addSegment = false;
    cycle = GetCycle();
}


Vector2 Snake::GenerateMove(Vector2 foodPos) {
    
    // Get the next position in the cycle
    Vector2 current = body[0];
    auto currentIt = find(cycle.begin(), cycle.end(), current);
    auto nextIt = next(currentIt);
    if (nextIt == cycle.end()) nextIt = cycle.begin();
    Vector2 cycleDir = *nextIt - current;


    Vector2 distanceToFood = foodPos - current;

    Vector2 dirToFoodX = {
        distanceToFood.x == 0.0f ? 0.0f : (distanceToFood.x > 0.0f ? 1.0f : -1.0f), 
        0.0f
    };
    Vector2 dirToFoodY = {
        0.0f,
        distanceToFood.y == 0.0f ? 0.0f : (distanceToFood.y > 0.0f ? 1.0f : -1.0f)
    };


    if (cycleDir == Vector2{1, 0} || cycleDir == Vector2{-1, 0} || cycleDir == Vector2{0, 1} || cycleDir == Vector2{0, -1}) { // As long as cycledir is valid
    
        Vector2 cuttingDir = GetCuttingDir(current, dirToFoodX, dirToFoodY);

        return cycleDir;
    }

    cout << "Invalid cycle direction, new game started" << endl;
    return Vector2{0, 0};
}


Vector2 Snake::GetCuttingDir(Vector2 current, Vector2 dirToFoodX, Vector2 dirToFoodY) {

    vector<Vector2> validDirections; // A vector of valid directions
    if (fabs(dirToFoodX.x) > 0) validDirections.push_back(dirToFoodX); // Can move on the x-axis if head not on the same x-coordinate as food
    if (fabs(dirToFoodY.y) > 0) validDirections.push_back(dirToFoodY); // Can move on the y-axis if head not on the same y-coordinate as food

    for (auto& dir : validDirections) { // Try cutting on the x-axis

        Vector2 candPos = Vector2Add(current, dir);

        auto it = find(cycle.begin(), cycle.end(), candPos);

        int i = distance(cycle.begin(), it); // The cycle index of the head of the snake
        vector <Vector2> shape;

        for (;;) {
            i--;
            if (i < 0) i = cycle.size() - 1; // Loop around
            if (cycle[i] == current) break; // Don't add current (the second snake compartment)
            
            shape.push_back(cycle[i]);
        }

        for (Vector2 coord : shape) {
            cout << "(" << coord.x << ", " << coord.y << ")" << endl;
        }
        cout << endl;



        // (DONE) Simulate the movement of the snake
        // (DONE) Backtrack in the cycle from the head of the snake until you reach the second compartment of the snake, meanwhile add all those vertices to a list
        // Make a function to determine which adjacent points to use to attempt to patch the cycle
        // function does the following: 1. creates a list of the points adjacent to the points in the cycle
        // 2. Iterates over each of these points, checking to see if the next in the cycle is in the list of adjacent points, make a list out of the candidate pairs
        // 3. bonus: make it attempt several different pairs
        // Give those vertices and the start and end vertices to the findHamiltonianPath function
        // If it cannot find a path with those vertices, try another pair

        // The new graph should patch a section that the snake will not use for as long as possible, run checks to even see if it is safe for the cycle to be patched that way

        // If operations take too long, shut it down and just follow the cycle
    }

    return Vector2{0, 0};
}


bool Snake::IsOccupied(const Vector2 pos) const {
    return occupied.find(pos) != occupied.end();
}


bool Snake::HasFreeSpaceAhead(Vector2 pos) {
    for (auto i = cycle.begin(); i != cycle.end(); ++i) {
        if (*i == pos) {
            int currentIdx = distance(cycle.begin(), i);
            int cycleSize = cycle.size();
            
            for (int j = 1; j <= 3; ++j) {
                int nextIdx = (currentIdx + j) % cycleSize;
                
                if (IsOccupied(cycle[nextIdx])) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}


int Snake::StepsBetween(Vector2 start, Vector2 end) {
    auto itStart = find(cycle.begin(), cycle.end(), start);
    auto itEnd = find(cycle.begin(), cycle.end(), end);

    int idxStart = distance(cycle.begin(), itStart);
    int idxEnd = distance(cycle.begin(), itEnd);

    int steps = idxEnd - idxStart;
    if (steps < 0) steps += (int)cycle.size();
    

    return steps;
}


deque <Vector2> Snake::GetCycle() {
    deque <Vector2> cycle = {};
    deque <Vector2> row = {};
    int w = gridWidth;
    int h = gridHeight;
    bool flipCycle = false;
    static std::mt19937 rng(std::random_device{}());

    if (w % 2 == 0 && h % 2 == 0) { // Both width and height are even
        flipCycle = std::uniform_int_distribution<int>(0, 1)(rng);
    }
    else if (w % 2 == 0) { // If only width is even, force height-based
        flipCycle = false;
    }
    else if (h % 2 == 0) { // If only height is even, force width-based
        flipCycle = true;
    }
    else { // Both odd, working algorhithm not implemented yet
        cout << "Invalid width and height values" << endl;
        return cycle;
    }


    bool mirrorCycle = std::uniform_int_distribution<int>(0, 1)(rng);
    bool reverseCycle = std::uniform_int_distribution<int>(0, 1)(rng);

    int outer = flipCycle ? h : w;
    int inner = flipCycle ? w : h;


    for (int outerIdx = 0; outerIdx < outer; ++outerIdx) {
        for (int innerIdx = 0; innerIdx < inner - 1; ++innerIdx) {
            Vector2 v;
            if (flipCycle)
                v = {(float)innerIdx, (float)outerIdx}; // height-based
            else
                v = {(float)outerIdx, (float)innerIdx}; // width-based

            if (mirrorCycle) {
                v.x = (w - 1) - v.x;
                v.y = (h - 1) - v.y;
            }

            row.push_front(v);
        }

        // Reverse every other row
        if (outerIdx % 2 == 1) {
            reverse(row.begin(), row.end());
        }

        cycle.insert(cycle.end(), row.begin(), row.end());
        row.clear();
    }

    // Add the final edge along the last row/column
    for (int i = (flipCycle ? h : w) - 1; i >= 0; --i) {
        Vector2 v;

        if (flipCycle)
            v = {float(inner - 1), (float)i}; // height-based
        else
            v = {(float)i, float(inner - 1)}; // width-based

        if (mirrorCycle) {
            v.x = (w - 1) - v.x;
            v.y = (h - 1) - v.y;
        }

        cycle.push_back(v);
    }

    if (reverseCycle) {
            reverse(cycle.begin(), cycle.end());
    }

    return cycle;
}


Graph Snake::BuildGraph(vector <Vector2> shape) {
    Graph g;

    // Add vertices
    for (Vector2 v : shape) {
        g.AddVertex(v);
    }
    
    // Add edges
    for (Vector2 v : shape) {
        Vector2 directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        
        for (Vector2 direction : directions) {
            Vector2 neighbor = Vector2Add(v, direction);

            if (g.HasVertex(neighbor)) {
                g.AddEdge(v, neighbor);
            }
        }
    }
    
    return g;
}


deque <Vector2> Snake::FindHamiltonianPath(Graph g, Vector2 start, Vector2 end) {

    if (g.IsValidGraph(start, end)) { // Running pruning before the search to rule out some graphs that cannot have a hamiltonian cycle.

        Vector2 current = start;
        int totalVertices = g.vertices.size();
        deque <Vector2> visited = {start};
        deque <Vector2> lastDecisionPoints = {start};
        map <Vector2, vector <Vector2>, Graph::Vector2Compare> availableMoves;

        while (visited.size() < totalVertices || current != end) {

            if (availableMoves.find(current) == availableMoves.end()) { // Compute available moves for coordinate if not already done.
                availableMoves[current] = g.GetValidNeighbors(current, visited, totalVertices, end);
            }

            vector <Vector2>& moves = availableMoves[current]; // This is a reference and not a copy because of the "&" symbol.

            cout << "current: " << "(" << current.x << ", " << current.y << ")" << " Available: ";
            for (Vector2 move : moves) {
                cout << "(" << move.x << ", " << move.y << ") ";
            }
            cout << endl;

            if (!moves.empty()) {
                int AmountOfValidMoves = moves.size();

                if (AmountOfValidMoves >= 2) {
                    lastDecisionPoints.push_back(current);
                }
                
                int fewestValidNeighbors = INT_MAX;
                Vector2 bestMove = {};
                int bestMoveIdx = -1;

                for (int i = 0; i < moves.size(); i++) {
                    int ValidNeighborsCount = g.GetAmountOfValidNeighbors(moves[i], visited, totalVertices, end);

                    if (ValidNeighborsCount < fewestValidNeighbors) {
                        fewestValidNeighbors = ValidNeighborsCount;
                        bestMove = moves[i];
                        bestMoveIdx = i;
                    }
                }

                moves.erase(availableMoves[current].begin() + bestMoveIdx);
                visited.push_back(bestMove);
                current = bestMove;
            }
            else {

                if (visited.size() <= 1) {
                    cout << "No Hamiltonian path found!" << endl;
                    return {};
                }
                
                cout << "Backtracking from (" << current.x << ", " << current.y << ")" << endl;

                // Remove data on the tiles that we are backtracking past while going to the last decision point.
                while (visited.back() != lastDecisionPoints.back()) {
                    cout << "Erasing: " << "(" << visited.back().x << ", " << visited.back().y << ") " << endl;

                    availableMoves.erase(visited.back());
                    visited.pop_back();
                }

                current = lastDecisionPoints.back();
                lastDecisionPoints.pop_back();
            }
        }

        return visited;
    }

    cout << "No hamiltonian cycle possible, determined via pruning" << endl;
    return {};
}