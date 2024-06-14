#include "raylib.h"
#include "math.h"
#define RL_PLOT_IMPLEMENTATION
#include "rplot.h"
#define SIZE 100

int main(void)
{
    const int windowWidth = 900;
    const int windowHeight = 700;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "ScatterPlot2D example");
    SetTargetFPS(30);

    Figure2D *fig = LoadFigure2D((Rectangle){.x = 0, .y = 0, .width = windowWidth, .height = windowHeight}, "ScatterPlot2D example");
    float datax[SIZE];
    float datay[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        datax[i] = GetRandomValue(0, 100);
        datay[i] = GetRandomValue(0, 100);
    }
    ScatterPlot2DEx(fig, datax, datay, SIZE, 5.0, DARKPURPLE);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFigure2D(fig);
        EndDrawing();
    }

    UnloadFigure2D(fig);
    CloseWindow();

    return 0;
}