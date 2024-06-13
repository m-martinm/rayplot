/// PROBLEM: Right now labels and ticks are stored inside of the struct, maybe provide them as arguments to the render functions
/// TODO: Implement ticks and put them on the texture so that we dont have to redraw them
/// TODO: Get rid of the defines like OFFSET and make it a setting in Figure2D
/// TODO: Keep track of how many plots are drawn onto a figure
/// TODO: Add a camera to the figures so that we can zoom and pan
/// TODO: Conturffel vmit csinálni mert nem az igazi, tul sok az interpolácio és nem néz ki túl jól
/// TODO: https://en.wikipedia.org/wiki/Marching_squares for contur lines
/// TODO: Start to implement 3D surface, scatter and other plots but for them probably custom shaders are required

/// RPAPI void UpdateTicks2D(Figure2D *fig, float *xData, float *yData, int dataSize)
/// {
///     int stepX = dataSize / DEFAULT_TICK_COUNT;
///     static char buf[32] = {0};
///     int cursorX = 0;
///     int cursorY = 0;
///     for (int i = 0; i < DEFAULT_TICK_COUNT; i++)
///     {
///         // memset(buf, 0, 32); ???
///         sprintf(buf, "%.2F\t", xData[i * stepX]);
///         TextAppend(fig->xTicks, buf, &cursorX);
///         sprintf(buf, "%f ", yData[i * stepX]);
///         TextAppend(fig->yTicks, buf, &cursorY);
///     }
///     float sp = 10.0 * fig->plot.bounds.width / MeasureTextEx(GetFontDefault(), fig->xTicks, 12.0, 10.0).x;
///     printf("%f\n", MeasureTextEx(GetFontDefault(), fig->xTicks, 12.0, sp).x);
///     printf("%f\n", MeasureTextEx(GetFontDefault(), fig->xTicks, 12.0, 10.0).x);
///     printf("%f\n", fig->plot.bounds.width);
/// }

#ifndef RL_PLOT_H
#define RL_PLOT_H

#ifdef RL_PLOT_NO_ASSERT
#define NDEBUG
#endif

#ifndef DEFAULT_LINE_WIDTH
#define DEFAULT_LINE_WIDTH 3.0f
#endif

#ifndef DEFAULT_PLOT_COLOR
#define DEFAULT_PLOT_COLOR BLUE
#endif

#ifndef DEFAULT_FONTSIZE
#define DEFAULT_FONTSIZE 24.0f
#endif

#ifndef DEFAULT_FONT_SPACING
#define DEFAULT_FONT_SPACING 1.0f
#endif

#ifndef DEFAULT_MARKERSIZE
#define DEFAULT_MARKERSIZE 3.0f
#endif

#ifndef DEFAULT_TEXT_COLOR
#define DEFAULT_TEXT_COLOR BLACK
#endif

#ifndef DEFAULT_TICK_COUNT
#define DEFAULT_TICK_COUNT 10
#endif

#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif
#ifndef RPAPI
#define RPAPI static inline
#endif
    //------------------------------------------------- Declartions -------------------------------------------------------

