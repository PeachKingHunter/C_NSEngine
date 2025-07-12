#include "Render.h"

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
  SDL_SetRenderDrawColor(windowLC->renderer, 30, 30, 30, 255);  // fond noir
  SDL_RenderClear(windowLC->renderer);

  // Render all textLabels
  TextLabelLC *textLabels = windowLC->textLabels;
  while(textLabels != NULL){
    renderTextLabel(textLabels, windowLC);

    textLabels = textLabels->next;
  }

  // Render all buttons
  ButtonLC *button = windowLC->buttons;
  while(button != NULL){
    renderTextLabel(button->tl, windowLC);

    button = button->next;
  }


  // Set modifs
  SDL_RenderPresent(windowLC->renderer);
  // Call render on next window
  render(windowLC->next);
}


void renderTextLabel(TextLabelLC *textLabels, WindowLC *windowLC){
  // Verif Entries
  if(textLabels == NULL || windowLC == NULL)
    return;

  // Fill the button's background
  SDL_SetRenderDrawColor(windowLC->renderer, textLabels->backgroundColor[0], textLabels->backgroundColor[1], textLabels->backgroundColor [2], textLabels->backgroundColor[3]);    
  SDL_FRect rect2 = {textLabels->posX, textLabels->posY, textLabels->sizeX, textLabels->sizeY};
  SDL_RenderFillRect(windowLC->renderer, &rect2);

  // Draw border of the button
  SDL_SetRenderDrawColor(windowLC->renderer, textLabels->borderColor[0], textLabels->borderColor[1], textLabels->borderColor [2], textLabels->borderColor[3]);   
  SDL_FRect rect = {textLabels->posX, textLabels->posY, textLabels->sizeX, textLabels->sizeY};
  SDL_RenderRect(windowLC->renderer, &rect);

  // Write text in the button
  SDL_SetRenderDrawColor(windowLC->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDebugText(windowLC->renderer, textLabels->posX + 20, textLabels->posY + 20, textLabels->text); 
  // TODO temp not optimise Just For Debug

}
