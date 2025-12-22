#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"
#include <iostream>
#include <deque>
#include <ctime>
#include "Variables/Variables.h"
#include "GameClass/GameClass.h"

using namespace std;


bool IsReady(double interval) {
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
    int gameState = 0;
    float interval = 0.2; 
    Game game;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_UP) && gameState < 3) {
            gameState += 1;
        }
        else if (IsKeyPressed(KEY_DOWN) && gameState > 0) {
            gameState -= 1;
        };

        switch(gameState) {
            case 0: // Manual speed
                if (IsKeyPressed(KEY_RIGHT)) {
                    ready = true;
                }
                else {
                    ready = false;
                }
                break;
            case 1: // 5mps
                interval = 0.2;
                ready = IsReady(interval);
                break;
            case 2: // 20mps
                interval = 0.05;
                ready = IsReady(interval);
                break;
            case 3: // WARP
                interval = 0;
                ready = IsReady(interval);
                break;
        };

        if (ready) {
            game.Update();
        }

        BeginDrawing();
        game.Draw(gameState, horizontalGamePadding, windowWidth);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
