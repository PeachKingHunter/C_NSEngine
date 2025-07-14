#ifndef GAMESTRUCT_H
#define GAMESTRUCT_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../BigNumberManager/BigNumberManager.h"
#include "../Hachage/Hachage.h"
#include "../Parser/Parser.h"

// #include "Render.h"
#include "WindowLC.h"

typedef struct gameStruct {
  HashMap *globalVars;
  HashMap *ressourceVars;
  Category *allCategories;
  WindowLC *windows;
  BigNumber *timeSec;
  BigNumber *nbFrame;
  BigNumber *delta;
  TTF_Font *font;
} GameStruct;

GameStruct *getGameStruct();
void freeGameStruct();

FilePiece *searchObjectByName(Category *allCat, char *name, char *category);
ButtonLC *searchButton(WindowLC *windowLC, char *name);
TextLabelLC *searchTextLabel(WindowLC *windowLC, char *name);
void *searchInstanceByName(WindowLC *windowLC, char *name, int *type);
TTF_Font *getGameStructFont();
#endif
