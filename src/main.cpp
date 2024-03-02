#include <vector>
#include <string>
#include <map>
#include <numeric>
#include <functional>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// Переменные для элементов интерфейса
std::map<std::string, Rectangle> gui_rects;
std::map<std::string, std::function<void(void)>> gui_funcs;
int lv_playlist_scroll_index = 0;
int lv_playlist_active = -1;
int current_active = -1;
std::string lv_playlist_text = "";

// Глобальные переменные (общие для всего приложения)
Music music;                          // Текущая музыка
std::vector<std::string> full_paths;  // Пути до файлов музыки
std::vector<std::string> name_files;  // Имя файла для отображения плейлиста

// Функции для интерфейса
void LoadMusic(void)
{
  if(full_paths.empty()) return;
  if(current_active == lv_playlist_active) return;

  TraceLog(LOG_INFO, "LOAD");
  if(IsMusicReady(music))
    UnloadMusicStream(music);
  music = LoadMusicStream(full_paths.at(lv_playlist_active).c_str());
  current_active = lv_playlist_active;
}

void BtnPlay(void)
{
  TraceLog(LOG_INFO, "PLAY");
  PlayMusicStream(music);
}

void BtnPause(void)
{
  PauseMusicStream(music);
}

void BtnStop(void)
{
  StopMusicStream(music);
}

// Функции для работы интерфейса
void InitGUI(void)
{
  gui_rects["btn_play"] = {10.0f, 350.0f, 100.0f, 40.0f};
  gui_funcs["btn_play"] = BtnPlay;
  gui_rects["btn_pause"] = {120.0f, 350.0f, 100.0f, 40.0f};
  gui_funcs["btn_pause"] = BtnPause;
  gui_rects["btn_stop"] = {230.0f, 350.0f, 100.0f, 40.0f};
  gui_funcs["btn_stop"] = BtnStop;
  
  gui_rects["lv_playlist"] = {340.0f, 10.0f, 250.0f, 380.0f};
}

void UpdateGUI(void)
{
  if(GuiButton(gui_rects["btn_play"], "Play")) gui_funcs["btn_play"]();
  if(GuiButton(gui_rects["btn_pause"], "Pause")) gui_funcs["btn_pause"]();
  if(GuiButton(gui_rects["btn_stop"], "Stop")) gui_funcs["btn_stop"]();
  GuiListView(gui_rects["lv_playlist"], lv_playlist_text.c_str(), &lv_playlist_scroll_index, &lv_playlist_active);
  if(lv_playlist_active != -1) LoadMusic();
}

int main()
{
  InitWindow(600, 400, "mp3player");
  InitAudioDevice();
  InitGUI();

  GuiSetStyle(0, 16, 20);

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
      lv_playlist_text = std::accumulate
      (
        std::next(name_files.begin()), name_files.end(),name_files[0],
          [](std::string a, std::string b) 
          {
            return a + ';' + b;
          }
      );
    }
    UpdateMusicStream(music);

    BeginDrawing();
    ClearBackground(BLACK);
    UpdateGUI();
    EndDrawing();
  }
  CloseAudioDevice();
  CloseWindow();
  return 0;
}