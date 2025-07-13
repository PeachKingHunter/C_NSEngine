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
  SDL_FRect rect = {textLabels->posX, textLabels->posY, textLabels->sizeX, textLabels->sizeY};
  SDL_RenderFillRect(windowLC->renderer, &rect);

  // Draw border of the button
  SDL_SetRenderDrawColor(windowLC->renderer, textLabels->borderColor[0], textLabels->borderColor[1], textLabels->borderColor [2], textLabels->borderColor[3]);   
  //SDL_FRect rect = {textLabels->posX, textLabels->posY, textLabels->sizeX, textLabels->sizeY};
  SDL_RenderRect(windowLC->renderer, &rect);

  // Write text in the button
  //SDL_SetRenderDrawColor(windowLC->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
  //SDL_RenderDebugText(windowLC->renderer, textLabels->posX + 20, textLabels->posY + 20, textLabels->text); 

  // Create text on surface
  GameStruct *gameStruct = getGameStruct();
  TTF_Font *font = getGameStructFont();
  //SDL_Color textColor = { 0, 0, 0, 255 };
  SDL_Color textColor = { textLabels->textColor[0], textLabels->textColor[1], textLabels->textColor[2], textLabels->textColor[3] };
  SDL_Surface *textSurface = TTF_RenderText_Blended(font, textLabels->text, strlen(textLabels->text), textColor);
  if(textSurface == NULL)
    return;

  // Get size from the surface
  float textWidth = textSurface->w * textLabels->textSize;
  float textHeight = textSurface->h * textLabels->textSize;

  // Convert surface to texture
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(windowLC->renderer, textSurface);

  int posX = textLabels->posX + (textLabels->sizeX - textWidth)/2 + 2;
  int posY = textLabels->posY + (textLabels->sizeY - textHeight)/2;
  SDL_FRect textRect = {posX, posY, textWidth, textHeight };

  // Draw texture of the text
  SDL_RenderTexture(windowLC->renderer, textTexture, NULL, &textRect);    
  SDL_DestroySurface(textSurface);
  SDL_DestroyTexture(textTexture);
}
