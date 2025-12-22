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
    bool won = false;
    int score = 0;
    double winTime;
    int wins = 0;
    int losses = 0;
    int moves = 0;

    Game() = default; // Nothing to construct

    void Draw(int gameState, int horizontalGamePadding, int windowWidth);
    void DrawCycle(int horizontalGamePadding) const;
    void DrawGrid(int horizontalGamePadding) const;
    void DrawUI(int gameState, int horizontalGamePadding, int windowWidth) const;
    void DrawWinScreen(int horizontalGamePadding) const;
    void DrawBorder(int horizontalGamePadding) const;
    void Update();
    void CheckFoodCollision();
    void CheckEdgeCollision();
    void CheckSnakeCollision();
    void GameOver();
    void NewGame();
    void Win();
};