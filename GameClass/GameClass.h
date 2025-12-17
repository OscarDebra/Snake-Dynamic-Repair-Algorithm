#pragma once
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include <deque>
#include "../SnakeClass/SnakeClass.h"
#include "../FoodClass/FoodClass.h"
#include "../Variables/Variables.h"

using namespace std;

bool ElementInDeque(Vector2 element, const std::deque<Vector2>& dq);

class Game {
public:

    bool ElementInDeque(Vector2 element, const deque<Vector2> & dq) {
        for (unsigned int i = 0; i < dq.size(); i++) {
            if (Vector2Equals(dq[i], element)) return true;
        }
        return false;
    }

    Snake snake;
    Food food;
    bool won;
    int score;
    double winTime;
    int wins;
    int losses;
    int moves;

    Game() = default; // Nothing to construct

    void Draw(float speed);
    void DrawCycle() const;
    void DrawGrid() const;
    void DrawUI(float speed) const;
    void DrawWinScreen() const;
    void Update();
    void CheckFoodCollision();
    void CheckEdgeCollision();
    void CheckSnakeCollision();
    void GameOver();
    void NewGame();
    void Win();
};