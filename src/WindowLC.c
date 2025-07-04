#include "WindowLC.h"

void freeWindowLC(WindowLC *windowLC){
  while(windowLC != NULL){
    SDL_DestroyWindow(windowLC->window);
    SDL_DestroyRenderer(windowLC->renderer);

    freeButtons(windowLC->buttons);
    freeTextLabels(windowLC->textLabels);
    
    WindowLC *temp = windowLC;
    windowLC = windowLC->next;
    free(temp);
  }
}