#define DEBUG_LOG(msg) printf("[DEBUG]: %s at line:%d in file: %s", msg, __LINE__, __FILE__)
#define MAX_STRING_SIZE 128
#define RP_EPS 2.0f
#define OFFSET_BOUNDS 50.0f

    typedef enum
    {
        ColorMapStyle_BLUE_RED,
        ColorMapStyle_WHITE_BLACK,
        ColorMapStyle_YELLOW_DARKPURPLE
    } ColorMapStyle;

    typedef struct
    {
        RenderTexture2D target;
        Rectangle bounds;
    } Plot2D;

    typedef struct
    {
        Plot2D plot;
        Rectangle area;
        char xLabel[MAX_STRING_SIZE];
        char yLabel[MAX_STRING_SIZE];
        char xTicks[MAX_STRING_SIZE];
        char yTicks[MAX_STRING_SIZE];
        char title[MAX_STRING_SIZE];
        Vector2 titlePosition;
        Vector2 xLabelPosition;
        Vector2 yLabelPosition;
        Vector2 xLim;
        Vector2 yLim;
    } Figure2D;

    // ------------ Basic Functions ------------

    RPAPI Figure2D *LoadFigure2D(Rectangle area, const char *title);
    RPAPI void UnloadFigure2D(Figure2D *fig);
    RPAPI void SetTitle(Figure2D *fig, const char *title);
    RPAPI void SetLabels(Figure2D *fig, const char *xLabel, const char *yLabel);
    RPAPI void SetLimits(Figure2D *fig, Vector2 xLim, Vector2 yLim);
    RPAPI void DrawFigure2D(Figure2D *fig);
    RPAPI void ClearFigure2D(Figure2D *fig);
    RPAPI void GridOn(Figure2D *fig);

    // ------------ Line Plot ------------

    RPAPI void LinePlot2DPro(Figure2D *fig, float *xData, float *yData, int dataSize, float LineWidth, Color color);
    RPAPI void LinePlot2DEx(Figure2D *fig, float *xData, float *yData, int dataSize);
    RPAPI void LinePlot2D(Figure2D *fig, float *yData, int dataSize);

    // ------------ Scatter PLot ------------

    RPAPI void ScatterPlot2DEx(Figure2D *fig, float *xData, float *yData, int dataSize, float markerSize, Color color);
    RPAPI void ScatterPlot2D(Figure2D *fig, float *xData, float *yData, int dataSize);

    // ------------ Stem Plot ------------

    RPAPI void StemPlot2DPro(Figure2D *fig, float *xData, float *yData, int dataSize, float markerSize, float lineWidth, Color color);
    RPAPI void StemPlot2DEx(Figure2D *fig, float *xData, float *yData, int dataSize);
    RPAPI void StemPlot2D(Figure2D *fig, float *yData, int dataSize);

    // ------------ HeatMap and Conturf ------------

    RPAPI void Contourf2DEx(Figure2D *fig, float *data, int rows, int cols, ColorMapStyle style, int rowMajor);
    RPAPI void Contourf2D(Figure2D *fig, float *data, int rows, int cols);
    RPAPI void HeatMap2DEx(Figure2D *fig, float *data, int rows, int cols, ColorMapStyle style, int rowMajor);
    RPAPI void HeatMap2D(Figure2D *fig, float *data, int rows, int cols);

    // ------------ Helper Functions ------------

    RPAPI Color InterpolateColorLinear(Color start, Color end, float factor);
    RPAPI int RowMajorIndex(int r, int c, int rows, int cols);
    RPAPI int ColMajorIndex(int r, int c, int rows, int cols);
    RPAPI Vector2 FindMinMax1D(float *data, int dataSize);
    RPAPI Vector2 FindMinMax2D(float *data, int rows, int cols, int rowMajor);

#ifdef __cplusplus
}
#endif
#endif // RL_PLOT_H

// ---------------------------------------------- Implementations -----------------------------------------------------
#ifdef RL_PLOT_IMPLEMENTATION

RPAPI Figure2D *LoadFigure2D(Rectangle area, const char *title)
{
    Figure2D *fig = (Figure2D *)RL_CALLOC(1, sizeof(Figure2D));
    fig->area = area;
    Plot2D p = {0};
    p.bounds = (Rectangle){.x = OFFSET_BOUNDS,
                           .y = OFFSET_BOUNDS,
                           .width = fig->area.width - 2.0f * OFFSET_BOUNDS,
                           .height = fig->area.height - 2.0f * OFFSET_BOUNDS};
    p.target = LoadRenderTexture(fig->area.width, fig->area.height);
    SetLabels(fig, "x-axis", "y-axis");
    fig->plot = p;
    SetTitle(fig, title);
    return fig;
}

