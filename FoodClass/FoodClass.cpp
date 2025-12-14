#include "FoodClass.h"
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include "../Variables/Variables.h"
#include <deque>

using std::deque;


static bool ElementInDeque(Vector2 element, const deque<Vector2>& dq) {
    for (size_t i = 0; i < dq.size(); ++i) {
        if (Vector2Equals(dq[i], element)) return true;
    }
    return false;
}


Food::Food() {
    // default placeholder position
    position = GetRandomPos();
}


Food::Food(const deque<Vector2> snakeBody) {
    position = GetFoodPos(snakeBody);
}


void Food::Draw() {
    float scale = cellSize - cellSize / 8.0f;
    Vector2 pos = {
        gamePadding + position.x * cellSize + cellSize / 16.0f,
        gamePadding + position.y * cellSize + cellSize / 16.0f
    };

    DrawRectangle(pos.x, pos.y, scale, scale, red);
}


Vector2 Food::GetRandomPos() {
    int x = GetRandomValue(0, (int)gridWidth - 1);
    int y = GetRandomValue(0, (int)gridHeight - 1);
    return Vector2{(float)x, (float)y};
}


Vector2 Food::GetFoodPos(deque<Vector2> snakeBody) {
    Vector2 pos = GetRandomPos();
    while (ElementInDeque(pos, snakeBody)) {
        pos = GetRandomPos();
    }
    return pos;
}
