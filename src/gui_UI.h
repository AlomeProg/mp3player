/*******************************************************************************************
*
*   UI v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_UI_IMPLEMENTATION
*       #include "gui_UI.h"
*
*       INIT: GuiUIState state = InitGuiUI();
*       DRAW: GuiUI(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 AlomeProg. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_UI_H
#define GUI_UI_H

typedef struct {
    bool WindowBox000Active;
    bool btn_loadPressed;
    bool btn_resumePressed;
    bool tb_pathEditMode;
    char tb_pathText[128];
    bool btn_playPressed;
    bool db_file_pathsEditMode;
    int db_file_pathsActive;

    Rectangle layoutRecs[6];

    char db_file_paths[256][16];
    int db_file_paths_index;
    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiUIState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiUIState InitGuiUI(void);
void GuiUI(GuiUIState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_UI_H

/***********************************************************************************
*
*   GUI_UI IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_UI_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiUIState InitGuiUI(void)
{
    GuiUIState state = { 0 };

    state.WindowBox000Active = true;
    state.btn_loadPressed = false;
    state.btn_resumePressed = false;
    state.tb_pathEditMode = false;
    strcpy(state.tb_pathText, "PATH");
    state.btn_playPressed = false;
    state.db_file_pathsEditMode = false;
    state.db_file_pathsActive = 0;

    state.layoutRecs[0] = (Rectangle){ 0, 0, 408, 336 };
    state.layoutRecs[1] = (Rectangle){ 288, 144, 96, 24 };
    state.layoutRecs[2] = (Rectangle){ 288, 96, 96, 24 };
    state.layoutRecs[3] = (Rectangle){ 24, 48, 240, 24 };
    state.layoutRecs[4] = (Rectangle){ 288, 48, 96, 24 };
    state.layoutRecs[5] = (Rectangle){ 24, 96, 120, 24 };

    // Custom variables initialization
    state.db_file_paths_index = 0;

    return state;
}

void GuiUI(GuiUIState *state)
{
    if (state->db_file_pathsEditMode) GuiLock();

    if (state->WindowBox000Active)
    {
        state->WindowBox000Active = !GuiWindowBox(state->layoutRecs[0], "MP3PLAYER");
        state->btn_loadPressed = GuiButton(state->layoutRecs[1], "LOAD"); 
        state->btn_resumePressed = GuiButton(state->layoutRecs[2], "RESUME/PAUSE"); 
        if (GuiTextBox(state->layoutRecs[3], state->tb_pathText, 128, state->tb_pathEditMode)) state->tb_pathEditMode = !state->tb_pathEditMode;
        state->btn_playPressed = GuiButton(state->layoutRecs[4], "PLAY/STOP"); 
        if (GuiDropdownBox(state->layoutRecs[5], NULL, &state->db_file_pathsActive, state->db_file_pathsEditMode)) state->db_file_pathsEditMode = !state->db_file_pathsEditMode;
    }
    
    GuiUnlock();
}

#endif // GUI_UI_IMPLEMENTATION
