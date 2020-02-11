#pragma once
#include "Blit3D.h"

//Menu Functionality
void scoreMenuText(AngelcodeFont *textFont, glm::vec2 menuPosition);
void gameOverMenuText(AngelcodeFont *textFont, glm::vec2 menuPosition);

//Button Functionality

//---General---
void pause(int number);
void resume(int number);
void back(int number);
void closeApp(int number);
void mainMenu(int number);

//---Unique---
void highlightWord(int number);
void switchUI(int number);

void basicMethod(int number);
void improvedMethod(int number);
void hashMethod(int number);