RPAPI void UnloadFigure2D(Figure2D *fig)
{
    UnloadRenderTexture(fig->plot.target);
    RL_FREE(fig);
}

RPAPI void SetTitle(Figure2D *fig, const char *title)
{
    TextCopy(fig->title, title);
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), fig->title, DEFAULT_FONTSIZE, DEFAULT_FONT_SPACING);
    fig->titlePosition = (Vector2){.x = fig->area.x + fig->area.width / 2.0 - titleSize.x / 2.0,
                                   .y = fig->area.y + OFFSET_BOUNDS / 2.0 - titleSize.y / 2.0};
}

RPAPI void SetLabels(Figure2D *fig, const char *xLabel, const char *yLabel)
{
    TextCopy(fig->xLabel, xLabel);
    TextCopy(fig->yLabel, yLabel);
    Vector2 xLabelSize = MeasureTextEx(GetFontDefault(), fig->xLabel, DEFAULT_FONTSIZE, DEFAULT_FONT_SPACING);
    Vector2 yLabelSize = MeasureTextEx(GetFontDefault(), fig->yLabel, DEFAULT_FONTSIZE, DEFAULT_FONT_SPACING);
    fig->xLabelPosition = (Vector2){.x = fig->area.x + fig->area.width / 2.0f - xLabelSize.x / 2.0f,
                                    .y = fig->area.y + fig->area.height - OFFSET_BOUNDS / 2.0f - xLabelSize.y / 2.0f};
    fig->yLabelPosition = (Vector2){.x = OFFSET_BOUNDS / 2.0f - yLabelSize.y / 2.0f,
                                    .y = fig->area.y + fig->area.height / 2.0f + yLabelSize.x / 2.0f};
}

RPAPI void SetLimits(Figure2D *fig, Vector2 xLim, Vector2 yLim)
{
    fig->xLim = xLim;
    fig->yLim = yLim;
}

RPAPI void DrawFigure2D(Figure2D *fig)
{
    DrawTexture(fig->plot.target.texture, fig->area.x, fig->area.y, WHITE);
    DrawTextPro(GetFontDefault(), fig->title, fig->titlePosition, Vector2Zero(), 0.0f, DEFAULT_FONTSIZE, DEFAULT_FONT_SPACING, DEFAULT_TEXT_COLOR);
    DrawTextPro(GetFontDefault(), fig->xLabel, fig->xLabelPosition, Vector2Zero(), 0.0f, DEFAULT_FONTSIZE, DEFAULT_FONT_SPACING, DEFAULT_TEXT_COLOR);
    DrawTextPro(GetFontDefault(), fig->yLabel, fig->yLabelPosition, Vector2Zero(), -90.0f, DEFAULT_FONTSIZE, DEFAULT_FONT_SPACING, DEFAULT_TEXT_COLOR);
    const float thickness = 3.0f;
    DrawRectangleLinesEx((Rectangle){
                             fig->plot.bounds.x + fig->area.x - thickness,
                             fig->area.y + fig->plot.bounds.y - thickness,
                             fig->plot.bounds.width + 2.0f * thickness,
                             fig->plot.bounds.height + 2.0f * thickness},
                         thickness, DEFAULT_TEXT_COLOR);
}

RPAPI void ClearFigure2D(Figure2D *fig)
{
    BeginTextureMode(fig->plot.target);
    ClearBackground(RAYWHITE);
    EndTextureMode();
}

RPAPI void GridOn(Figure2D *fig)
{
    DEBUG_LOG("UNIMPLEMENTED!");
}

RPAPI void LinePlot2DEx(Figure2D *fig, float *xData, float *yData, int dataSize)
{
    LinePlot2DPro(fig, xData, yData, dataSize, DEFAULT_LINE_WIDTH, DEFAULT_PLOT_COLOR);
}

