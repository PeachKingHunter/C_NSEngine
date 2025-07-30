#include "CodeInterpretor.h"
#include "ButtonLC.h"
#include "TextLabelLC.h"
#include "WindowLC.h"
#include <SDL3/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

void clickLeftMouse(WindowLC *windowLC, float posX, float posY,
                    SDL_WindowID windowID) {
  // Verif entry
  if (windowLC == NULL)
    return;

  // Only on visible windows
  if (windowLC->isVisible == 0) {
    clickLeftMouse(windowLC->next, posX, posY, windowID);
    return;
  }

  if (SDL_GetWindowID(windowLC->window) != windowID) {
    clickLeftMouse(windowLC->next, posX, posY, windowID);
    return;
  }

  // Detect click in button
  ButtonLC *button = windowLC->buttons;
  while (button != NULL) {
    if (button->tl->isVisible == true) {
      if (posX > getButtonPosX(button) && posY > getButtonPosY(button) &&
          posX < getButtonPosX(button) + getButtonSizeX(button) &&
          posY < getButtonPosY(button) + getButtonSizeY(button)) {
        // printf("Button clicked\n");
        Line *line = button->callLine;
        if (line->wordsLength == 3) {
          for (int i = 0; i < atoi(line->words[2]); i++) {
            executeCode(button->onClickScript, button->callLine, NULL);
          }

        } else {
          executeCode(button->onClickScript, button->callLine, NULL);
        }
      }
    }
    button = button->next;
  }

  // Navigate through all buttons
  clickLeftMouse(windowLC->next, posX, posY, windowID);
}

