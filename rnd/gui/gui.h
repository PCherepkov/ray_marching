#pragma once

#include <nfd.h>

#include "../../def.h"
// #include "../anim/anim.h"

class GUIData {
public:
    bool open_help_flag;
    bool show_fps_flag;
};

void openScene(void);
void saveSceneAs(void);
void saveScene(void);
void newScene(void);

void mainTopBar(void);
void mainOverlay(void);
void helpInfo(void);
void fpsCounter(void);

void UIInit(void);