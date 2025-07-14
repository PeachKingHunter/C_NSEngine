#include "TextLabelLC.h"

void freeTextLabels(TextLabelLC *tlLC) {
  while (tlLC != NULL) {
    if (tlLC->name != NULL)
      free(tlLC->name);

    if (tlLC->text != NULL)
      free(tlLC->text);

    free(tlLC->borderColor);
    free(tlLC->backgroundColor);
    free(tlLC->textColor);

    TextLabelLC *temp = tlLC;
    tlLC = tlLC->next;
    free(temp);
  }
}

TextLabelLC *createTextLabel(int sizeX, int sizeY, char *name, char **text,
                             int *borderColor, int *backgroundColor,
                             int *textColor, int textSize) {
  TextLabelLC *textLabel = (TextLabelLC *)malloc(sizeof(TextLabelLC));
  if (textLabel == NULL) {
    return NULL;
  }
  textLabel->sizeX = sizeX;
  textLabel->sizeY = sizeY;
  textLabel->name = strdup(name);

  textLabel->borderColor = (int *)malloc(sizeof(int) * 4);
  if (textLabel->borderColor != NULL) {
    textLabel->borderColor[0] = borderColor[0];
    textLabel->borderColor[1] = borderColor[1];
    textLabel->borderColor[2] = borderColor[2];
    textLabel->borderColor[3] = borderColor[3];
  }

  textLabel->backgroundColor = (int *)malloc(sizeof(int) * 4);
  if (textLabel->backgroundColor != NULL) {
    textLabel->backgroundColor[0] = backgroundColor[0];
    textLabel->backgroundColor[1] = backgroundColor[1];
    textLabel->backgroundColor[2] = backgroundColor[2];
    textLabel->backgroundColor[3] = backgroundColor[3];
  }

  textLabel->textColor = (int *)malloc(sizeof(int) * 4);
  if (textLabel->textColor != NULL) {
    textLabel->textColor[0] = textColor[0];
    textLabel->textColor[1] = textColor[1];
    textLabel->textColor[2] = textColor[2];
    textLabel->textColor[3] = textColor[3];
  }

  textLabel->textSize = ((float)textSize) / 10.;

  if (*text == NULL) {
    textLabel->text = (char *)malloc(sizeof(char) * 1);
    textLabel->text[0] = '\0';

  } else {
    textLabel->text = *text;
    *text = NULL;
  }
  textLabel->next = NULL;
  return textLabel;
}

// Getters
int getTextLabelSizeX(TextLabelLC *tl) {
  if (tl == NULL)
    return -1;

  return tl->sizeX;
}

int getTextLabelSizeY(TextLabelLC *tl) {
  if (tl == NULL)
    return -1;

  return tl->sizeY;
}

int getTextLabelPosX(TextLabelLC *tl) {
  if (tl == NULL)
    return -1;

  return tl->posX;
}

int getTextLabelPosY(TextLabelLC *tl) {
  if (tl == NULL)
    return -1;

  return tl->posY;
}

char *getTextLabelName(TextLabelLC *tl) {
  if (tl == NULL)
    return NULL;

  return tl->name;
}

char *getTextLabelText(TextLabelLC *tl) {
  if (tl == NULL)
    return NULL;

  return tl->text;
}

// Setters
void setTextLabelBorderColor(TextLabelLC *tl, int borderColor[4]) {
  if (tl == NULL || borderColor == NULL)
    return;

  tl->borderColor[0] = borderColor[0];
  tl->borderColor[1] = borderColor[1];
  tl->borderColor[2] = borderColor[2];
  tl->borderColor[3] = borderColor[3];
}

void setTextLabelBackgroundColor(TextLabelLC *tl, int backgroundColor[4]) {
  if (tl == NULL || backgroundColor == NULL)
    return;

  tl->backgroundColor[0] = backgroundColor[0];
  tl->backgroundColor[1] = backgroundColor[1];
  tl->backgroundColor[2] = backgroundColor[2];
  tl->backgroundColor[3] = backgroundColor[3];
}

void setTextLabelTextColor(TextLabelLC *tl, int textColor[4]) {
  if (tl == NULL || textColor == NULL)
    return;

  tl->textColor[0] = textColor[0];
  tl->textColor[1] = textColor[1];
  tl->textColor[2] = textColor[2];
  tl->textColor[3] = textColor[3];
}

void setTextLabelTextSize(TextLabelLC *tl, int textSize) {
  if (tl == NULL)
    return;

  tl->textSize = ((float)textSize) / 10;
}