RPAPI void LinePlot2DPro(Figure2D *fig, float *xData, float *yData, int dataSize, float LineWidth, Color color)
{
    fig->xLim = FindMinMax1D(xData, dataSize);
    fig->yLim = FindMinMax1D(yData, dataSize);

    BeginTextureMode(fig->plot.target);
    for (int i = 0; i < dataSize - 1; i++)
    {
        Vector2 start = (Vector2){.x = Remap(xData[i], fig->xLim.x, fig->xLim.y, fig->plot.bounds.x, fig->plot.bounds.x + fig->plot.bounds.width),
                                  .y = Remap(yData[i], fig->yLim.x, fig->yLim.y, fig->plot.bounds.y, fig->plot.bounds.y + fig->plot.bounds.height)};
        Vector2 end = (Vector2){.x = Remap(xData[i + 1], fig->xLim.x, fig->xLim.y, fig->plot.bounds.x, fig->plot.bounds.x + fig->plot.bounds.width),
                                .y = Remap(yData[i + 1], fig->yLim.x, fig->yLim.y, fig->plot.bounds.y, fig->plot.bounds.y + fig->plot.bounds.height)};
        DrawLineEx(start, end, LineWidth, color);
    }
    EndTextureMode();
}

RPAPI void LinePlot2D(Figure2D *fig, float *yData, int dataSize)
{

    fig->xLim = (Vector2){.x = 0.0f, .y = dataSize};
    fig->yLim = FindMinMax1D(yData, dataSize);

    BeginTextureMode(fig->plot.target);
    for (int i = 0; i < dataSize - 1; i++)
    {
        Vector2 start = (Vector2){.x = Remap(i, 0.0f, dataSize, fig->plot.bounds.x, fig->plot.bounds.x + fig->plot.bounds.width),
                                  .y = Remap(yData[i], fig->yLim.x, fig->yLim.y, fig->plot.bounds.y, fig->plot.bounds.y + fig->plot.bounds.height)};
        Vector2 end = (Vector2){.x = Remap(i + 1, 0.0f, dataSize, fig->plot.bounds.x, fig->plot.bounds.x + fig->plot.bounds.width),
                                .y = Remap(yData[i + 1], fig->yLim.x, fig->yLim.y, fig->plot.bounds.y, fig->plot.bounds.y + fig->plot.bounds.height)};
        DrawLineEx(start, end, DEFAULT_LINE_WIDTH, DEFAULT_PLOT_COLOR);
    }
    EndTextureMode();
}

RPAPI void ScatterPlot2D(Figure2D *fig, float *xData, float *yData, int dataSize)
{
    ScatterPlot2DEx(fig, xData, yData, dataSize, DEFAULT_MARKERSIZE, DEFAULT_PLOT_COLOR);
}

RPAPI void ScatterPlot2DEx(Figure2D *fig, float *xData, float *yData, int dataSize, float markerSize, Color color)
{

    fig->xLim = FindMinMax1D(xData, dataSize);
    fig->yLim = FindMinMax1D(yData, dataSize);

    BeginTextureMode(fig->plot.target);
    for (int i = 0; i < dataSize - 1; i++)
    {
        DrawCircle(Remap(xData[i], fig->xLim.x, fig->xLim.y, fig->plot.bounds.x + RP_EPS, fig->plot.bounds.x + fig->plot.bounds.width - RP_EPS),
                   Remap(yData[i], fig->yLim.x, fig->yLim.y, fig->plot.bounds.y + RP_EPS, fig->plot.bounds.y + fig->plot.bounds.height - RP_EPS),
                   markerSize, color);
    }
    EndTextureMode();
}

