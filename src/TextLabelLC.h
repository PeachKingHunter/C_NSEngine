#ifndef TEXTLABELLC_H
#define TEXTLABELLC_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../BigNumberManager/BigNumberManager.h"
#include "../Hachage/Hachage.h"
#include "../Parser/Parser.h"

typedef struct textLabelLC {
  int sizeX, sizeY;
  int posX, posY;
  char *name;
  char *text;
  int *borderColor;
  int *backgroundColor;
  int *textColor;
  float textSize;
  char *imagePath;
  // int *textBorderColor;
  // int textBorderSize;
  bool isVisible;
  struct textLabelLC *next;
} TextLabelLC;

void freeTextLabels(TextLabelLC *textLabels);
TextLabelLC *createTextLabel(int sizeX, int sizeY, char *name, char **text,
                             int *borderColor, int *backgroundColor,
                             int *textColor, int textSize, bool isVisible);

// Getters
int getTextLabelSizeX(TextLabelLC *tl);
int getTextLabelSizeY(TextLabelLC *tl);
int getTextLabelPosX(TextLabelLC *tl);
int getTextLabelPosY(TextLabelLC *tl);
char *getTextLabelName(TextLabelLC *tl);
char *getTextLabelText(TextLabelLC *tl);

// Setters
void setTextLabelBorderColor(TextLabelLC *tl, int borderColor[4]);
void setTextLabelBackgroundColor(TextLabelLC *tl, int backgroundColor[4]);
void setTextLabelTextColor(TextLabelLC *tl, int textColor[4]);
void setTextLabelTextSize(TextLabelLC *tl, int textSize);

#endif
