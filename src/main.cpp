#include <raylib.h>
#include <raygui.h>

int main()
{
  InitWindow(800, 600, "mp3player");
  SetTargetFPS(60);
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(0, 0);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}