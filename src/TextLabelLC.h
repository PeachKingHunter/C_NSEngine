#ifndef TEXTLABELLC_H
#define TEXTLABELLC_H

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Parser/Parser.h"
#include "../Hachage/Hachage.h"
#include "../BigNumberManager/BigNumberManager.h"

typedef struct textLabelLC {
  int sizeX, sizeY;
  int posX, posY;
  char *name;
  char *text;  
  int *borderColor;
  int *backgroundColor;
  struct textLabelLC *next;
} TextLabelLC;

void freeTextLabels(TextLabelLC *textLabels);
TextLabelLC *createTextLabel(int sizeX, int sizeY, char *name, char **text, int *borderColor, int *backgroundColor);

#endif
