#include "raylib.h"
#include "math.h"
#define RL_PLOT_IMPLEMENTATION
#include "rplot.h"
#include "RayPlot_CustomFont.h"
#define SIZE 15

// Basically the two functions produce the same plot
// Only with low data count are they different, because
// Contourf also interpolates the color between datapoints, that means
// it does much more calculations, so with high datacount it is better to
// use HeatMap

int main(void)
{
    const int windowWidth = 1200;
    const int windowHeight = 600;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "HeatMap2D and Contourf example");
    SetTargetFPS(30);
    Font customFont = LoadFont_RayPlot();

    Rectangle heatmapRect = (Rectangle)(Rectangle){.x = 0, .y = 0, .width = windowWidth / 2.0 - RP_EPS, .height = windowHeight};
    Rectangle contourfRect = (Rectangle)(Rectangle){.x = windowWidth / 2.0 + RP_EPS, .y = 0, .width = windowWidth / 2.0 - RP_EPS, .height = windowHeight};
    Figure2D *heatmapFig = LoadFigure2D(heatmapRect, "HeatMap example", customFont);
    Figure2D *contourfFig = LoadFigure2D(contourfRect, "Contourf example", customFont);

    float *data = (float *)calloc(SIZE * SIZE, sizeof(float));
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            data[RowMajorIndex(i, j, SIZE, SIZE)] = sinf((float)j) + cosf((float)i) * expf((float)j * 0.1f);
        }
    }
    HeatMap2DEx(heatmapFig, (float *)data, SIZE, SIZE, ColorMapStyle_YELLOW_DARKPURPLE, true);
    Contourf2DEx(contourfFig, (float *)data, SIZE, SIZE, ColorMapStyle_YELLOW_DARKPURPLE, true);
    free(data);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFigure2D(contourfFig);
        DrawFigure2D(heatmapFig);
        EndDrawing();
    }
    UnloadFigure2D(heatmapFig);
    UnloadFigure2D(contourfFig);
    CloseWindow();

    return 0;
}
