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
    bool TextBox004EditMode;
    char TextBox004Text[128];

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
static void Button001();
static void Button002();
static void Button003();

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
    state.TextBox004EditMode = false;
    strcpy(state.TextBox004Text, "PATH");

    state.layoutRecs[0] = (Rectangle){ 120, 48, 408, 192 };
    state.layoutRecs[1] = (Rectangle){ 408, 192, 96, 24 };
    state.layoutRecs[2] = (Rectangle){ 408, 144, 96, 24 };
    state.layoutRecs[3] = (Rectangle){ 408, 96, 96, 24 };
    state.layoutRecs[4] = (Rectangle){ 144, 192, 240, 24 };

    // Custom variables initialization

    return state;
}
static void Button001()
{
    // TODO: Implement control logic
}
static void Button002()
{
    // TODO: Implement control logic
}
static void Button003()
{
    // TODO: Implement control logic
}


void GuiUI(GuiUIState *state)
{
    if (state->WindowBox000Active)
    {
        state->WindowBox000Active = !GuiWindowBox(state->layoutRecs[0], "MP3PLAYER");
        if (GuiButton(state->layoutRecs[1], "PLAY")) Button001(); 
        if (GuiButton(state->layoutRecs[2], "RESUME/STOP")) Button002(); 
        if (GuiButton(state->layoutRecs[3], "PLAY/PAUSE")) Button003(); 
        if (GuiTextBox(state->layoutRecs[4], state->TextBox004Text, 128, state->TextBox004EditMode)) state->TextBox004EditMode = !state->TextBox004EditMode;
    }
}

#endif // GUI_UI_IMPLEMENTATION