void executeCode(FilePiece *code, Line *callLine, HashMap *beforeLocalVar) {
  // Verif entry
  if (code == NULL)
    return;

  // Create hashmap for variables
  HashMap *map = hashmap_create();
  GameStruct *gameStruct = getGameStruct();

  // Parametres
  int i = 2;
  if (callLine != NULL) {
    while (i + 1 < callLine->wordsLength) {
      // Resolve local variable
      BigNumber *val = getVar(beforeLocalVar, callLine, i + 1);
      if (val != NULL) {
        BigNumber *copy = copyNum(val);
        if (copy == NULL)
          continue;
        hashmap_insert(map, callLine->words[i], copy, deleteNum);
      } else {
        BigNumber *val = createNumByConst(callLine->words[i + 1]);
        hashmap_insert(map, callLine->words[i], val, deleteNum);
      }
      i += 2; // Key and value
    }
  }

  // printf("\n--execute Script--\n");
  //  Execute lines
  for (int i = 0; i < code->nbLine; i++) {
    Line *line = code->data[i];
    if (line != NULL) {
      if (line->wordsLength > 0) {

        // Write in the console for debugging
        if (strcmp("printf", line->words[0]) == 0) {
          for (int j = 1; j < line->wordsLength; j++) {

            BigNumber *val = getVar(map, line, j);
            if (val == NULL) {
              printf("%s", line->words[j]);

            } else {
              printNum(val);
            }
          }
          printf("\n");

          // Change the text of an button
        } else if (strcmp("stopScript", line->words[0]) == 0) {
          break;

        } else if (strcmp("changeTextOf", line->words[0]) == 0) {
          // Get The button and textLabel
          ButtonLC *button = searchButton(gameStruct->windows, line->words[1]);
          TextLabelLC *tl =
              searchTextLabel(gameStruct->windows, line->words[1]);
          if (button == NULL && tl == NULL) {
            printf("Button AND TextLabel NOT Found\n");
            continue;
          }

          // Get the text to write
          char buffer[500];
          int index = 0;
          buffer[index] = '\0';
          for (int i = 2; i < line->wordsLength; i++) {
            // the buffer have a limited size
            if (strlen(line->words[i]) - 1 + index > 500)
              break;

            // Copy each string
            BigNumber *val = getVar(map, line, i);
            if (val == NULL) {
              strcpy(buffer + index, line->words[i]);
              index += strlen(line->words[i]);
            } else {
              char *string = getStrOfNum(val);
              strcpy(buffer + index, string);
              index += strlen(string);
              free(string);
            }
            // buffer[index] = ' ';
            // index++;
          }
          buffer[index] = '\0';

          // Change the text
          if (button != NULL) {
            free(button->tl->text);
            button->tl->text = strdup(buffer);

          } else if (tl != NULL) {
            free(tl->text);
            tl->text = strdup(buffer);
          }
        } else if (strcmp("changeImageOf", line->words[0]) == 0) {
          // Get The button and textLabel
          ButtonLC *button = searchButton(gameStruct->windows, line->words[1]);
          TextLabelLC *tl =
              searchTextLabel(gameStruct->windows, line->words[1]);
          if (button == NULL && tl == NULL) {
            printf("Button AND TextLabel NOT Found\n");
            continue;
          }

          // Get the image path to write
          char buffer[500];
          int index = 0;
          buffer[index] = '\0';
          for (int i = 2; i < line->wordsLength; i++) {
            // the buffer have a limited size
            if (strlen(line->words[i]) - 1 + index > 500)
              break;

            strcpy(buffer + index, "images/");
            index += strlen("images/");
            // Copy each string
            BigNumber *val = getVar(map, line, i);
            if (val == NULL) {
              strcpy(buffer + index, line->words[i]);
              index += strlen(line->words[i]);
            } else {
              char *string = getStrOfNum(val);
              strcpy(buffer + index, string);
              index += strlen(string);
              free(string);
            }
            // buffer[index] = ' ';
            // index++;
          }
          buffer[index] = '\0';

          // Change the text
          if (button != NULL) {
            free(button->tl->imagePath);
            button->tl->imagePath = strdup(buffer);

          } else if (tl != NULL) {
            free(tl->imagePath);
            tl->imagePath = strdup(buffer);
          }

        } else if (strcmp("changeBackgroundColorOf", line->words[0]) == 0) {
          // Get The button or textLabel
          ButtonLC *button = searchButton(gameStruct->windows, line->words[1]);
          TextLabelLC *tl = NULL;
          if (button != NULL)
            tl = button->tl;

          if (tl == NULL)
            tl = searchTextLabel(gameStruct->windows, line->words[1]);

          if (tl == NULL) {
            printf("Button AND TextLabel NOT Found\n");
            continue;
          }

          // Change RGB color of the background
          if (line->wordsLength >= 3)
            tl->backgroundColor[0] = atoi(line->words[2]);
          if (line->wordsLength >= 4)
            tl->backgroundColor[1] = atoi(line->words[3]);
          if (line->wordsLength >= 5)
            tl->backgroundColor[2] = atoi(line->words[4]);

        } else if (strcmp("startScript", line->words[0]) == 0) {

          FilePiece *toStartScript = searchObjectByName(
              gameStruct->allCategories, line->words[1], "Script");
          executeCode(toStartScript, line, map);

          // Create an integer variable
        } else if (strcmp("int", line->words[0]) == 0) {
          if (line->wordsLength == 2) {
            hashmap_insert(map, line->words[1], createEmptyNum(0), deleteNum);

          } else if (line->wordsLength == 3) {
            BigNumber *tmpN = getVar(map, line, 2);
            if (tmpN != NULL) {
              hashmap_insert(map, line->words[1], copyNum(tmpN), deleteNum);

            } else {
              hashmap_insert(map, line->words[1],
                             createNumByConst(line->words[2]), deleteNum);
            }

          } else {
            printf("Too few or too many arg for int variable\n");
          }
        } else if (strcmp("intG", line->words[0]) == 0) {
          if (line->wordsLength == 2) {
            hashmap_insert(gameStruct->globalVars, line->words[1],
                           createEmptyNum(0), deleteNum);

          } else if (line->wordsLength == 3) {
            BigNumber *tempN = getVar(map, line, 2);
            if (tempN == NULL) {
              hashmap_insert(gameStruct->globalVars, line->words[1],
                             createNumByConst(line->words[2]), deleteNum);

            } else {
              hashmap_insert(gameStruct->globalVars, line->words[1],
                             copyNum(tempN), deleteNum);
            }

          } else {
            printf("Too few or too many arg for int variable\n");
          }

          // if operator
        } else if (strcmp("if", line->words[0]) == 0) {
          int res = resolveCondition(line, 1, map);

          // Skip to the end of the if loop
          int neededEnd = 0;
          if (res == 0)
            neededEnd++;

          while (neededEnd > 0) {
            i++;
            if (i >= code->nbLine)
              break;

            line = code->data[i];
            if (strcmp(line->words[0], "if") == 0) {
              neededEnd++;

            } else if (strcmp(line->words[0], "end") == 0 ||
                       line->words[0][0] == '}') {
              neededEnd--;
            }
          }

          // load an variable and if not exist, create it.
        } else if (strcmp("loadOrCreate", line->words[0]) == 0) {
          // TODO
          // char *val = NULL;
          if (line->wordsLength == 4) {
            hashmap_insert(gameStruct->ressourceVars, line->words[2],
                           createNumByConst(line->words[3]), deleteNum);

          } else {
            printf("Too few or too many arg for loadOrCreate variable\n");
          }

        } else if (strcmp("setSizeOf", line->words[0]) == 0 &&
                   line->wordsLength == 4) {
          int type = 0;
          void *instance =
              searchInstanceByName(gameStruct->windows, line->words[1], &type);
          if (instance == NULL || type == -1)
            continue;

          if (type == 2) {
            ButtonLC *button = instance;

            // For sizeX
            BigNumber *tmpN = getVar(map, line, 2);
            if (tmpN != NULL) {
              char *str = getStrOfNumNotFormated(tmpN);
              if (str != NULL) {
                button->tl->sizeX = atoi(str);
                free(str);
              }

            } else if (line->words[2][0] < '9' && line->words[2][0] > '0') {
              button->tl->sizeX = atoi(line->words[2]);
            }

            // TODO For sizeY

          } else if (type == 3) {
            TextLabelLC *tl = instance;
            // For sizeX
            BigNumber *tmpN = getVar(map, line, 2);
            if (tmpN != NULL) {
              char *str = getStrOfNumNotFormated(tmpN);
              if (str != NULL) {
                tl->sizeX = atoi(str);
                free(str);
              }

            } else if (line->words[2][0] < '9' && line->words[2][0] > '0') {
              tl->sizeX = atoi(line->words[2]);
            }

            // TODO Like above
          }

        } else if (strcmp("hideOf", line->words[0]) == 0 &&
                   line->wordsLength >= 2) {
          int type = 0;
          void *instance =
              searchInstanceByName(gameStruct->windows, line->words[1], &type);
          if (instance == NULL || type == -1)
            continue;

          if (type == 1) {
            WindowLC *obj = instance;
            SDL_HideWindow(obj->window);
            obj->isVisible = false;

          } else if (type == 2) {
            ButtonLC *obj = instance;
            obj->tl->isVisible = false;

          } else if (type == 3) {
            TextLabelLC *obj = instance;
            obj->isVisible = false;
          }

        } else if (strcmp("showOf", line->words[0]) == 0 &&
                   line->wordsLength >= 2) {
          int type = 0;
          void *instance =
              searchInstanceByName(gameStruct->windows, line->words[1], &type);
          if (instance == NULL || type == -1)
            continue;

          if (type == 1) {
            WindowLC *obj = instance;
            SDL_ShowWindow(obj->window);
            obj->isVisible = true;

          } else if (type == 2) {
            ButtonLC *obj = instance;
            obj->tl->isVisible = true;

          } else if (type == 3) {
            TextLabelLC *obj = instance;
            obj->isVisible = true;
          }

          // Arithmetic operators
        } else if ((strcmp("add", line->words[0]) == 0 ||
                    strcmp("set", line->words[0]) == 0 ||
                    strcmp("minus", line->words[0]) == 0 ||
                    strcmp("div", line->words[0]) == 0 ||
                    strcmp("mult", line->words[0]) == 0) &&
                   line->wordsLength == 3) {

          BigNumber *var1 = NULL;
          BigNumber *var2 = NULL;
          int var2IsToFree = 0;

          // Resolving variables
          var1 = getVar(map, line, 1);
          if (var1 == NULL)
            continue;

          var2 = getVar(map, line, 2);
          if (var2 == NULL) {
            var2 = createNumByConst(line->words[2]);
            // var2 = createNumByConst("10");
            var2IsToFree = 1;
          }

          if (strcmp("add", line->words[0]) == 0)
            addNumInto(var1, var2);
          if (strcmp("mult", line->words[0]) == 0)
            multNumInto(var1, var2);
          if (strcmp("div", line->words[0]) == 0)
            divNumInto(var1, var2);
          if (strcmp("minus", line->words[0]) == 0)
            minusNumInto(var1, var2);
          if (strcmp("set", line->words[0]) == 0) {
            free(var1->number);
            var1->number = strdup(var2->number);
            var1->size = var2->size;
            var1->usedSize = var2->usedSize;
          }

          // Clearing temp variables
          if (var2IsToFree == 1)
            deleteNum(var2);

        } else if (strcmp("randInto", line->words[0]) == 0 &&
                   line->wordsLength >= 4) {
          BigNumber *var1 = getVar(map, line, 1);
          // If Var Not Exist
          if (var1 == NULL) {
            hashmap_insert(map, line->words[1],
                           createSmallRandomNumber(atoi(line->words[2]),
                                                   atoi(line->words[3])),
                           deleteNum);
            continue;
          }
          // If Var Exist change it
          BigNumber *temp = createSmallRandomNumber(atoi(line->words[2]),
                                                    atoi(line->words[3]));
          if (temp == NULL) {
            printf("Error randInto\n");
            continue;
          }
          free(var1->number);
          var1->number = temp->number;
          var1->usedSize = temp->usedSize;
          var1->size = temp->size;
          free(temp);
        }
      }
    }
  }

  // Clear temp variables
  hashmap_destroy(map, deleteNum);
}

