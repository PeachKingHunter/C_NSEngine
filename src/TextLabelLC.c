#include "TextLabelLC.h"

void freeTextLabels(TextLabelLC *tlLC) {
  while(tlLC != NULL){
    if(tlLC->name != NULL)
      free(tlLC->name);

    if(tlLC->text != NULL)
      free(tlLC->text);

    free(tlLC->borderColor);
    free(tlLC->backgroundColor);

    TextLabelLC *temp = tlLC;
    tlLC = tlLC->next;
    free(temp);
  }
}

TextLabelLC *createTextLabel(int sizeX, int sizeY, char *name, char **text, int *borderColor, int *backgroundColor){
  TextLabelLC *textLabel = (TextLabelLC *) malloc(sizeof(TextLabelLC));
  if(textLabel == NULL){
    return NULL;
  }
  textLabel->sizeX = sizeX;
  textLabel->sizeY = sizeY;
  textLabel->name = strdup(name);

  textLabel->borderColor = (int *) malloc(sizeof(int) * 4);
  if(textLabel->borderColor != NULL){
    textLabel->borderColor[0] = borderColor[0];
    textLabel->borderColor[1] = borderColor[1];
    textLabel->borderColor[2] = borderColor[2];
    textLabel->borderColor[3] = borderColor[3];
  }

  textLabel->backgroundColor = (int *) malloc(sizeof(int) * 4);
  if(textLabel->backgroundColor != NULL){
    textLabel->backgroundColor[0] = backgroundColor[0];
    textLabel->backgroundColor[1] = backgroundColor[1];
    textLabel->backgroundColor[2] = backgroundColor[2];
    textLabel->backgroundColor[3] = backgroundColor[3];
  }

  if(*text == NULL){
    textLabel->text = (char *) malloc(sizeof(char) * 1);
    textLabel->text[0] = '\0';

  } else {
    textLabel->text = *text;
    *text = NULL;

  }
  return textLabel;
}
