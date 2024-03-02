#include <vector>
#include <string>
#include <map>
#include <functional>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// Переменные для элементов интерфейса
std::map<std::string, Rectangle> gui_rects;
std::map<std::string, std::function<void(void)>> gui_funcs;

// Глобальные переменные (общие для всего приложения)
Music music;                          // Текущая музыка
std::vector<std::string> full_paths;  // Пути до файлов музыки
std::vector<std::string> name_files;  // Имя файла для отображения плейлиста

// Функции для интерфейса
void LoadMusic(void)
{
  TraceLog(LOG_INFO, "LOAD");
  if(IsMusicReady(music))
    UnloadMusicStream(music);
  music = LoadMusicStream("music.mp3");
}

void BtnPlay(void)
{}

void BtnPause(void)
{}

void BtnStop(void)
{}

// Функции для работы интерфейса
void InitGUI(void)
{
  gui_rects["btn_play"] = {0.0f, 0.0f, 96.0f, 32.0f};
  gui_funcs["btn_play"] = BtnPlay;
  gui_rects["btn_pause"] = {0.0f, 0.0f, 96.0f, 32.0f};
  gui_funcs["btn_pause"] = BtnPause;
  gui_rects["btn_stop"] = {0.0f, 0.0f, 96.0f, 32.0f};
  gui_funcs["btn_stop"] = BtnStop;
}

void UpdateGUI(void)
{
  if(GuiButton(gui_rects["btn_play"], "Play")) gui_funcs["btn_play"]();
  if(GuiButton(gui_rects["btn_pause"], "Pause")) gui_funcs["btn_pause"]();
  if(GuiButton(gui_rects["btn_stop"], "Stop")) gui_funcs["btn_stop"]();
}

int main()
{
  InitWindow(800, 600, "mp3player");
  InitAudioDevice();
  InitGUI();

  while (!WindowShouldClose())
  {
    if(IsFileDropped())
    {
      FilePathList dropped_files = LoadDroppedFiles();
      for (int i = 0; i < (int)dropped_files.count; i++)
      {
        if (IsFileExtension(dropped_files.paths[i], ".mp3"))
        {
          if(std::find(full_paths.begin(), full_paths.end(), dropped_files.paths[i]) == full_paths.end())
          {
            full_paths.push_back(dropped_files.paths[i]);
            name_files.push_back(GetFileNameWithoutExt(dropped_files.paths[i]));
          }
        }
      }
      UnloadDroppedFiles(dropped_files);
      for (int i = 0; i < full_paths.size(); i++)
        TraceLog(LOG_INFO, "Path load: %s", full_paths.at(i).c_str());
      for (int i = 0; i < name_files.size(); i++)
        TraceLog(LOG_INFO, "Name file: %s", name_files.at(i).c_str());
    }

    BeginDrawing();
    ClearBackground(BLACK);
    UpdateGUI();
    EndDrawing();
  }
  CloseAudioDevice();
  CloseWindow();
  return 0;
}