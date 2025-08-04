#ifndef WINDOWLC_H
#define WINDOWLC_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../BigNumberManager/BigNumberManager.h"
#include "../Hachage/Hachage.h"
#include "../Parser/Parser.h"

#include "ButtonLC.h"
#include "TextLabelLC.h"

typedef struct windowLC {
  SDL_Window *window;
  int sizeX;
  int sizeY;
  SDL_Renderer *renderer;
  char *name;
  ButtonLC *buttons;
  TextLabelLC *textLabels;
  int *backgroundColor;
  int isVisible;
  struct windowLC *next;
} WindowLC;

void freeWindowLC(WindowLC *windowLC);

#endif
