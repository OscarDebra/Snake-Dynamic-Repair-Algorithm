#include "Variables.h"

Color black = {0, 0, 0, 255};
Color yellow = {255, 251, 0, 255};
Color green = {80, 180, 80, 255 };
Color darkGreen = {6, 50, 6, 255};
Color red = { 255, 0, 0, 255};

int gridWidth  = 30;
int gridHeight = 30;
double lastUpdateTime = 0;
int gamePadding = 75;

int windowHeight = (800 - gamePadding * 2) / gridHeight;
int windowWidth  = (1400 - gamePadding * 2) / gridWidth;
int cellSize = (windowHeight < windowWidth ? windowHeight : windowWidth);