#include <string>
#include <vector>

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#define GUI_UI_IMPLEMENTATION
#include "gui_UI.h"

Music music;
GuiUIState state;

void btn_load()
{
  TraceLog(LOG_INFO, "LOAD");
  if(IsMusicReady(music))
  {
    UnloadMusicStream(music);
    return;
  }
  music = LoadMusicStream(state.tb_pathText);
}

void btn_resume()
{
  TraceLog(LOG_INFO, "RESUME");
  if(IsMusicStreamPlaying(music))
  {
    PauseMusicStream(music);
    return;
  }
  ResumeMusicStream(music);
}

void btn_play()
{
  TraceLog(LOG_INFO, "PLAY");
  if(IsMusicStreamPlaying(music))
  {
    StopMusicStream(music);
    return;
  }
  PlayMusicStream(music);
}

int main()
{
  state = InitGuiUI();
  SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(state.layoutRecs[0].width, state.layoutRecs[0].height, "mp3player");
  InitAudioDevice();
  SetTargetFPS(60);

  std::vector<std::string> file_paths;

  while (!WindowShouldClose())
  {
    if (state.WindowBox000Active == false) break;
    if (state.btn_loadPressed == true) btn_load();
    if (state.btn_playPressed == true) btn_play();
    if (state.btn_resumePressed == true) btn_resume();
    
    if (IsFileDropped())
    {
      FilePathList droppedFiles = LoadDroppedFiles();
      for (int i = 0; i < (int)droppedFiles.count; i++)
      {
        if (IsFileExtension(droppedFiles.paths[i], ".mp3"))
        {
          file_paths.push_back(std::string(droppedFiles.paths[i]));
          TraceLog(LOG_INFO, "FILE LOAD:%s", file_paths.back().c_str());
        }
      }
      UnloadDroppedFiles(droppedFiles);
    }

    UpdateMusicStream(music);
    
    BeginDrawing();
    ClearBackground(BLACK);
    GuiUI(&state);
    // DrawFPS(0, 0);
    EndDrawing();
  }
  if(IsMusicReady(music))
    UnloadMusicStream(music);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}