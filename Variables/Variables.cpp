#include "Variables.h"

Color backgroundColor = {0, 0, 0, 255};
Color cycleColor = {200, 196, 0, 255};
Color snakeColor = {0, 0, 170, 255};
Color foodColor = { 255, 0, 0, 255};
Color textColor = {204, 204, 204, 255};
Color borderColor = {80, 180, 80, 255};

int gridWidth  = 30;
int gridHeight = 30;
double lastUpdateTime = 0;

int minHorizontalGamePadding = 75;
int verticalGamePadding = 75;


int heightBasedCellSize = (800 - verticalGamePadding*2) / gridHeight;
int widthBasedCellSize  = (1500 - minHorizontalGamePadding*2) / gridWidth;
int cellSize = (heightBasedCellSize < widthBasedCellSize ? heightBasedCellSize : widthBasedCellSize); // Declare cellsize based on how small it needs to be to fit along the tightest axis.

int pixelGridWidth = gridWidth*cellSize; // The width of the playing field in pixels
int pixelGridHeight = gridHeight*cellSize; // The height of the playing field in pixels

int windowHeight = verticalGamePadding*2 + pixelGridHeight;


int getWindowWidth() {
    int windowWidth = minHorizontalGamePadding + pixelGridWidth;
    int minWindowWidth = 500; 

    if (windowWidth < minWindowWidth) { // Window width is 500px minimum, recalculate the padding if the board is too skinny so it stays centered
        return minWindowWidth;
    }

    return windowWidth;
};


int getHorizontalGamePadding(int windowWidth) {
    return windowWidth/2 - pixelGridWidth/2;
};
