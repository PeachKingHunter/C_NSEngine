#include "SDL3/SDL.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "Parser/Parser.h"
#include "src/CodeInterpretor.h"
#include "src/GameStruct.h"
#include "src/Render.h"

#include <time.h>

// Warning the wait is rounded exemple: 1000/60 = 16.66 -> 16 or 17 IDK
#define TARGET_FPS 100

void infoSizeOperand(Line *line, void *obj, int objType, int sizeX, int sizeY) {
  if (line->wordsLength < 3)
    printf("Error: size too few args");
  // Change sizeVariables
  sizeX = atoi(line->words[1]);
  sizeY = atoi(line->words[2]);

  // If the object was already created
  if (objType == 1) {
    SDL_SetWindowSize(((WindowLC *)obj)->window, sizeX, sizeY);

  } else if (objType == 2) {
    ((ButtonLC *)obj)->tl->sizeX = sizeX;
    ((ButtonLC *)obj)->tl->sizeY = sizeY;

  } else if (objType == 3) {
    ((TextLabelLC *)obj)->sizeX = sizeX;
    ((TextLabelLC *)obj)->sizeY = sizeY;
  }
}

void infoTextOperand(Line *line, void *obj, int objType, char **text) {
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
}

int main(int argc, char **argv) {
  srand(time(NULL));
  // Variables
  WindowLC *window = NULL;

  GameStruct *gameStruct = getGameStruct();
  gameStruct->delta = NULL;
  gameStruct->globalVars = hashmap_create();
  gameStruct->ressourceVars = hashmap_create();

  // Load game data
  Category *allCategories = parse("./gameFolder/");

  // Create game objects
  Category *info = searchCategory(allCategories, "Info");
  if (info != NULL) {
    FilePiece *fp = info->files;
    while (fp != NULL) {
      Line *line = fp->data[0];

      // all lines in .Info
      int abc = 0;
      int sizeX = 320;
      int sizeY = 180;
      int posX = 0;
      int posY = 0;
      int objType = -1;
      FilePiece *onClickScript = NULL;
      Line *callLine = NULL;
      void *obj = NULL;
      char *text = NULL;
      int borderColor[4] = {255, 0, 0, 255};
      int backgroundColor[4] = {255, 0, 0, 255};
      int textColor[4] = {255, 0, 0, 255};
      int textSize = 10;

      while (abc < fp->nbLine) {
        line = fp->data[abc];

        if (strcmp(line->words[0], "size") == 0) {
          infoSizeOperand(line, obj, objType, sizeX, sizeY);

        } else if (strcmp(line->words[0], "text") == 0) {
          infoTextOperand(line, obj, objType, &text);

        } else if (strcmp(line->words[0], "onClick") == 0) {
          // Change Variable for the script when button clicked
          onClickScript = copyFilePiece(
              searchObjectByName(allCategories, line->words[1], "Script"));

          callLine = copyLine(line);
          // If the object was already created
          if (objType == 2) {
            ((ButtonLC *)obj)->onClickScript = onClickScript;
            ((ButtonLC *)obj)->callLine = callLine;
            onClickScript = NULL;
            callLine = NULL;
          }

        } else if (strcmp(line->words[0], "pos") == 0) {
          // Change positionVariables
          posX = atoi(line->words[1]);
          posY = atoi(line->words[2]);

          // If the object was already created
          if (objType == 2) {
            ((ButtonLC *)obj)->tl->posX = posX;
            ((ButtonLC *)obj)->tl->posY = posY;
          } else if (objType == 3) {
            ((TextLabelLC *)obj)->posX = posX;
            ((TextLabelLC *)obj)->posY = posY;
          }

        } else if (strcmp(line->words[0], "borderColor") == 0) {
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

        } else if (strcmp(line->words[0], "backgroundColor") == 0) {
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

        } else if (strcmp(line->words[0], "textColor") == 0) {
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

        } else if (strcmp(line->words[0], "textSize") == 0) {
          // Change Temp Var textSize
          if (line->wordsLength > 1)
            textSize = atoi(line->words[1]);

          // If the object was already created
          if (objType == 2) {
            setButtonTextSize(obj, textSize);

          } else if (objType == 3) {
            setTextLabelTextSize(obj, textSize);
          }

        } else if (strcmp(line->words[0], "objType") == 0) {
          if (strcmp(line->words[1], "Window") == 0) {
            // Create Window
            WindowLC *tmp = (WindowLC *)malloc(sizeof(WindowLC));
            if (tmp == NULL) {
              printf("Error Creating Window");
              gameStruct->windows = window;
              freeGameStruct();
              return -1;
            }

            tmp->next = window;
            window = tmp;
            // window->next = tmp;
            window->buttons = NULL;
            window->textLabels = NULL;
            window->isVisible = 1;

            window->window =
                SDL_CreateWindow("Test", sizeX, sizeY, SDL_WINDOW_RESIZABLE);
            if (window->window == NULL) {
              free(window);
              return -1;
            }

            // Create Renderer
            SDL_Renderer *renderer = SDL_CreateRenderer(window->window, NULL);
            if (renderer == NULL) {
              freeWindowLC(window);
              return -1;
            }
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            window->renderer = renderer;

            objType = 1;
            obj = window;

          } else if (strcmp(line->words[1], "Button") == 0) {
            // Create Button
            ButtonLC *button = createButton(
                sizeX, sizeY, fp->fileName, &text, &onClickScript, &callLine,
                borderColor, backgroundColor, textColor, textSize);
            if (button == NULL) {
              printf("Error Creating a Button");
              freeWindowLC(window);
              return -1;
            }

            button->next = window->buttons;
            window->buttons = button;

            objType = 2;
            obj = button;

          } else if (strcmp(line->words[1], "TextLabel") == 0) {
            // Create textLabel
            TextLabelLC *textLabel =
                createTextLabel(sizeX, sizeY, fp->fileName, &text, borderColor,
                                backgroundColor, textColor, textSize);
            if (textLabel == NULL) {
              printf("Error Creating a Button");
              freeWindowLC(window);
              return -1;
            }

            textLabel->next = window->textLabels;
            window->textLabels = textLabel;

            objType = 3;
            obj = textLabel;
          }
        }

        // Get next object info
        abc++;
      }

      // Free unused var
      if (text != NULL)
        free(text);
      freeFilePiece(onClickScript);
      freeLine(callLine);

      fp = fp->next;
    }
  }
  Category *tempCat = allCategories;
  if (tempCat != NULL) {
    if (strcmp(tempCat->name, "Info") == 0) {
      allCategories = freeCategory(tempCat);

    } else {
      while (tempCat->next != NULL) {
        if (strcmp(tempCat->next->name, "Info") == 0) {
          tempCat->next = freeCategory(tempCat->next);
          break;
        }
        tempCat = tempCat->next;
      }
    }
  }
  // printCategories(allCategories);

  gameStruct->windows = window;
  gameStruct->allCategories = allCategories;

  gameStruct->timeSec = createEmptyNum(5);
  gameStruct->nbFrame = createEmptyNum(5);
  gameStruct->delta = createEmptyNum(2);

  // Execute on start Scripts (before load ressources)
  Category *startingScripts = searchCategory(allCategories, "Start");
  FilePiece *startScriptsFile = startingScripts->files;
  while (startScriptsFile != NULL) {
    executeCode(startScriptsFile, NULL, NULL);
    startScriptsFile = startScriptsFile->next;
  }
  loadHashmap("saveRessourceData", gameStruct->ressourceVars, deleteNum,
              createNumByConst);

  // Execute on start later Scripts (after load ressources)
  startingScripts = searchCategory(allCategories, "StartLate");
  startScriptsFile = startingScripts->files;
  while (startScriptsFile != NULL) {
    executeCode(startScriptsFile, NULL, NULL);
    startScriptsFile = startScriptsFile->next;
  }  

  // Init SDL3
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return -1;
  }
  if (!TTF_Init()) {
    SDL_Quit();
    return -1;
  }

  // Load font
  gameStruct->font = TTF_OpenFont("fonts/NerdFontMonoReg.ttf", 24.0f);
  if (!gameStruct->font) {
    SDL_Log("Failed to load font: %s", SDL_GetError());
    return 1;
  }

  // Main Loop
  SDL_Event event;

  int isRunning = 1;

  BigNumber *bigOne = createNumByInt(1);
  int frame = 0;

  unsigned long lastTicks = SDL_GetTicks();
  unsigned long actuTicks;
  int offset = 0;

  // int frame = 0;
  while (isRunning) {
    frame++;
    addNumInto(gameStruct->nbFrame, bigOne);
    // wait time for target_fps
    int delayTime = 1000 / TARGET_FPS - offset;
    if (delayTime < 0)
      delayTime = 1;
    SDL_Delay(delayTime);

    // Calcul wait error
    actuTicks = SDL_GetTicks();
    int delta = actuTicks - lastTicks;
    lastTicks = actuTicks;
    offset += delta - 1000 / TARGET_FPS;
    if (gameStruct->delta != NULL) {
      deleteNum(gameStruct->delta);
    }
    gameStruct->delta = createNumByInt(delta);

    // Inputs managing
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_KEY_DOWN:
        SDL_KeyboardEvent keyboardEvent = event.key;
        if (keyboardEvent.key == SDLK_ESCAPE)
          isRunning = 0;
        break;
      case SDL_EVENT_MOUSE_BUTTON_UP:
        SDL_MouseButtonEvent mouseButtonEvent = event.button;
        if (mouseButtonEvent.button == 1) {
          clickLeftMouse(window, mouseButtonEvent.x, mouseButtonEvent.y,
                         mouseButtonEvent.windowID);
        }
        break;
      }

      if (event.type == SDL_EVENT_QUIT) {
        isRunning = 0;
      }
    }

    // Call update functions
    Category *scripts = searchCategory(allCategories, "Update");
    if (scripts != NULL) {
      FilePiece *scriptsFile = scripts->files;
      while (scriptsFile != NULL) {
        executeCode(scriptsFile, NULL, NULL);
        scriptsFile = scriptsFile->next;
      }
    }

    if (frame >= TARGET_FPS) {
      frame -= TARGET_FPS;
      addNumInto(gameStruct->timeSec, bigOne);
      // onUpdateS();
      /*Category *scripts = searchCategory(allCategories, "UpdateS");
    if(scripts != NULL){
      FilePiece *scriptsFile = scripts->files;
      while(scriptsFile != NULL){
        executeCode(scriptsFile, NULL, NULL);
        scriptsFile = scriptsFile->next;
      }
      }*/
    }

    // Render the whole game
    render(window);
  }
  deleteNum(bigOne);

  // Save Data
  saveHashmap("saveRessourceData", gameStruct->ressourceVars,
              getStrOfNumNotFormated);

  // Close All And Stop Program
  freeGameStruct();
  TTF_CloseFont(gameStruct->font);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
