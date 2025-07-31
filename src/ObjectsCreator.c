#include "ObjectsCreator.h"
#include "WindowLC.h"

int sizeOperand(Line *line, void *obj, int objType, int *sizeX, int *sizeY) {
  if (line == NULL || obj == NULL)
    return 0;

  if (strcmp(line->words[0], "size") != 0)
    return 0;

  if (line->wordsLength < 3) {
    printf("Error: size too few args");
    return 1;
  }

  // Change sizeVariables
  *sizeX = atoi(line->words[1]);
  *sizeY = atoi(line->words[2]);

  // If the object was already created
  if (objType == 1) {
    SDL_SetWindowSize(((WindowLC *)obj)->window, *sizeX, *sizeY);
    ((WindowLC *)obj)->sizeX = *sizeX;
    ((WindowLC *)obj)->sizeY = *sizeY;

  } else if (objType == 2) {
    ((ButtonLC *)obj)->tl->sizeX = *sizeX;
    ((ButtonLC *)obj)->tl->sizeY = *sizeY;

  } else if (objType == 3) {
    ((TextLabelLC *)obj)->sizeX = *sizeX;
    ((TextLabelLC *)obj)->sizeY = *sizeY;
  }

  return 1;
}

int textOperand(Line *line, void *obj, int objType, char **text) {
  if (line == NULL || obj == NULL || text == NULL)
    return 0;

  if (strcmp(line->words[0], "text") != 0)
    return 0;

  // Get the text to write
  char buffer[500];
  int index = 0;
  buffer[index] = '\0';
  for (int i = 1; i < line->wordsLength; i++) {
    // the buffer have a limited size
    if (strlen(line->words[i]) - 1 + index > 499)
      break;

    // Copy each string
    BigNumber *val = getVar(NULL, line, i);
    if (val == NULL) {
      strcpy(buffer + index, line->words[i]);
      index += strlen(line->words[i]);
    } else {
      char *string = getStrOfNum(val);
      strcpy(buffer + index, string);
      index += strlen(string);
      free(string);
    }
    index++;
    buffer[index] = ' ';
    index++;
  }
  index++;
  buffer[index] = '\0';
  // printf("%s\n", buffer);

  *text = strdup(buffer);

  // If the object was already created
  if (objType == 2) {
    ButtonLC *objCasted = ((ButtonLC *)obj);
    free(objCasted->tl->text);
    objCasted->tl->text = *text;
    *text = NULL;

  } else if (objType == 3) {
    TextLabelLC *objCasted = ((TextLabelLC *)obj);
    free(objCasted->text);
    objCasted->text = *text;
    *text = NULL;
  }

  return 1;
}

int onClickOperand(Line *line, void *obj, FilePiece **onClickScript,
                   Line **callLine, Category *allCategories, int objType) {
  if (line == NULL || allCategories == NULL)
    return 0;

  if (strcmp(line->words[0], "onClick") != 0)
    return 0;

  // Change Variable for the script when button clicked
  *onClickScript = copyFilePiece(
      searchObjectByName(allCategories, line->words[1], "Script"));
  *callLine = copyLine(line);

  // If the object was already created
  if (objType == 2) {
    ((ButtonLC *)obj)->onClickScript = *onClickScript;
    ((ButtonLC *)obj)->callLine = *callLine;
    *onClickScript = NULL;
    *callLine = NULL;
  }

  return 1;
}

int posOperand(Line *line, int *posX, int *posY, void *obj, int objType) {
  if (line == NULL)
    return 0;

  if (strcmp(line->words[0], "pos") != 0)
    return 0;

  // Change positionVariables
  *posX = atoi(line->words[1]);
  *posY = atoi(line->words[2]);

  // If the object was already created
  if (objType == 2) {
    ((ButtonLC *)obj)->tl->posX = *posX;
    ((ButtonLC *)obj)->tl->posY = *posY;
  } else if (objType == 3) {
    ((TextLabelLC *)obj)->posX = *posX;
    ((TextLabelLC *)obj)->posY = *posY;
  }

  return 1;
}

int showOperand(Line *line, bool *isVisible, void *obj, int objType) {
  if (line == NULL)
    return 0;

  if (strcmp(line->words[0], "show") != 0)
    return 0;

  // Change positionVariables
  *isVisible = true;

  // If the object was already created
  if (objType == 1) {
    ((WindowLC *)obj)->isVisible = *isVisible;
    // SDL_ShowWindow(obj);
  } else if (objType == 2) {
    ((ButtonLC *)obj)->tl->isVisible = *isVisible;
  } else if (objType == 3) {
    ((TextLabelLC *)obj)->isVisible = *isVisible;
  }

  return 1;
}

int hideOperand(Line *line, bool *isVisible, void *obj, int objType) {
  if (line == NULL)
    return 0;

  if (strcmp(line->words[0], "hide") != 0)
    return 0;

  // Change positionVariables
  *isVisible = false;

  // If the object was already created
  if (objType == 1) {
    ((WindowLC *)obj)->isVisible = *isVisible;
    // SDL_ShowWindow(obj);
  } else if (objType == 2) {
    ((ButtonLC *)obj)->tl->isVisible = *isVisible;
  } else if (objType == 3) {
    ((TextLabelLC *)obj)->isVisible = *isVisible;
  }

  return 1;
}

