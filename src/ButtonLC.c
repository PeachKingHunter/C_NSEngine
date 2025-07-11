#include "ButtonLC.h"

void freeButtons(ButtonLC *buttons) {
  while(buttons != NULL){
      free(buttons->name);

    if(buttons->text != NULL)
      free(buttons->text);
    
    free(buttons->borderColor);
    free(buttons->backgroundColor);
    free(buttons->textColor);

    freeFilePiece(buttons->onClickScript);
    freeLine(buttons->callLine);

    ButtonLC *temp = buttons;
    buttons = buttons->next;
    free(temp);
  }
}


ButtonLC *createButton(int sizeX, int sizeY, char *name, char **text, FilePiece **onClickScript, Line **callLine, int *borderColor, int *backgroundColor){
  ButtonLC *button = (ButtonLC *) malloc(sizeof(ButtonLC));
  button->sizeX = sizeX;
  button->sizeY = sizeY;
  button->onClickScript = *onClickScript;
  *onClickScript = NULL;
  button->callLine = *callLine;
  *callLine = NULL;
  button->name = strdup(name);

  button->borderColor = (int *) malloc(sizeof(int) * 4);
  if(button->borderColor != NULL){
    button->borderColor[0] = borderColor[0];
    button->borderColor[1] = borderColor[1];
    button->borderColor[2] = borderColor[2];
    button->borderColor[3] = borderColor[3];
  }

  button->backgroundColor = (int *) malloc(sizeof(int) * 4);
  if(button->backgroundColor != NULL){
    button->backgroundColor[0] = backgroundColor[0];
    button->backgroundColor[1] = backgroundColor[1];
    button->backgroundColor[2] = backgroundColor[2];
    button->backgroundColor[3] = backgroundColor[3];
  }

  button->textColor = (int *) malloc(sizeof(int) * 4);
  if(button->textColor != NULL){
    button->textColor[0] = 255;
    button->textColor[1] = 0;
    button->textColor[2] = 0;
    button->textColor[3] = 255;
  }

  if(*text == NULL){
    button->text = (char *) malloc(sizeof(char) * 1);
    button->text[0] = '\0';

  } else {
    button->text = *text;
    *text = NULL;

  }
  return button;
}
