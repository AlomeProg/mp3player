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

    Rectangle layoutRecs[5];

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

    state.layoutRecs[0] = (Rectangle){ 0, 0, 408, 192 };
    state.layoutRecs[1] = (Rectangle){ 288, 144, 96, 24 };
    state.layoutRecs[2] = (Rectangle){ 288, 96, 96, 24 };
    state.layoutRecs[3] = (Rectangle){ 24, 144, 240, 24 };
    state.layoutRecs[4] = (Rectangle){ 288, 48, 96, 24 };

    // Custom variables initialization

    return state;
}

void GuiUI(GuiUIState *state)
{
    const char *WindowBox000Text = "MP3PLAYER";
    const char *btn_loadText = "LOAD";
    const char *btn_resumeText = "RESUME/PAUSE";
    const char *btn_playText = "PLAY/STOP";
    
    if (state->WindowBox000Active)
    {
        state->WindowBox000Active = !GuiWindowBox(state->layoutRecs[0], WindowBox000Text);
        state->btn_loadPressed = GuiButton(state->layoutRecs[1], btn_loadText); 
        state->btn_resumePressed = GuiButton(state->layoutRecs[2], btn_resumeText); 
        if (GuiTextBox(state->layoutRecs[3], state->tb_pathText, 128, state->tb_pathEditMode)) state->tb_pathEditMode = !state->tb_pathEditMode;
        state->btn_playPressed = GuiButton(state->layoutRecs[4], btn_playText); 
    }
}

#endif // GUI_UI_IMPLEMENTATION
