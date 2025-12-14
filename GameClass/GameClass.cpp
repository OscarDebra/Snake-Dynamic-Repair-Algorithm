#include "GameClass.h"
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include <iostream>

using std::deque;

bool ElementInDeque(Vector2 element, const deque<Vector2> & dq) {
    for (unsigned int i = 0; i < dq.size(); i++) {
        if (Vector2Equals(dq[i], element)) return true;
    }
    return false;
}


Game::Game() {
    Snake snake;
    Food food;
    won = false;
    score = 0;
    winTime = 0.0;

    wins = 0;
    losses = 0;
    moves = 0;
}


void Game::Draw() {
    if (!won) food.Draw();
    snake.Draw();

    if (won) {
        const char* msg = "You win!";
        int fontSize = 100;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), msg, fontSize, 0);
        float x = gamePadding + (cellSize * gridWidth) / 2.0f - textSize.x / 2.0f;
        float y = gamePadding + (cellSize * gridHeight) / 2.0f - textSize.y / 2.0f;
        DrawText(msg, (int)x, (int)y, fontSize, yellow);
    }
}


void Game::Update() {
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
        CheckSelfCollision();
    }
}


void Game::CheckFoodCollision() {
    if (Vector2Equals(snake.body[0], food.position)) {
        snake.addSegment = true;
        score++;

        if ((int)snake.body.size() == (int)(gridWidth * gridHeight)) {
            Win();
        } else {
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


void Game::CheckSelfCollision() {
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
