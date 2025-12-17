#include "GameClass.h"
#include <iostream>
#include <deque>

using namespace std;

void Game::Draw(float speed) { // Don't even try to change the order
    ClearBackground(black);

    if (!won) food.Draw();
    snake.Draw();
    if (won) DrawWinScreen();

    DrawCycle();
    // DrawGrid();

    DrawUI(speed);
}


void Game::DrawCycle() const {
    deque<Vector2> cycle = snake.cycle;
    
    if (!cycle.empty()) {
        // Draw lines between consecutive vertices
        for (size_t i = 0; i < cycle.size() - 1; i++) {
            Vector2 start = {
                gamePadding + cycle[i].x * cellSize + cellSize / 2,
                gamePadding + cycle[i].y * cellSize + cellSize / 2
            };
            Vector2 end = {
                gamePadding + cycle[i + 1].x * cellSize + cellSize / 2,
                gamePadding + cycle[i + 1].y * cellSize + cellSize / 2
            };
            DrawLineV(start, end, darkGreen);
        }

        // Draw last edge
        Vector2 lastVertex = {
            gamePadding + cycle.back().x * cellSize + cellSize / 2,
            gamePadding + cycle.back().y * cellSize + cellSize / 2
        };
        Vector2 firstVertex = {
            gamePadding + cycle.front().x * cellSize + cellSize / 2,
            gamePadding + cycle.front().y * cellSize + cellSize / 2
        };
        DrawLineV(lastVertex, firstVertex, red);
    }
}

void Game::DrawGrid () const {
        for (int x = gamePadding; x <= gridWidth*cellSize + gamePadding; x += cellSize) { // vertical lines
            DrawLine(x, gamePadding, x, gridHeight*cellSize + gamePadding, darkGreen);
        }
        for (int y = gamePadding; y <= gridHeight*cellSize + gamePadding; y += cellSize) { // horizontal lines
            DrawLine(gamePadding, y, gridWidth*cellSize + gamePadding, y, darkGreen);
        }
}


void Game::DrawUI(float speed) const {
    DrawText("AI plays snake", gamePadding, gamePadding - 50, 40, green);

    DrawText(TextFormat("%i", score), gamePadding, gamePadding + cellSize*gridHeight + 10, 40, green);

    int textWidth = MeasureText("Speed: 20.00", 40);
    DrawText(TextFormat("Speed: %s", speed > 0.01 ? TextFormat("%.2f", 1/speed) : "WARP"), gamePadding + gridWidth*cellSize - textWidth, gamePadding + cellSize*gridHeight + 10, 40, green);
}

void Game::DrawWinScreen() const {
    const char* msg = "You win!";
    int fontSize = 100;
    Vector2 textSize = MeasureTextEx(GetFontDefault(), msg, fontSize, 0);
    float x = gamePadding + (cellSize * gridWidth) / 2.0f - textSize.x / 2.0f;
    float y = gamePadding + (cellSize * gridHeight) / 2.0f - textSize.y / 2.0f;
    DrawText(msg, (int)x, (int)y, fontSize, yellow);
}


void Game::Update() {
    snake.direction = snake.GenerateMove(food.position);

    if (won) {
        if (GetTime() - winTime >= 3.0) {
            NewGame();
            won = false;
        }
        return;
    }
    else {
        moves++;
        snake.Update();
        CheckFoodCollision();
        snake.Grow();
        CheckEdgeCollision();
        CheckSnakeCollision();
    }
}


void Game::CheckFoodCollision() {
    if (Vector2Equals(snake.body[0], food.position)) {
        snake.addSegment = true;
        score++;

        if ((int)snake.body.size() == (int)(gridWidth * gridHeight)) {
            Win();
        } 
        else {
            food.position = food.GetFoodPos(snake.body);
        }
    }
}


void Game::CheckEdgeCollision() {
    Vector2 head = snake.body[0];
    if (head.x < 0 || head.x >= gridWidth || head.y < 0 || head.y >= gridHeight) {
        GameOver();
    }
}


void Game::CheckSnakeCollision() {
    deque<Vector2> headlessBody = snake.body;
    if (!headlessBody.empty()) headlessBody.pop_front();
    if (ElementInDeque(snake.body[0], headlessBody)) {
        GameOver();
    }
}


void Game::GameOver() {
    losses++;
    cout << "Loss x" << losses << endl;
    moves = 0;
    NewGame();
}


void Game::NewGame() {
    snake.Reset();
    food.position = food.GetFoodPos(snake.body);
    lastUpdateTime = 0;
    score = 0;
    won = false;
}


void Game::Win() {
    wins++;
    cout << "Win x" << wins << " moves: " << moves << endl;
    moves = 0;
    winTime = GetTime();
    won = true;
}
