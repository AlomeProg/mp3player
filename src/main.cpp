#include <vector>
#include <string>
#include <map>
#include <numeric>
#include <array>
#include <functional>
#include <complex>
#include <cmath>
#include <iostream>

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
float sb_volume   = 100;
float sb_pitch    = 100;
float sb_pan      = 100;
std::array<float, 320> averageVolume; // Громкость для визуализации
float sb_freq0    = 200;
float sb_freq1    = 3000;
float sb_freq2    = 5000;

// Глобальные переменные (общие для всего приложения)
Music music;                          // Текущая музыка
std::vector<std::string> full_paths;  // Пути до файлов музыки
std::vector<std::string> name_files;  // Имя файла для отображения плейлиста
bool is_equalizer = false;            // Включен ли эквалайзер

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
  TraceLog(LOG_INFO, "PAUSE");
  PauseMusicStream(music);
}

void BtnStop(void)
{
  TraceLog(LOG_INFO, "STOP");
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

  gui_rects["sb_volume"] = {80.0f, 320.0f, 250.0f, 20.0f};
  gui_rects["sb_pitch"] = {80.0f, 290.0f, 250.0f, 20.0f};
  gui_rects["sb_pan"] = {80.0f, 260.0f, 250.0f, 20.0f};

  gui_rects["lv_playlist"] = {340.0f, 10.0f, 250.0f, 240.0f};

  gui_rects["r_visual"] = {10.0f, 10.0f, 320.0f, 240.0f};
  gui_rects["cb_equalizer"] = {340.0f, 260.0f, 20.0f, 20.0f};

  gui_rects["sb_freq0"] = {400.0f, 290.0f, 100.0f, 20.0f};
  gui_rects["sb_freq1"] = {400.0f, 320.0f, 100.0f, 20.0f};
  gui_rects["sb_freq2"] = {400.0f, 350.0f, 100.0f, 20.0f};
}

void UpdateVolume(void)
{
  if(sb_volume <= 0.01) 
  {
    SetMusicVolume(music, 0.0f);
    return;
  }
  SetMusicVolume(music, sb_volume / 100.0f);
}

void UpdatePitch(void)
{
  if(sb_pitch <= 0.01) 
  {
    SetMusicPitch(music, 0.0f);
    return;
  }
  SetMusicPitch(music, sb_pitch / 100.0f);
}

void UpdatePan(void)
{
  if(sb_pan <= 0.01) 
  {
    SetMusicPan(music, 0.0f);
    return;
  }
  SetMusicPan(music, sb_pan / 100.0f);
}

void UpdateGUI(void)
{
  if(GuiButton(gui_rects["btn_play"], "Play")) gui_funcs["btn_play"]();
  if(GuiButton(gui_rects["btn_pause"], "Pause")) gui_funcs["btn_pause"]();
  if(GuiButton(gui_rects["btn_stop"], "Stop")) gui_funcs["btn_stop"]();
  GuiListView(gui_rects["lv_playlist"], lv_playlist_text.c_str(), &lv_playlist_scroll_index, &lv_playlist_active);
  if(lv_playlist_active != -1) LoadMusic();
  int tmp = 0;
  tmp = GuiSlider(gui_rects["sb_volume"], "Volume:", "", &sb_volume, 0, 100);
  if(tmp == 1) UpdateVolume();
  tmp = GuiSlider(gui_rects["sb_pitch"], "Pitch:", "", &sb_pitch, 0, 100);
  if(tmp == 1) UpdatePitch();
  tmp = GuiSlider(gui_rects["sb_pan"], "Pan:", "", &sb_pan, 0, 100);
  if(tmp == 1) UpdatePan();
  tmp = GuiSlider(gui_rects["sb_freq0"], "Freq0:", "", &sb_freq0, 20, 20000);
  tmp = GuiSlider(gui_rects["sb_freq1"], "Freq1:", "", &sb_freq1, 20, 20000);
  tmp = GuiSlider(gui_rects["sb_freq2"], "Freq2:", "", &sb_freq2, 20, 20000);
  
  DrawRectangleLinesEx(gui_rects["r_visual"], 2.0f, BLACK);
  for (int i = 0; i < averageVolume.size(); i++)
    DrawLine(
      gui_rects["r_visual"].x + 1 + i, 
      gui_rects["r_visual"].y + gui_rects["r_visual"].height - 1 - averageVolume[i] * 256, 
      gui_rects["r_visual"].x + 1 + i,
      gui_rects["r_visual"].y + gui_rects["r_visual"].height - 1, MAROON
    );
  GuiCheckBox(gui_rects["cb_equalizer"], "Equalizer", &is_equalizer);
}

