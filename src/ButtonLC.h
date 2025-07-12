#ifndef BUTTONLC_H
#define BUTTONLC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Parser/Parser.h"
#include "../Hachage/Hachage.h"
#include "../BigNumberManager/BigNumberManager.h"

#include "TextLabelLC.h"

typedef struct buttonLC {
  FilePiece *onClickScript;
  Line *callLine; //Line that callOnClickScript
  TextLabelLC *tl;
  struct buttonLC *next;
} ButtonLC;

void freeButtons(ButtonLC *buttons);
ButtonLC *createButton(int sizeX, int sizeY, char *name, char **text, FilePiece **onClickScript, Line **callLine, int *colorButton, int *backgroundButton);

// Getters
int getButtonSizeX(ButtonLC *button);
int getButtonSizeY(ButtonLC *button);
int getButtonPosX(ButtonLC *button);
int getButtonPosY(ButtonLC *button);
char *getButtonName(ButtonLC *button);
char *getButtonText(ButtonLC *button);

//Setters
void setButtonBorderColor(ButtonLC *button, int borderColor[4]);
void setButtonBackgroundColor(ButtonLC *button, int backgroundColor[4]);

#endif
