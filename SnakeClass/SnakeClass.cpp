#include "SnakeClass.h"
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include "../Variables/Variables.h"
#include <chrono>
#include <iostream>
#include <climits>
#include <random>


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


Snake::Snake() {
    body = SetInitialBody();
    direction = {1, 0};
    addSegment = false;

    cycle = GetCycle();
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
    float snakeThickness = cellSize*0.85f;

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
    body.push_front(Vector2Add(body[0], direction));
}


void Snake::Grow() {
    // if addSegment is false, pop back (no growth). If true, keep tail (grow).
    if (!addSegment) body.pop_back();
    addSegment = false;
}


void Snake::Reset() {
    body = SetInitialBody();
    direction = {1, 0};
    addSegment = false;

    cycle = GetCycle();
}


Vector2 Snake::GenerateMove(Vector2 foodPos) {
    Vector2 currentPos = body[0];

    Vector2 distanceToFood = foodPos - currentPos;

    bool moveOnX = fabs(distanceToFood.x) > 0.0f;
    bool moveOnY = fabs(distanceToFood.y) > 0.0f;

    Vector2 dirToFoodX = {distanceToFood.x > 0 ? 1.0f : -1.0f, 0.0f};
    Vector2 dirToFoodY = {0.0f, distanceToFood.y > 0 ? 1.0f : -1.0f};

    // Find current position in cycle
    auto currentIt = find(cycle.begin(), cycle.end(), currentPos);

    // Get the next position in the cycle
    auto nextIt = next(currentIt);
    if (nextIt == cycle.end()) nextIt = cycle.begin();
    Vector2 cycleDir = *nextIt - currentPos;

    // Avoid turning back on ourselves
    if (cycleDir + direction == Vector2{0, 0}) {
        auto nextNextIt = next(nextIt);
        if (nextNextIt == cycle.end()) nextNextIt = cycle.begin();
        cycleDir = *nextNextIt - currentPos;
    }

    return cycleDir;


    // Try to find a cutting move toward the food
    Vector2 bestMove = CutForward(direction, currentPos, foodPos);
    
    // If no valid cutting move found, follow the cycle
    if (bestMove == Vector2{0, 0}) {
        return cycleDir;
    }
    
    return bestMove;
}



bool Snake::IsOccupied(const Vector2 pos) {
    for (const auto segment : body) {
        if (segment == pos) return true;
    }
    return false;
}


bool Snake::HasFreeSpaceAhead(Vector2 pos) {
    for (auto i = cycle.begin(); i != cycle.end(); ++i) {
        if (*i == pos) {
            int currentIdx = distance(cycle.begin(), i);
            int cycleSize = cycle.size();
            
            for (int j = 1; j <= cycleRowWidth; ++j) {
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


Vector2 Snake::CutForward(Vector2 dir, Vector2 pos, Vector2 foodPos) {
    // Possible local directions relative to the current one
    Vector2 right = { -dir.y,  dir.x };  // rotate +90° (right)
    Vector2 left  = {  dir.y, -dir.x };  // rotate -90° (left)
    Vector2 forward = dir;

    vector<Vector2> moves = {forward, left, right};
    Vector2 bestDir = Vector2{0, 0};
    int bestSteps = INT_MAX; // Big number

    for (Vector2 move : moves) {
        Vector2 candPos = pos + move;

        // Check if the move goes backwards in the cycle
        if (StepsBetween( pos, foodPos) < StepsBetween(candPos, foodPos)) continue;

        // Check if the candidate position is in the cycle
        auto candIt = find(cycle.begin(), cycle.end(), candPos);
        if (candIt == cycle.end()) continue;

        // Check if we can continue forward from this position
        auto nextCandIt = next(candIt);
        if (nextCandIt == cycle.end()) nextCandIt = cycle.begin();
        Vector2 nextCandDir = *nextCandIt - candPos;

        // Don't turn back (next direction shouldn't be opposite to current move)
        if (move + nextCandDir == Vector2{0, 0}) continue;

        // Check if this position is safe (no snake body ahead)
        if (IsOccupied(candPos)) continue;

        // Calculate steps from this position to food
        int steps = StepsBetween(candPos, foodPos);

        // Prefer moves that MINIMIZE steps to food
        if (steps < bestSteps and HasFreeSpaceAhead(candPos)) {
            bestSteps = steps;
            bestDir = move;
        }
    }

    return bestDir;
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

    cycleRowWidth = inner;

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