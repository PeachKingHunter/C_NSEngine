#ifndef WINDOWLC_H 
#define WINDOWLC_H

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Parser/Parser.h"
#include "../Hachage/Hachage.h"
#include "../BigNumberManager/BigNumberManager.h"

#include "ButtonLC.h"
#include "TextLabelLC.h"

typedef struct windowLC {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int isVisible;
  //char *name;
  ButtonLC *buttons;
  TextLabelLC *textLabels;
  struct windowLC *next;
} WindowLC;

void freeWindowLC(WindowLC *windowLC);

#endif