int borderColorOperand(Line *line, int *borderColor, void *obj, int objType) {
  if (line == NULL)
    return 0;

  if (strcmp(line->words[0], "borderColor") != 0)
    return 0;

  // Change Temp Var borderColor
  if (line->wordsLength > 3) {
    borderColor[0] = atoi(line->words[1]);
    borderColor[1] = atoi(line->words[2]);
    borderColor[2] = atoi(line->words[3]);
  }
  if (line->wordsLength > 4)
    borderColor[3] = atoi(line->words[4]);

  // If the object was already created
  if (objType == 2) {
    setButtonBorderColor(obj, borderColor);

  } else if (objType == 3) {
    setTextLabelBorderColor(obj, borderColor);
  }

  return 1;
}
int backgroundColorOperand(Line *line, int *backgroundColor, void *obj,
                           int objType) {
  if (line == NULL)
    return 0;

  if (strcmp(line->words[0], "backgroundColor") != 0)
    return 0;

  // Change Temp Var backgroundColor
  if (line->wordsLength > 3) {
    backgroundColor[0] = atoi(line->words[1]);
    backgroundColor[1] = atoi(line->words[2]);
    backgroundColor[2] = atoi(line->words[3]);
  }
  if (line->wordsLength > 4)
    backgroundColor[3] = atoi(line->words[4]);

  // If the object was already created
  if (objType == 2) {
    setButtonBackgroundColor(obj, backgroundColor);

  } else if (objType == 3) {
    setTextLabelBackgroundColor(obj, backgroundColor);
  }

  return 1;
}
int textColorOperand(Line *line, int *textColor, void *obj, int objType) {
  if (line == NULL)
    return 0;

  if (strcmp(line->words[0], "textColor") != 0)
    return 0;

  // Change Temp Var textColor
  if (line->wordsLength > 3) {
    textColor[0] = atoi(line->words[1]);
    textColor[1] = atoi(line->words[2]);
    textColor[2] = atoi(line->words[3]);
  }
  if (line->wordsLength > 4)
    textColor[3] = atoi(line->words[4]);

  // If the object was already created
  if (objType == 2) {
    setButtonTextColor(obj, textColor);

  } else if (objType == 3) {
    setTextLabelTextColor(obj, textColor);
  }

  return 1;
}

int textSizeOperand(Line *line, int *textSize, void *obj, int objType) {
  if (line == NULL)
    return 0;

  if (strcmp(line->words[0], "textSize") != 0)
    return 0;

  // Change Temp Var textSize
  if (line->wordsLength > 1)
    *textSize = atoi(line->words[1]);

  // If the object was already created
  if (objType == 2) {
    setButtonTextSize(obj, *textSize);

  } else if (objType == 3) {
    setTextLabelTextSize(obj, *textSize);
  }

  return 1;
}

int objTypeOperand(Line *line, WindowLC **window, int sizeX, int sizeY,
                   FilePiece *fp, bool isVisible, void **obj, int *objType,
                   char **text, FilePiece **onClickScript, Line **callLine,
                   int *borderColor, int *backgroundColor, int *textColor,
                   int textSize) {
  if (line == NULL)
    return 0;

  if (strcmp(line->words[0], "objType") != 0)
    return 0;

  GameStruct *gameStruct = getGameStruct();

  if (strcmp(line->words[1], "Window") == 0) {
    // Create Window
    WindowLC *tmp = (WindowLC *)malloc(sizeof(WindowLC));
    if (tmp == NULL) {
      printf("Error Creating Window");
      gameStruct->windows = *window;
      freeGameStruct();
      return -1;
    }

    tmp->next = *window;
    *window = tmp;
    // window->next = tmp;
    (*window)->buttons = NULL;
    (*window)->textLabels = NULL;
    (*window)->name = strdup(fp->fileName);
    (*window)->window =
        SDL_CreateWindow("Test", sizeX, sizeY, SDL_WINDOW_RESIZABLE);
    (*window)->sizeX = sizeX;
    (*window)->sizeY = sizeY;
    if ((*window)->window == NULL) {
      free(*window);
      return -1;
    }

    // Create Renderer
    SDL_Renderer *renderer = SDL_CreateRenderer((*window)->window, NULL);
    if (renderer == NULL) {
      freeWindowLC(*window);
      return -1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (*window)->renderer = renderer;

    (*window)->isVisible = isVisible;
    if (isVisible == false) {
      SDL_HideWindow((*window)->window);
    }

    *objType = 1;
    *obj = (*window);

  } else if (strcmp(line->words[1], "Button") == 0) {
    // Create Button
    ButtonLC *button = createButton(
        sizeX, sizeY, fp->fileName, text, onClickScript, callLine, borderColor,
        backgroundColor, textColor, textSize, isVisible);
    if (button == NULL) {
      printf("Error Creating a Button");
      freeWindowLC((*window));
      return -1;
    }

    button->next = (*window)->buttons;
    (*window)->buttons = button;

    *objType = 2;
    *obj = button;

  } else if (strcmp(line->words[1], "TextLabel") == 0) {
    // Create textLabel
    TextLabelLC *textLabel =
        createTextLabel(sizeX, sizeY, fp->fileName, text, borderColor,
                        backgroundColor, textColor, textSize, isVisible);
    if (textLabel == NULL) {
      printf("Error Creating a Button");
      freeWindowLC((*window));
      return -1;
    }

    textLabel->next = (*window)->textLabels;
    (*window)->textLabels = textLabel;

    *objType = 3;
    *obj = textLabel;
  }

  return 1;
}
