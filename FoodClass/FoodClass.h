#pragma once
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"
#include <deque>

class Food {
public:
    Vector2 position;

    Food();
    Food(std::deque<Vector2> snakeBody);

    void Draw();
    Vector2 GetRandomPos();
    Vector2 GetFoodPos(std::deque<Vector2> snakeBody);
};