RPAPI void StemPlot2DPro(Figure2D *fig, float *xData, float *yData, int dataSize, float markerSize, float lineWidth, Color color)
{

    fig->xLim = FindMinMax1D(xData, dataSize);
    fig->yLim = FindMinMax1D(yData, dataSize);

    BeginTextureMode(fig->plot.target);
    for (int i = 0; i < dataSize - 1; i++)
    {
        DrawCircle(Remap(xData[i], fig->xLim.x, fig->xLim.y, fig->plot.bounds.x + RP_EPS, fig->plot.bounds.x + fig->plot.bounds.width - RP_EPS),
                   Remap(yData[i], fig->yLim.x, fig->yLim.y, fig->plot.bounds.y + RP_EPS, fig->plot.bounds.y + fig->plot.bounds.height - RP_EPS),
                   markerSize, color);
    }
    EndTextureMode();
}

RPAPI void StemPlot2DEx(Figure2D *fig, float *xData, float *yData, int dataSize)
{
}

RPAPI void StemPlot2D(Figure2D *fig, float *yData, int dataSize)
{
}

RPAPI void Contourf2D(Figure2D *fig, float *data, int rows, int cols)
{
    Contourf2DEx(fig, data, rows, cols, ColorMapStyle_YELLOW_DARKPURPLE, 1);
}

RPAPI void Contourf2DEx(Figure2D *fig, float *data, int rows, int cols, ColorMapStyle style, int rowMajor)
{
    /// TODO: Maybe a better option would be to write a custom shader, that way
    ///       interpolating between colors would be maybe faster.
    ///       Also here and heatmao we dont set xLim and yLim because we only get
    ///       the 'z' data not the x,y the function can still be used, but its not really
    ///       a contourplot its more like an interpolated heatmap only.
    /// Indexing: x + N_x*y A[y][x] row major
    /// Indexing: y + N_y*(x-1) col major
    const float cellWidth = fig->plot.bounds.width / (cols - 1);
    const float cellHeight = fig->plot.bounds.height / (rows - 1);

    int (*indexer)(int, int, int, int);
    if (rowMajor)
    {
        indexer = &RowMajorIndex;
    }
    else
    {
        indexer = &ColMajorIndex;
    }
    Color start;
    Color end;
    switch (style)
    {
    case ColorMapStyle_WHITE_BLACK:
        start = WHITE;
        end = BLACK;
        break;
    case ColorMapStyle_BLUE_RED:
        start = BLUE;
        end = RED;
        break;
    case ColorMapStyle_YELLOW_DARKPURPLE:
        start = YELLOW;
        end = DARKPURPLE;
        break;
    default:
        start = YELLOW;
        end = DARKPURPLE;
        break;
    }

    Vector2 dataRange = FindMinMax2D(data, rows, cols, rowMajor);

    float currentFactor;
    Color currentColor;
    float rightFactor;
    Color rightColor;
    float downFactor;
    Color downColor;
    float diagFactor;
    Color diagColor;
    float offsetX = fig->plot.bounds.x;
    float offsetY = fig->plot.bounds.y;
    BeginTextureMode(fig->plot.target);
    for (int r = 0; r < rows - 1; r++)
    {
        for (int c = 0; c < cols - 1; c++)
        {
            currentFactor = Remap(data[indexer(r, c, rows, cols)], dataRange.x, dataRange.y, 0.0f, 1.0f);
            currentColor = InterpolateColorLinear(start, end, currentFactor);
            rightFactor = Remap(data[indexer(r, c + 1, rows, cols)], dataRange.x, dataRange.y, 0.0f, 1.0f);
            rightColor = InterpolateColorLinear(start, end, rightFactor);
            diagFactor = Remap(data[indexer(r + 1, c + 1, rows, cols)], dataRange.x, dataRange.y, 0.0f, 1.0f);
            diagColor = InterpolateColorLinear(start, end, diagFactor);
            downFactor = Remap(data[indexer(r + 1, c, rows, cols)], dataRange.x, dataRange.y, 0.0f, 1.0f);
            downColor = InterpolateColorLinear(start, end, downFactor);

            Rectangle rect = (Rectangle){offsetX + c * cellWidth,
                                         offsetY + r * cellHeight,
                                         cellWidth,
                                         cellHeight};
            DrawRectangleGradientEx(rect, currentColor, downColor, diagColor, rightColor);
        }
    }
    EndTextureMode();
}

