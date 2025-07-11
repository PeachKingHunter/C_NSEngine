#ifndef GAMESTRUCT_H
#define GAMESTRUCT_H

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Parser/Parser.h"
#include "../Hachage/Hachage.h"
#include "../BigNumberManager/BigNumberManager.h"

#include "Render.h"

typedef struct gameStruct {
  HashMap *globalVars;
  HashMap *ressourceVars;
  Category *allCategories;
  WindowLC *windows;
  BigNumber *timeSec;
  BigNumber *nbFrame;
  BigNumber *delta;
} GameStruct;

GameStruct *getGameStruct();
void freeGameStruct();

FilePiece *searchObjectByName(Category *allCat, char *name, char *category);
ButtonLC *searchButton(WindowLC *windowLC, char *name);
TextLabelLC *searchTextLabel(WindowLC *windowLC, char *name);
void *searchInstanceByName(WindowLC *windowLC, char *name, int *type);

#endif

