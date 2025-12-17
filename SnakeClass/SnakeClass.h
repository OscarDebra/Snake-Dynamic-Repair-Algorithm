#pragma once
#include <deque>
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"

using namespace std;

class Snake {
public:
    deque<Vector2> cycle;
    deque<Vector2> body;
    Vector2 direction;
    bool addSegment;
    int input;
    int cycleRowWidth;

    static deque<Vector2> SetInitialBody();
    Snake();
    Vector2 CenterOf(Vector2 seg);
    Vector2 CornerOf(Vector2 seg);
    void Draw();
    void Update();
    void Grow();
    void Reset();
    Vector2 GenerateMove(Vector2 foodPos);
    deque<Vector2> GetCycle();
    bool IsOccupied(const Vector2 pos);
    bool HasFreeSpaceAhead(Vector2 pos);
    int StepsBetween(Vector2 start, Vector2 end);
    Vector2 CanCutForward();
    Vector2 CutForward(Vector2 dir, Vector2 pos, Vector2 foodPos);
};