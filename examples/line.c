#include "raylib.h"
#include "math.h"
#define RL_PLOT_IMPLEMENTATION
#include "rplot.h"
#define SIZE 100

int main(void)
{
    const int windowWidth = 1200;
    const int windowHeight = 600;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "LinePlot2D example");
    SetTargetFPS(30);

    Rectangle rect1 = (Rectangle)(Rectangle){.x = 0, .y = 0, .width = windowWidth / 2.0 - RP_EPS, .height = windowHeight};
    Rectangle rect2 = (Rectangle)(Rectangle){.x = windowWidth / 2.0 + RP_EPS, .y = 0, .width = windowWidth / 2.0 - RP_EPS, .height = windowHeight};
    Figure2D *fig1 = LoadFigure2D(rect1, "LinePlot2DEx example");
    Figure2D *fig2 = LoadFigure2D(rect2, "LinePlot2D example");
    float dataX[SIZE];
    float dataY[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        dataX[i] = Remap(i, 0, SIZE, 0, 2.0 * PI);
        dataY[i] = sinf(dataX[i]);
    }
    LinePlot2DEx(fig1, dataX, dataY, SIZE);
    LinePlot2D(fig2, dataY, SIZE);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFigure2D(fig1);
        DrawFigure2D(fig2);
        EndDrawing();
    }

    UnloadFigure2D(fig1);
    UnloadFigure2D(fig2);
    CloseWindow();

    return 0;
}
