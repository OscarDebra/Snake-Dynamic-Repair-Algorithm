#pragma once
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raylib.h"

extern Color backgroundColor;
extern Color cycleColor;
extern Color snakeColor;
extern Color darkGreen;
extern Color foodColor;
extern Color textColor;
extern Color borderColor;

extern int gridWidth;
extern int gridHeight;
extern double lastUpdateTime;

extern int verticalGamePadding;

extern int cellSize;

extern int pixelGridWidth;
extern int pixelGridHeight;

extern int windowHeight;

int getWindowWidth();
int getHorizontalGamePadding(int windowWidth);
