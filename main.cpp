#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include <iostream>
#include <deque>
#include <ctime>
#include "Variables/Variables.h"
#include "SnakeClass/SnakeClass.h"
#include "FoodClass/FoodClass.h"
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
            game.snake.direction = game.snake.GenerateMove(game.food.position);
            game.Update();
        }

        ClearBackground(black);

        // Code to make a world grid
        for (int x = gamePadding; x <= gridWidth*cellSize + gamePadding; x += cellSize) { // vertical lines
            DrawLine(x, gamePadding, x, gridHeight*cellSize + gamePadding, darkGreen);
        }
        for (int y = gamePadding; y <= gridHeight*cellSize + gamePadding; y += cellSize) { // horizontal lines
            DrawLine(gamePadding, y, gridWidth*cellSize + gamePadding, y, darkGreen);
        }


        // Code to show the active path
        // if (!cycle.empty()) {
        //     for (size_t i = 0; i < cycle.size() - 1; i++) {
        //         Vector2 start = {
        //             gamePadding + cycle[i].x * cellSize + cellSize / 2,
        //             gamePadding + cycle[i].y * cellSize + cellSize / 2
        //         };
        //         Vector2 end = {
        //             gamePadding + cycle[i + 1].x * cellSize + cellSize / 2,
        //             gamePadding + cycle[i + 1].y * cellSize + cellSize / 2
        //         };
        //         DrawLineV(start, end, darkGreen);
        //     }
        // }
        

        //     Vector2 last = {
        //         gamePadding + cycle.back().x * cellSize + cellSize / 2,
        //         gamePadding + cycle.back().y * cellSize + cellSize / 2
        //     };
        //     Vector2 first = {
        //         gamePadding + cycle.front().x * cellSize + cellSize / 2,
        //         gamePadding + cycle.front().y * cellSize + cellSize / 2
        //     };
        //     DrawLineV(last, first, darkGreen);

        DrawText("AI plays snake", gamePadding, gamePadding - 50, 40, green);
        DrawText(TextFormat("%i", game.score), gamePadding, gamePadding + cellSize*gridHeight + 10, 40, green);

        game.Draw();
        EndDrawing();

    }

    CloseWindow();
    return 0;
}
