#include "raylib.h"
#define RL_PLOT_IMPLEMENTATION
#include "RayPlot_CustomFont.h"
#include "rplot.h"
#define SIZE 100

int main(void) {
    const int windowWidth = 800;
    const int windowHeight = 800;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(windowWidth, windowHeight, "Testing");
    SetTargetFPS(30);
    Texture tex = LoadTexture("./assets/tex.png");
    Rectangle dest =
        (Rectangle){.x = 0, .y = 0, .width = tex.width, .height = tex.height};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexturePro(
            tex,
            (Rectangle){
                .x = 0, .y = 0, .width = tex.width, .height = tex.height},
            dest, (Vector2){.x = 0, .y = 0}, 0.0f, WHITE);
        DrawRectangleLinesEx(dest, 2.0f, ORANGE);
        EndDrawing();
    }

    CloseWindow();
    UnloadTexture(tex);
    return 0;
}
