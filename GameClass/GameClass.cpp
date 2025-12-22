#include "GameClass.h"
#include <iostream>
#include <deque>
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"

using namespace std;



void Game::Draw(int gameState, int horizontalGamePadding, int windowWidth) { // Don't even try to change the order
    ClearBackground(backgroundColor);

    // DrawGrid(horizontalGamePadding);

    if (!won) food.Draw(horizontalGamePadding);
    snake.Draw(horizontalGamePadding);
    if (won) DrawWinScreen(horizontalGamePadding);

    DrawCycle(horizontalGamePadding);
    DrawBorder(horizontalGamePadding);

    DrawUI(gameState, horizontalGamePadding, windowWidth);
}


void Game::DrawCycle(int horizontalGamePadding) const {
    deque<Vector2> cycle = snake.cycle;
    
    if (!cycle.empty()) {
        // Draw lines between consecutive vertices
        for (size_t i = 0; i < cycle.size() - 1; i++) {
            Vector2 start = {
                horizontalGamePadding + cycle[i].x * cellSize + cellSize / 2,
                verticalGamePadding + cycle[i].y * cellSize + cellSize / 2
            };
            Vector2 end = {
                horizontalGamePadding + cycle[i + 1].x * cellSize + cellSize / 2,
                verticalGamePadding + cycle[i + 1].y * cellSize + cellSize / 2
            };
            DrawLineV(start, end, cycleColor);
        }

        // Draw last edge
        Vector2 lastVertex = {
            horizontalGamePadding + cycle.back().x * cellSize + cellSize / 2,
            verticalGamePadding + cycle.back().y * cellSize + cellSize / 2
        };
        Vector2 firstVertex = {
            horizontalGamePadding + cycle.front().x * cellSize + cellSize / 2,
            verticalGamePadding + cycle.front().y * cellSize + cellSize / 2
        };
        DrawLineV(lastVertex, firstVertex, cycleColor);
    }
}

void Game::DrawGrid (int horizontalGamePadding) const {
        for (int x = horizontalGamePadding; x <= pixelGridWidth + horizontalGamePadding; x += cellSize) { // vertical lines
            DrawLine(x, verticalGamePadding, x, pixelGridHeight + verticalGamePadding, borderColor);
        }
        for (int y = verticalGamePadding; y <= pixelGridHeight + verticalGamePadding; y += cellSize) { // horizontal lines
            DrawLine(horizontalGamePadding, y, pixelGridWidth + horizontalGamePadding, y, borderColor);
        }
}


void Game::DrawUI(int gameState, int horizontalGamePadding, int windowWidth) const {
    int offset = 15; // Offset of text from the edge of the screen
    int textSize = 40;

    int textWidth = MeasureText("Algorithm plays snake", textSize);
    DrawText("Algorithm plays snake", (windowWidth - textWidth)/2, offset, textSize, textColor);

    DrawText(TextFormat("%i", score), offset, verticalGamePadding*2 + cellSize*gridHeight - textSize - offset, textSize, textColor);

    textWidth = MeasureText("Speed: 20mps", 40);

    switch(gameState) {
        case 0:
            DrawText(TextFormat("Speed: MAN"), horizontalGamePadding*2 + gridWidth*cellSize - textWidth - offset, verticalGamePadding*2 + cellSize*gridHeight -textSize - offset, textSize, textColor);
            break;
        case 1:
            DrawText(TextFormat("Speed: 5mps"), horizontalGamePadding*2 + gridWidth*cellSize - textWidth - offset, verticalGamePadding*2 + cellSize*gridHeight -textSize - offset, textSize, textColor);
            break;
        case 2:
            DrawText(TextFormat("Speed: 20mps"), horizontalGamePadding*2 + gridWidth*cellSize - textWidth - offset, verticalGamePadding*2 + cellSize*gridHeight -textSize - offset, textSize, textColor);
            break;
        case 3:
            DrawText(TextFormat("Speed: WARP"), horizontalGamePadding*2 + gridWidth*cellSize - textWidth - offset, verticalGamePadding*2 + cellSize*gridHeight -textSize - offset, textSize, textColor);
            break;
    }
}

void Game::DrawWinScreen(int horizontalGamePadding) const {
    const char* msg = "You win!";
    int fontSize = 100;
    Vector2 textSize = MeasureTextEx(GetFontDefault(), msg, fontSize, 0);
    float x = horizontalGamePadding + (cellSize * gridWidth) / 2.0f - textSize.x / 2.0f;
    float y = verticalGamePadding + (cellSize * gridHeight) / 2.0f - textSize.y / 2.0f;
    DrawText(msg, (int)x, (int)y, fontSize, cycleColor);
}

void Game::DrawBorder(int horizontalGamePadding) const {
    float lineThickness = 1;

    float horizontalOffset = horizontalGamePadding - lineThickness;
    float verticalOffset = verticalGamePadding - lineThickness;

    float borderWidth = pixelGridWidth + lineThickness*2;
    float borderHeight = pixelGridHeight + lineThickness*2;

    Rectangle rect = {horizontalOffset, verticalOffset, borderWidth, borderHeight};
    DrawRectangleLinesEx(rect, lineThickness, borderColor);
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
