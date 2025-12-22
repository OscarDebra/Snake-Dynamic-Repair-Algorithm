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
    int windowWidth = getWindowWidth();
    int horizontalGamePadding = getHorizontalGamePadding(windowWidth); // We declare widowWidth and horizontalGamePadding in main because there is some logic required in getting their value

    InitWindow(windowWidth, windowHeight, "Snake game");
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
        game.Draw(speed, horizontalGamePadding, windowWidth);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
