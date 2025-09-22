#include "raylib.h"

int main()
{
    InitWindow(800, 720, "Gameboy Emulator");
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
