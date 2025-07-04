#include "ButtonLC.h"

void freeButtons(ButtonLC *buttons) {
  while(buttons != NULL){
      free(buttons->name);

    if(buttons->text != NULL)
      free(buttons->text);
    
    freeFilePiece(buttons->onClickScript);
    freeLine(buttons->callLine);

    ButtonLC *temp = buttons;
    buttons = buttons->next;
    free(temp);
  }
}


ButtonLC *createButton(int sizeX, int sizeY, char *name, char **text, FilePiece **onClickScript, Line **callLine){
  ButtonLC *button = (ButtonLC *) malloc(sizeof(ButtonLC));
  button->sizeX = sizeX;
  button->sizeY = sizeY;
  button->onClickScript = *onClickScript;
  *onClickScript = NULL;
  button->callLine = *callLine;
  *callLine = NULL;
  button->name = strdup(name);

  if(*text == NULL){
    button->text = (char *) malloc(sizeof(char) * 1);
    button->text[0] = '\0';

  } else {
    button->text = *text;
    *text = NULL;

  }
  return button;
}
