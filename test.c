#include "raylib.h"
#include "math.h"
#define RL_PLOT_IMPLEMENTATION
#include "rplot.h"
#include "RayPlot_CustomFont.h"
#define SIZE 100

int main(void)
{
    const int windowWidth = 800;
    const int windowHeight = 500;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "Testing");
    SetTargetFPS(30);

    Font customfont = LoadFont_RayPlot();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextEx(customfont, "This is customfont with and fontsize 14", (Vector2){50, 200}, 14.0, 0.0, BLACK);
        DrawTextEx(customfont, "This is customfont with and fontsize 16", (Vector2){50, 300}, 16.0, 0.0, BLACK);
        DrawTextEx(customfont, "This is customfont with and fontsize 24", (Vector2){50, 400}, 24.0, 0.0, BLACK);
        DrawTextEx(customfont, "This is customfont with and fontsize 32", (Vector2){50, 100}, 32.0, 0, BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}