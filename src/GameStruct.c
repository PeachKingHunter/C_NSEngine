#include "GameStruct.h"

void freeGameStruct(){
  GameStruct *gameStruct = getGameStruct();
  if(gameStruct == NULL)
    return ;

  hashmap_destroy(gameStruct->globalVars, deleteNum); //OK
  hashmap_destroy(gameStruct->ressourceVars, deleteNum); //OK
  freeCategories(gameStruct->allCategories); //OK
  freeWindowLC(gameStruct->windows);
  deleteNum(gameStruct->timeSec);
  deleteNum(gameStruct->delta);

  // How To destroy constant ??
  //free(gameStruct);
}

void render(WindowLC *windowLC) {
  // Verif entry
  if(windowLC == NULL)
    return;

  // Only on visible windows
  if(windowLC->isVisible == 0) {
    render(windowLC->next);
    return ;
  }

  // Draw background and clear window's drawing
  SDL_SetRenderDrawColor(windowLC->renderer, 0, 0, 0, 255);  // fond noir
  SDL_RenderClear(windowLC->renderer);


  // Render all buttons
  ButtonLC *button = windowLC->buttons;
  while(button != NULL){
    // Draw border of the button
    SDL_SetRenderDrawColor(windowLC->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_FRect rect = {button->posX, button->posY, button->sizeX, button->sizeY};
    SDL_RenderRect(windowLC->renderer, &rect);

    // Write text in the button
    SDL_SetRenderDrawColor(windowLC->renderer, 150, 100, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(windowLC->renderer, button->posX + 20, button->posY + 20, button->text); // TODO temp not optimise Just For Debug

    button = button->next;
  }
  // Render all textLabels
  TextLabelLC *textLabels = windowLC->textLabels;
  while(textLabels != NULL){
    // Draw border of the button
    SDL_SetRenderDrawColor(windowLC->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_FRect rect = {textLabels->posX, textLabels->posY, textLabels->sizeX, textLabels->sizeY};
    SDL_RenderRect(windowLC->renderer, &rect);

    // Write text in the button
    SDL_SetRenderDrawColor(windowLC->renderer, 150, 100, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugText(windowLC->renderer, textLabels->posX + 20, textLabels->posY + 20, textLabels->text); // TODO temp not optimise Just For Debug

    textLabels = textLabels->next;
  }

  // Set modifs
  SDL_RenderPresent(windowLC->renderer);
  // Call render on next window
  render(windowLC->next);
}


FilePiece *searchObjectByName(Category *allCat, char *name, char *category){
  // Verif entries
  if(allCat == NULL || name == NULL)
    return NULL;
    
  // Take the category of scripts
  Category *script = searchCategory(allCat, category);
  if(script == NULL)
    return NULL;

  // Navigate throught scripts
  FilePiece *filePiece = script->files;
  while(filePiece != NULL){
    if(strcmp(name, filePiece->fileName) == 0){
      return filePiece;
    }

    filePiece = filePiece->next;
  }

  return NULL;
}


GameStruct *getGameStruct(){
  static GameStruct gameStruct = {};
  return &gameStruct;
}


ButtonLC *searchButton(WindowLC *windowLC, char *name){
  // Verif entry
  if(windowLC == NULL)
    return NULL;

  // navigate throught buttons
  ButtonLC *button = windowLC->buttons;
  while(button != NULL){
    if(strcmp(button->name, name) == 0)
      return button;

    button = button->next;
  }

  if(windowLC->next != NULL)
    return searchButton(windowLC->next, name);

  return NULL;
}

