#ifndef CODEINTERPRETOR_H
#define CODEINTERPRETOR_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../BigNumberManager/BigNumberManager.h"
#include "../Hachage/Hachage.h"
#include "../Parser/Parser.h"

#include "GameStruct.h"

void clickLeftMouse(WindowLC *windowLC, float posX, float posY,
                    SDL_WindowID windowID);
void executeCode(FilePiece *code, Line *callLine, HashMap *beforeLocalVar);
int resolveCondition(Line *line, int start, HashMap *variables);
BigNumber *getVar(HashMap *vars, Line *line, int j);

#endif
