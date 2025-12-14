#pragma once
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"
#include <deque>
#include "../SnakeClass/SnakeClass.h"
#include "../FoodClass/FoodClass.h"
#include "../Variables/Variables.h"

using namespace std;

bool ElementInDeque(Vector2 element, const std::deque<Vector2>& dq);

class Game {
public:
    Snake snake;
    Food food;
    bool won;
    int score;
    double winTime;
    int wins;
    int losses;
    int moves;

    Game();

    void Draw();
    void Update();

    void CheckFoodCollision();
    void CheckEdgeCollision();
    void CheckSelfCollision();

    void GameOver();
    void NewGame();
    void Win();
};