#include "ButtonLC.h"

void freeButtons(ButtonLC *buttons) {
  while (buttons != NULL) {
    freeTextLabels(buttons->tl);

    freeFilePiece(buttons->onClickScript);
    freeLine(buttons->callLine);

    ButtonLC *temp = buttons;
    buttons = buttons->next;
    free(temp);
  }
}

ButtonLC *createButton(int sizeX, int sizeY, char *name, char **text,
                       FilePiece **onClickScript, Line **callLine,
                       int *borderColor, int *backgroundColor, int *textColor,
                       int textSize, bool isVisible) {
  ButtonLC *button = (ButtonLC *)malloc(sizeof(ButtonLC));
  button->tl = createTextLabel(sizeX, sizeY, name, text, borderColor,
                               backgroundColor, textColor, textSize, isVisible);
  button->onClickScript = *onClickScript;
  *onClickScript = NULL;
  button->callLine = *callLine;
  *callLine = NULL;

  return button;
}

int getButtonSizeX(ButtonLC *button) {
  if (button == NULL)
    return -1;

  return getTextLabelSizeX(button->tl);
}

int getButtonSizeY(ButtonLC *button) {
  if (button == NULL)
    return -1;

  return getTextLabelSizeY(button->tl);
}

int getButtonPosX(ButtonLC *button) {
  if (button == NULL)
    return -1;

  return getTextLabelPosX(button->tl);
}

int getButtonPosY(ButtonLC *button) {
  if (button == NULL)
    return -1;

  return getTextLabelPosY(button->tl);
}

char *getButtonName(ButtonLC *button) {
  if (button == NULL)
    return NULL;

  return getTextLabelName(button->tl);
}

char *getButtonText(ButtonLC *button) {
  if (button == NULL)
    return NULL;

  return getTextLabelText(button->tl);
}

// Setters
void setButtonBorderColor(ButtonLC *button, int borderColor[4]) {
  if (button == NULL)
    return;

  setTextLabelBorderColor(button->tl, borderColor);
}

void setButtonBackgroundColor(ButtonLC *button, int backgroundColor[4]) {
  if (button == NULL)
    return;

  setTextLabelBackgroundColor(button->tl, backgroundColor);
}

void setButtonTextColor(ButtonLC *button, int textColor[4]) {
  if (button == NULL)
    return;

  setTextLabelTextColor(button->tl, textColor);
}

void setButtonTextSize(ButtonLC *button, int TextSize) {
  if (button == NULL)
    return;

  setTextLabelTextSize(button->tl, TextSize);
}
