#include "SDL3/SDL_render.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_events.h"

#include "Parser/Parser.h"
#include "src/GameStruct.h"
#include "src/CodeInterpretor.h"
#include "src/Render.h"

#include <time.h>

// Warning the wait is rounded exemple: 1000/60 = 16.66 -> 16 or 17 IDK
#define TARGET_FPS 100

void infoSizeOperand(Line *line, void *obj, int objType, int sizeX, int sizeY){
  if(line->wordsLength < 3)
    printf("Error: size too few args");
  //Change sizeVariables
  sizeX = atoi(line->words[1]);
  sizeY = atoi(line->words[2]);

  //If the object was already created
  if(objType == 1){
    SDL_SetWindowSize( ((WindowLC *) obj)->window, sizeX, sizeY);

  } else if(objType == 2){
    ((ButtonLC *) obj)->sizeX = sizeX;
    ((ButtonLC *) obj)->sizeY = sizeY;

  } else if(objType == 3){
    ((TextLabelLC *) obj)->sizeX = sizeX;
    ((TextLabelLC *) obj)->sizeY = sizeY;

  }
}

void infoTextOperand(Line *line, void *obj, int objType, char **text){
  // Get the text to write
  char buffer[500];
  int index = 0;
  buffer[index] = '\0';
  for(int i=1; i < line->wordsLength; i++){
    // the buffer have a limited size
    if(strlen(line->words[i]) - 1 + index > 499)
      break;

    // Copy each string
    BigNumber *val = getVar(NULL, line, i);
    if(val == NULL){
      strcpy(buffer+index, line->words[i]);
      index += strlen(line->words[i]);
    } else {
      char *string = getStrOfNum(val);
      strcpy(buffer+index, string);
      index += strlen(string);
      free(string);
    }
    index++;
    buffer[index] = ' ';
    index++;
  }
  index++;
  buffer[index] = '\0';
  //printf("%s\n", buffer);

  *text = strdup(buffer);

  //If the object was already created
  if(objType == 2){
    ButtonLC *objCasted = ((ButtonLC *) obj);
    free(objCasted->text);
    objCasted->text = *text;
    *text = NULL;

  } else if(objType == 3){
    TextLabelLC *objCasted = ((TextLabelLC *) obj);
    free(objCasted->text);
    objCasted->text = *text;
    *text = NULL;

  }
}