RPAPI void HeatMap2DEx(Figure2D *fig, float *data, int rows, int cols, ColorMapStyle style, int rowMajor)
{
    const float cellWidth = fig->plot.bounds.width / cols;
    const float cellHeight = fig->plot.bounds.height / rows;

    int (*indexer)(int, int, int, int);
    if (rowMajor)
    {
        indexer = &RowMajorIndex;
    }
    else
    {
        indexer = &ColMajorIndex;
    }
    Color start;
    Color end;
    switch (style)
    {
    case ColorMapStyle_WHITE_BLACK:
        start = WHITE;
        end = BLACK;
        break;
    case ColorMapStyle_BLUE_RED:
        start = BLUE;
        end = RED;
        break;
    case ColorMapStyle_YELLOW_DARKPURPLE:
        start = YELLOW;
        end = DARKPURPLE;
        break;
    default:
        start = YELLOW;
        end = DARKPURPLE;
        break;
    }

    Vector2 dataRange = FindMinMax2D(data, rows, cols, rowMajor);

    float factor;
    Color currentColor;
    float offsetX = fig->plot.bounds.x;
    float offsetY = fig->plot.bounds.y;
    BeginTextureMode(fig->plot.target);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            factor = Remap(data[indexer(r, c, rows, cols)], dataRange.x, dataRange.y, 0.0f, 1.0f);
            currentColor = InterpolateColorLinear(start, end, factor);

            Rectangle rect = (Rectangle){offsetX + c * cellWidth,
                                         offsetY + r * cellHeight,
                                         cellWidth,
                                         cellHeight};
            DrawRectangleRec(rect, currentColor);
        }
    }
    EndTextureMode();
}

RPAPI void HeatMap2D(Figure2D *fig, float *data, int rows, int cols)
{
    HeatMap2DEx(fig, data, rows, cols, ColorMapStyle_YELLOW_DARKPURPLE, 1);
}

RPAPI Color InterpolateColorLinear(Color start, Color end, float factor)
{
    Color c = (Color){
        .r = (int)((float)end.r * factor + (float)start.r * (1.0f - factor)),
        .g = (int)((float)end.g * factor + (float)start.g * (1.0f - factor)),
        .b = (int)((float)end.b * factor + (float)start.b * (1.0f - factor)),
        .a = (int)((float)end.a * factor + (float)start.a * (1.0f - factor))};
    return c;
}

RPAPI int RowMajorIndex(int r, int c, int rows, int cols)
{
    return c + cols * r;
}

RPAPI int ColMajorIndex(int r, int c, int rows, int cols)
{
    return r + rows * c;
}

RPAPI Vector2 FindMinMax1D(float *data, int dataSize)
{
    float min = data[0];
    float max = data[0];

    for (int x = 1; x < dataSize; x++)
    {
        if (data[x] < min)
        {
            min = data[x];
        }
        if (data[x] > max)
        {
            max = data[x];
        }
    }

    return (Vector2){.x = min, .y = max};
}

RPAPI Vector2 FindMinMax2D(float *data, int rows, int cols, int rowMajor)
{
    int (*indexer)(int, int, int, int) = rowMajor ? &RowMajorIndex : &ColMajorIndex;

    float min = data[0];
    float max = data[0];
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (data[indexer(r, c, rows, cols)] < min)
            {
                min = data[indexer(r, c, rows, cols)];
            }
            if (data[indexer(r, c, rows, cols)] > max)
            {
                max = data[indexer(r, c, rows, cols)];
            }
        }
    }
    return (Vector2){.x = min, .y = max};
}

#endif // RL_PLOT_IMPLEMENTATION