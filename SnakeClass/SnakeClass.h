#pragma once
#include <deque>
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"
#include <unordered_set>
#include "../GraphClass/GraphClass.h"

using namespace std;

class Snake {
private:
    struct Vector2Hash {
        size_t operator()(const Vector2& v) const {
            return hash <int>()(v.x) ^ (hash <int>()(v.y) << 1);
        }
    };


public:
    deque<Vector2> cycle = GetCycle();
    deque<Vector2> body = SetInitialBody();
    unordered_set <Vector2, Vector2Hash> occupied;
    Vector2 direction = {1, 0};
    bool addSegment = false;

    Snake() = default;

    static deque<Vector2> SetInitialBody();
    Vector2 CenterOf(Vector2 seg, int horizontalGamePadding);
    Vector2 CornerOf(Vector2 seg, int horizontalGamePadding);
    void Draw(int horizontalGamePadding);
    void Update();
    void Grow();
    void Reset();
    Vector2 GenerateMove(Vector2 foodPos);
    Vector2 GetCuttingDir(Vector2 current, Vector2 dirToFoodX, Vector2 dirToFoodY);
    deque<Vector2> GetCycle();
    bool IsOccupied(const Vector2 pos) const;
    bool HasFreeSpaceAhead(Vector2 pos);
    int StepsBetween(Vector2 start, Vector2 end);
    Graph BuildGraph(vector <Vector2> shape);
    deque<Vector2> FindHamiltonianPath(Graph g, Vector2 start, Vector2 end);
};