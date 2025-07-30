#include "GameStruct.h"

void freeGameStruct() {
  GameStruct *gameStruct = getGameStruct();
  if (gameStruct == NULL)
    return;

  hashmap_destroy(gameStruct->globalVars, deleteNum);    // OK
  hashmap_destroy(gameStruct->ressourceVars, deleteNum); // OK
  freeCategories(gameStruct->allCategories);             // OK
  freeWindowLC(gameStruct->windows);
  deleteNum(gameStruct->timeSec);
  deleteNum(gameStruct->delta);
  deleteNum(gameStruct->nbFrame);

  // How To destroy constant ??
  // free(gameStruct);
}

FilePiece *searchObjectByName(Category *allCat, char *name, char *category) {
  // Verif entries
  if (allCat == NULL || name == NULL)
    return NULL;

  // Take the category of scripts
  Category *script = searchCategory(allCat, category);
  if (script == NULL)
    return NULL;

  // Navigate throught scripts
  FilePiece *filePiece = script->files;
  while (filePiece != NULL) {
    if (strcmp(name, filePiece->fileName) == 0) {
      return filePiece;
    }

    filePiece = filePiece->next;
  }

  return NULL;
}

GameStruct *getGameStruct() {
  static GameStruct gameStruct = {};
  return &gameStruct;
}

TTF_Font *getGameStructFont() {
  GameStruct *gS = getGameStruct();
  if (gS == NULL)
    return NULL;
  return gS->font;
}

ButtonLC *searchButton(WindowLC *windowLC, char *name) {
  // Verif entry
  if (windowLC == NULL)
    return NULL;

  // navigate throught buttons
  ButtonLC *button = windowLC->buttons;
  while (button != NULL) {
    if (strcmp(getButtonName(button), name) == 0)
      return button;

    button = button->next;
  }

  if (windowLC->next != NULL)
    return searchButton(windowLC->next, name);

  return NULL;
}

TextLabelLC *searchTextLabel(WindowLC *windowLC, char *name) {
  // Verif entry
  if (windowLC == NULL)
    return NULL;

  // navigate throught buttons
  TextLabelLC *tl = windowLC->textLabels;
  while (tl != NULL) {
    if (strcmp(tl->name, name) == 0)
      return tl;

    tl = tl->next;
  }

  if (windowLC->next != NULL)
    return searchTextLabel(windowLC->next, name);

  return NULL;
}

void *searchInstanceByName(WindowLC *windowLC, char *name, int *type) {
  // Verif entry
  if (windowLC == NULL || name == NULL || type == NULL) {
    *type = -1;
    return NULL;
  }

  void *res = searchButton(windowLC, name);
  if (res != NULL) {
    *type = 2;
    return res;
  }

  res = searchTextLabel(windowLC, name);
  if (res != NULL) {
    *type = 3;
    return res;
  }

  while(windowLC != NULL){
    if(strcmp(windowLC->name, name) == 0){      
      *type = 1;
      return windowLC;
    }
    windowLC = windowLC->next;
  }

  return NULL;
}
