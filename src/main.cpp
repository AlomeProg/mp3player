#include "raylib.h"

int main()
{
  InitWindow(800, 600, "mp3player");
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(BLACK); 
    EndDrawing();
  }
  CloseWindow();
  return 0;
}