int resolveCondition(Line *line, int start, HashMap *vars) {
  // Verif entries
  if (line == NULL || vars == NULL)
    return -1;

  if (line->wordsLength < start + 3)
    return -1;

  // Variables
  int res = -1;

  BigNumber *var1 = NULL;
  int var1IsToFree = 0;
  BigNumber *var2 = NULL;
  int var2IsToFree = 0;

  // Resolving variables
  var1 = getVar(vars, line, start);
  if (var1 == NULL) {
    var1 = createNumByConst(line->words[start]);
    var1IsToFree = 1;
  }
  var2 = getVar(vars, line, start + 2);
  if (var2 == NULL) {
    var2 = createNumByConst(line->words[start + 2]);
    var2IsToFree = 1;
  }

  // Conditions
  if (line->words[start + 1][0] == '=') {
    res = areNumsEqual(var1, var2);
  } else if (line->words[start + 1][0] == '<') {
    res = isNumInf(var1, var2);
  } else if (line->words[start + 1][0] == '>') {
    res = isNumSup(var1, var2);
  }

  // Clearing temp variables
  if (var1IsToFree == 1)
    deleteNum(var1);

  if (var2IsToFree == 1)
    deleteNum(var2);

  // Return result of condition resolving
  return res;
}

// Get varaible by an string if exist
BigNumber *getVar(HashMap *vars, Line *line, int j) {
  // Verif entries
  if (line == NULL)
    return NULL;

  if (line->wordsLength < j || j < 0) {
    printf("Error get var line not have j word\n");
    return NULL;
  }

  // Get game struct
  GameStruct *gameStruct = getGameStruct();

  // Priority for preBuilt variables, local variables, global and ressource in
  // last
  if (strcmp(line->words[j], "time") == 0) {
    return gameStruct->timeSec;
  }

  if (strcmp(line->words[j], "delta") == 0)
    return gameStruct->delta;

  if (strcmp(line->words[j], "nbFrame") == 0)
    return gameStruct->nbFrame;

  void *val = NULL;
  if (vars != NULL) {
    val = hashmap_get(vars, line->words[j]);
    if (val != NULL)
      return val;
  }

  val = hashmap_get(gameStruct->globalVars, line->words[j]);
  if (val != NULL)
    return val;

  val = hashmap_get(gameStruct->ressourceVars, line->words[j]);
  if (val != NULL)
    return val;

  return NULL;
}
