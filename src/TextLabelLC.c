#include "TextLabelLC.h"

void freeTextLabels(TextLabelLC *tlLC) {
  while(tlLC != NULL){
    if(tlLC->name != NULL)
      free(tlLC->name);

    if(tlLC->text != NULL)
      free(tlLC->text);

    TextLabelLC *temp = tlLC;
    tlLC = tlLC->next;
    free(temp);
  }
}

TextLabelLC *createTextLabel(int sizeX, int sizeY, char *name, char **text){
  TextLabelLC *textLabel = (TextLabelLC *) malloc(sizeof(TextLabelLC));
  if(textLabel == NULL){
    return NULL;
  }
  textLabel->sizeX = sizeX;
  textLabel->sizeY = sizeY;
  textLabel->name = strdup(name);

  if(*text == NULL){
    textLabel->text = (char *) malloc(sizeof(char) * 1);
    textLabel->text[0] = '\0';

  } else {
    textLabel->text = *text;
    *text = NULL;

  }
  return textLabel;
}
