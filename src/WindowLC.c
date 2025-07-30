#include "WindowLC.h"

void freeWindowLC(WindowLC *windowLC) {
  while (windowLC != NULL) {
    SDL_DestroyWindow(windowLC->window);
    SDL_DestroyRenderer(windowLC->renderer);

    freeButtons(windowLC->buttons);
    freeTextLabels(windowLC->textLabels);
    free(windowLC->name);

    WindowLC *temp = windowLC;
    windowLC = windowLC->next;
    free(temp);
  }
}