int main(int argc, char** argv)
{
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
  if(info != NULL){
    FilePiece *fp = info->files;
    while(fp != NULL){
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

      while(abc < fp->nbLine){
        line = fp->data[abc];

        if(strcmp(line->words[0], "size") == 0){
          infoSizeOperand(line, obj, objType, sizeX, sizeY);

        } else if(strcmp(line->words[0], "text") == 0){
          infoTextOperand(line, obj, objType, &text);

        } else if(strcmp(line->words[0], "onClick") == 0){
          //Change Variable for the script when button clicked
          onClickScript = copyFilePiece(searchObjectByName(allCategories, line->words[1], "Script"));

          callLine = copyLine(line);
          // If the object was already created
          if(objType == 2){
            ((ButtonLC *) obj)->onClickScript = onClickScript;
            ((ButtonLC *) obj)->callLine = callLine; 
            onClickScript = NULL;
            callLine = NULL;
          }

        } else if(strcmp(line->words[0], "pos") == 0){
          //Change positionVariables
          posX = atoi(line->words[1]);
          posY = atoi(line->words[2]);

          //If the object was already created
          if(objType == 2){
            ((ButtonLC *) obj)->posX = posX;
            ((ButtonLC *) obj)->posY = posY;
          } else if(objType == 3){
            ((TextLabelLC *) obj)->posX = posX;
            ((TextLabelLC *) obj)->posY = posY;
          }

        } else if(strcmp(line->words[0], "borderColor") == 0){
          // Change Temp Var borderColor
          if(line->wordsLength > 3){
            borderColor[0] = atoi(line->words[1]);
            borderColor[1] = atoi(line->words[2]);
            borderColor[2] = atoi(line->words[3]);
          }
          if(line->wordsLength > 4)
            borderColor[3] = atoi(line->words[4]);

          // If the object was already created
          if(objType == 2){
            ((ButtonLC *) obj)->borderColor[0] = borderColor[0];
            ((ButtonLC *) obj)->borderColor[1] = borderColor[1];
            ((ButtonLC *) obj)->borderColor[2] = borderColor[2];
            ((ButtonLC *) obj)->borderColor[3] = borderColor[3];
          } else if(objType == 3){
            ((TextLabelLC *) obj)->borderColor[0] = borderColor[0];
            ((TextLabelLC *) obj)->borderColor[1] = borderColor[1];
            ((TextLabelLC *) obj)->borderColor[2] = borderColor[2];
            ((TextLabelLC *) obj)->borderColor[3] = borderColor[3];
          }

        } else if(strcmp(line->words[0], "backgroundColor") == 0){
          // Change Temp Var backgroundColor
          if(line->wordsLength > 3){
            backgroundColor[0] = atoi(line->words[1]);
            backgroundColor[1] = atoi(line->words[2]);
            backgroundColor[2] = atoi(line->words[3]);
          }
          if(line->wordsLength > 4)
            backgroundColor[3] = atoi(line->words[4]);

          // If the object was already created
          if(objType == 2){
            ((ButtonLC *) obj)->backgroundColor[0] = backgroundColor[0];
            ((ButtonLC *) obj)->backgroundColor[1] = backgroundColor[1];
            ((ButtonLC *) obj)->backgroundColor[2] = backgroundColor[2];
            ((ButtonLC *) obj)->backgroundColor[3] = backgroundColor[3];

          } else if(objType == 3){
            ((TextLabelLC *) obj)->backgroundColor[0] = backgroundColor[0];
            ((TextLabelLC *) obj)->backgroundColor[1] = backgroundColor[1];
            ((TextLabelLC *) obj)->backgroundColor[2] = backgroundColor[2];
            ((TextLabelLC *) obj)->backgroundColor[3] = backgroundColor[3];

          }


        } else if(strcmp(line->words[0], "objType") == 0){
          if(strcmp(line->words[1], "Window") == 0){
            // Create Window
            WindowLC *tmp = (WindowLC *) malloc(sizeof(WindowLC));
            if (tmp == NULL) {
              printf("Error Creating Window");
              gameStruct->windows = window;
              freeGameStruct();
              return -1;
            }

            tmp->next = window;
            window = tmp;
            //window->next = tmp;
            window->buttons = NULL;
            window->textLabels = NULL;
            window->isVisible = 1;

            window->window = SDL_CreateWindow(
              "Test",
              sizeX,
              sizeY, 
              SDL_WINDOW_RESIZABLE
            );
            if(window->window == NULL){
              free(window);
              return -1;
            } 

            // Create Renderer
            SDL_Renderer* renderer = SDL_CreateRenderer(window->window, NULL);
            if(renderer == NULL){
              freeWindowLC(window);
              return -1;
            }
            SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
            window->renderer = renderer;
            
            objType = 1;
            obj = window;

          } else if(strcmp(line->words[1], "Button") == 0){
            // Create Button
            ButtonLC *button = createButton(sizeX, sizeY, fp->fileName, &text, &onClickScript, &callLine, borderColor, backgroundColor);
            if(button == NULL){
              printf("Error Creating a Button");
              freeWindowLC(window);
              return -1;
            }
            
            button->next = window->buttons;
            window->buttons = button;

            objType = 2;
            obj = button;

          } else if(strcmp(line->words[1], "TextLabel") == 0){
            // Create textLabel 
            TextLabelLC *textLabel = createTextLabel(sizeX, sizeY, fp->fileName, &text, borderColor, backgroundColor);
            if(textLabel == NULL){
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
      if(text != NULL)
        free(text);
      freeFilePiece(onClickScript);
      freeLine(callLine);

      fp = fp->next;
    }
  }
  Category *tempCat = allCategories;
  if(tempCat != NULL){
    if(strcmp(tempCat->name, "Info") == 0){
      allCategories = freeCategory(tempCat);

    } else {
      while(tempCat->next != NULL){
        if(strcmp(tempCat->next->name, "Info") == 0){
          tempCat->next = freeCategory(tempCat->next);
          break;
        }
        tempCat = tempCat->next;
      }

    }
  }
  //printCategories(allCategories);
  

  gameStruct->windows = window;
  gameStruct->allCategories = allCategories;

  gameStruct->timeSec = createEmptyNum(5);
  gameStruct->nbFrame = createEmptyNum(5);
  gameStruct->delta = createEmptyNum(2);

  // Execute on start Scripts
  Category *startingScripts = searchCategory(allCategories, "Start");
  FilePiece *startScriptsFile = startingScripts->files;
  while(startScriptsFile != NULL){
    executeCode(startScriptsFile, NULL, NULL);
    startScriptsFile = startScriptsFile->next;
  }
  
  loadHashmap("saveRessourceData", gameStruct->ressourceVars, deleteNum, createNumByConst);

  // Init SDL3
  if (!SDL_Init(SDL_INIT_VIDEO)){
    return -1;
  }
  
  // Main Loop
  SDL_Event event;
  
  int isRunning = 1;


  BigNumber *bigOne = createNumByInt(1);
  int frame = 0;

  unsigned long lastTicks = SDL_GetTicks();
  unsigned long actuTicks;
  int offset = 0;
 
  //int frame = 0;
  while(isRunning){ 
    frame++;
    addNumInto(gameStruct->nbFrame, bigOne);
    // wait time for target_fps
    int delayTime = 1000/TARGET_FPS - offset;
    if(delayTime < 0)
      delayTime = 1;
    SDL_Delay(delayTime);
  
    // Calcul wait error
    actuTicks = SDL_GetTicks();
    int delta = actuTicks - lastTicks;
    lastTicks = actuTicks;
    offset += delta - 1000/TARGET_FPS;
    if(gameStruct->delta != NULL){
      deleteNum(gameStruct->delta);
    }
    gameStruct->delta = createNumByInt(delta);

    // Inputs managing
    while(SDL_PollEvent(&event)){
      switch(event.type){
        case SDL_EVENT_KEY_DOWN:
          SDL_KeyboardEvent keyboardEvent = event.key;
          if(keyboardEvent.key == SDLK_ESCAPE)
            isRunning = 0;
        break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
          SDL_MouseButtonEvent mouseButtonEvent = event.button;
          if(mouseButtonEvent.button == 1){
            clickLeftMouse(window, mouseButtonEvent.x, mouseButtonEvent.y, mouseButtonEvent.windowID);
          }
          break;
      }

      if(event.type == SDL_EVENT_QUIT){
        isRunning = 0;
      }
    }

    // Call update functions
    Category *scripts = searchCategory(allCategories, "Update");
    if(scripts != NULL){
      FilePiece *scriptsFile = scripts->files;
      while(scriptsFile != NULL){
        executeCode(scriptsFile, NULL, NULL);
        scriptsFile = scriptsFile->next;
      }
    }

    if(frame >= TARGET_FPS){
      frame -= TARGET_FPS;
      addNumInto(gameStruct->timeSec, bigOne);
      //onUpdateS();  
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
  saveHashmap("saveRessourceData", gameStruct->ressourceVars, getStrOfNumNotFormated);
  
  
  // Close All And Stop Program
  freeGameStruct();
  SDL_Quit();  

  return 0;
}

