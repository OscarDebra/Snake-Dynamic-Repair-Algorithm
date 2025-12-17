#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include <iostream>
#include <deque>
#include <ctime>
#include "Variables/Variables.h"
#include "SnakeClass/SnakeClass.h"
#include "FoodClass/FoodClass.h"
#include "GameClass/GameClass.h"
#include "GraphClass/GraphClass.h"

using namespace std;


bool Await(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}


int main() {
    InitWindow(gamePadding*2 + cellSize*gridWidth, gamePadding*2 + cellSize*gridHeight, "Snake game");
    SetTargetFPS(10000);
    bool ready = false;
    float speed = 0.01f;
    Game game;

    while (!WindowShouldClose()) {
        deque <Vector2> cycle = game.snake.cycle;
        BeginDrawing();

        if (IsKeyPressed(KEY_UP)) {
            speed += 0.1f;
        };
        if (IsKeyPressed(KEY_DOWN)) {
            speed -= 0.1f;
        };

        ready = Await(speed);

        if (ready) {
            game.Update();
        }

        game.Draw();
        EndDrawing();

    }

    CloseWindow();
    return 0;
}