void fft(float* real, float* imag, int n) {
  if (n == 1) return;

  float* real_even = (float*)malloc(n / 2 * sizeof(float));
  float* imag_even = (float*)malloc(n / 2 * sizeof(float));
  float* real_odd = (float*)malloc(n / 2 * sizeof(float));
  float* imag_odd = (float*)malloc(n / 2 * sizeof(float));

  for (int i = 0; i < n / 2; ++i) {
    real_even[i] = real[2 * i];
    imag_even[i] = imag[2 * i];
    real_odd[i] = real[2 * i + 1];
    imag_odd[i] = imag[2 * i + 1];
  }

  fft(real_even, imag_even, n / 2);
  fft(real_odd, imag_odd, n / 2);

  for (int k = 0; k < n / 2; ++k) {
    float t_real = cos(2 * PI * k / n) * real_odd[k] - sin(2 * PI * k / n) * imag_odd[k];
    float t_imag = sin(2 * PI * k / n) * real_odd[k] + cos(2 * PI * k / n) * imag_odd[k];
    real[k] = real_even[k] + t_real;
    imag[k] = imag_even[k] + t_imag;
    real[k + n / 2] = real_even[k] - t_real;
    imag[k + n / 2] = imag_even[k] - t_imag;
  }

  free(real_even);
  free(imag_even);
  free(real_odd);
  free(imag_odd);
}

void ifft(float* real, float* imag, int n) {
  // Меняем действительные и мнимые части местами
  for (int i = 0; i < n; ++i) {
    float temp = real[i];
    real[i] = imag[i];
    imag[i] = temp;
  }

  // Выполняем прямое преобразование Фурье
  fft(real, imag, n);

  // Меняем действительные и мнимые части обратно и делим на n
  for (int i = 0; i < n; ++i) {
    float temp = real[i];
    real[i] = imag[i] / n;
    imag[i] = temp / n;
  }
}

// Функция для создания эквалайзера для одной частоты
void equalize_frequency(float* real, float* imag, int n, int sampleRate, float targetFrequency, float targetAmplitude) {
    int targetIndex = targetFrequency * n / sampleRate;

    // Вычисляем текущую амплитуду целевой частоты
    float currentAmplitude = sqrt(real[targetIndex] * real[targetIndex] + imag[targetIndex] * imag[targetIndex]);

    // Вычисляем коэффициент масштабирования для установки нужной амплитуды
    float scale = targetAmplitude / currentAmplitude;

    // Масштабируем амплитуду целевой частоты
    real[targetIndex] *= scale;
    imag[targetIndex] *= scale;
}

void ProcessAudio(void *buffer, unsigned int frames)
{
  float *samples = (float *)buffer;   // Samples internally stored as <float>s
  float average = 0.0f;               // Temporary average volume
    
  for (unsigned int frame = 0; frame < frames; frame++)
  {
    float *left = &samples[frame * 2 + 0], *right = &samples[frame * 2 + 1];
    
    average += fabsf(*left) / frames;   // accumulating average volume
    average += fabsf(*right) / frames;
  }

  for (int i = 0; i < averageVolume.size(); i++) averageVolume[i] = averageVolume[i + 1];

  averageVolume.at(averageVolume.size()-1) = average; 

  if(!is_equalizer) return;

  float *real = (float*)calloc(512, sizeof(float));
  float *imag = (float*)calloc(512, sizeof(float));

  for (size_t i = 0; i < frames; i++)
  {
    real[i] = samples[i];
  }

  fft(real, imag, 512);

  equalize_frequency(real, imag, 512, 44100, sb_freq0, 0.5);
  equalize_frequency(real, imag, 512, 44100, sb_freq1, 2);
  equalize_frequency(real, imag, 512, 44100, sb_freq2, 3);

  ifft(real, imag, 512);
  
  for (size_t i = 0; i < frames; i++)
  {
    samples[i] = real[i];
  }
}

int main()
{
  InitWindow(600, 400, "mp3player");
  SetWindowState(FLAG_VSYNC_HINT);
  InitAudioDevice();
  InitGUI();

  AttachAudioMixedProcessor(ProcessAudio);

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
    ClearBackground(RAYWHITE);
    UpdateGUI();
    DrawFPS(0.0f, 0.0f);
    EndDrawing();
  }
  CloseAudioDevice();
  CloseWindow();
  return 0;
}