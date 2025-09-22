#include "raylib.h"

int main()
{
    InitWindow(800, 600, "Gameboy Emulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("hello world!", 190, 200, 48, BEIGE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
