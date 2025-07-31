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

// Principal
void clickLeftMouse(WindowLC *windowLC, float posX, float posY,
                    SDL_WindowID windowID);
void executeCode(FilePiece *code, Line *callLine, HashMap *beforeLocalVar);
int resolveCondition(Line *line, int start, HashMap *variables);
BigNumber *getVar(HashMap *vars, Line *line, int j);

// Operands
int ifOperand(Line *line, HashMap *map, int *index, FilePiece *code);
int printfOperand(Line *line, HashMap *map);
int changeTextOf(Line *line, HashMap *map);
int changeImageOf(Line *line, HashMap *map);
int changeBackgroundColorOf(Line *line, HashMap *map);
int startScript(Line *line, HashMap *map);
int intOperand(Line *line, HashMap *map);
int intG(Line *line, HashMap *map);
int loadOrCreate(Line *line, HashMap *map);
int setSizeOf(Line *line, HashMap *map);
int hideOf(Line *line, HashMap *map);
int showOf(Line *line, HashMap *map);
int arithmetic(Line *line, HashMap *map);
int randInto(Line *line, HashMap *map);

#endif
