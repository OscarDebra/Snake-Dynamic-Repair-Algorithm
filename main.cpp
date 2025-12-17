#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"
#include <iostream>
#include <deque>
#include <ctime>
#include "Variables/Variables.h"
#include "GameClass/GameClass.h"

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
    float speed = 0.05f;
    Game game;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_UP) && speed > 0.0f) {
            speed -= 0.05f;
        }
        else if (IsKeyPressed(KEY_DOWN) && speed < 1.0f) {
            speed += 0.05f;
        };

        ready = Await(speed);

        if (ready) {
            game.Update();
        }

        BeginDrawing();
        game.Draw(speed);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
