#ifndef BUTTONLC_H
#define BUTTONLC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Parser/Parser.h"
#include "../Hachage/Hachage.h"
#include "../BigNumberManager/BigNumberManager.h"

typedef struct buttonLC {
  int sizeX, sizeY;
  int posX, posY;
  char *name;
  char *text;
  int *borderColor;
  int *backgroundColor;
  int *textColor;
  FilePiece *onClickScript;
  Line *callLine; //Line that callOnClickScript
  struct buttonLC *next;
} ButtonLC;

void freeButtons(ButtonLC *buttons);
ButtonLC *createButton(int sizeX, int sizeY, char *name, char **text, FilePiece **onClickScript, Line **callLine, int *colorButton, int *backgroundButton);

#endif
