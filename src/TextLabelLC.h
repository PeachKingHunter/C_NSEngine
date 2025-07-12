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

// Getters
int getTextLabelSizeX(TextLabelLC *tl);
int getTextLabelSizeY(TextLabelLC *tl);
int getTextLabelPosX(TextLabelLC *tl);
int getTextLabelPosY(TextLabelLC *tl);
char *getTextLabelName(TextLabelLC *tl);
char *getTextLabelText(TextLabelLC *tl);

//Setters
void setTextLabelBorderColor(TextLabelLC *tl, int borderColor[4]);
void setTextLabelBackgroundColor(TextLabelLC *tl, int backgroundColor[4]);

#endif
