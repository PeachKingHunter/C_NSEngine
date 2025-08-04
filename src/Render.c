#include "Render.h"
#include "SDL3_image/SDL_image.h"
#include <SDL3/SDL_video.h>

void render(WindowLC *windowLC) {
  // Verif entry
  if (windowLC == NULL)
    return;

  // Only on visible windows
  if (windowLC->isVisible == 0) {
    render(windowLC->next);
    return;
  }

  // Draw background and clear window's drawing
  SDL_SetRenderDrawColor(windowLC->renderer, windowLC->backgroundColor[0],
                         windowLC->backgroundColor[1],
                         windowLC->backgroundColor[2],
                         windowLC->backgroundColor[3]); // window background
  SDL_RenderClear(windowLC->renderer);

  // Render all textLabels
  TextLabelLC *textLabels = windowLC->textLabels;
  while (textLabels != NULL) {
    renderTextLabel(textLabels, windowLC);

    textLabels = textLabels->next;
  }

  // Render all buttons
  ButtonLC *button = windowLC->buttons;
  while (button != NULL) {
    renderTextLabel(button->tl, windowLC);

    button = button->next;
  }

  // Set modifs
  SDL_RenderPresent(windowLC->renderer);
  // Call render on next window
  render(windowLC->next);
}

void renderTextLabel(TextLabelLC *textLabels, WindowLC *windowLC) {
  // Verif Entries
  if (textLabels == NULL || windowLC == NULL)
    return;

  if (textLabels->isVisible == false)
    return;

  int windowWidth = -1;
  int windowHeight = -1;
  if (SDL_GetWindowSize(windowLC->window, &windowWidth, &windowHeight) == 0) {
    windowWidth = windowLC->sizeX;
    windowHeight = windowLC->sizeY;
  }

  renderBackgroundAndBorder(textLabels, windowLC, windowWidth, windowHeight);
  renderText(textLabels, windowLC, windowWidth, windowHeight);
  renderImage(textLabels, windowLC, windowWidth, windowHeight);
}

void renderBackgroundAndBorder(TextLabelLC *textLabels, WindowLC *windowLC,
                               int windowWidth, int windowHeight) {
  // Calculs
  int posX = textLabels->posX * windowWidth / windowLC->sizeX;
  int posY = textLabels->posY * windowHeight / windowLC->sizeY;
  int sizeX = textLabels->sizeX * windowWidth / windowLC->sizeX;
  int sizeY = textLabels->sizeY * windowHeight / windowLC->sizeY;
  SDL_FRect rect = {posX, posY, sizeX, sizeY};

  // Background
  SDL_SetRenderDrawColor(windowLC->renderer, textLabels->backgroundColor[0],
                         textLabels->backgroundColor[1],
                         textLabels->backgroundColor[2],
                         textLabels->backgroundColor[3]);

  SDL_RenderFillRect(windowLC->renderer, &rect);

  // Border
  SDL_SetRenderDrawColor(windowLC->renderer, textLabels->borderColor[0],
                         textLabels->borderColor[1], textLabels->borderColor[2],
                         textLabels->borderColor[3]);
  SDL_RenderRect(windowLC->renderer, &rect);
}

void renderText(TextLabelLC *textLabels, WindowLC *windowLC, int windowWidth,
                int windowHeight) {
  // Create text on surface
  GameStruct *gameStruct = getGameStruct();
  TTF_Font *font = getGameStructFont();

  SDL_Color textColor = {textLabels->textColor[0], textLabels->textColor[1],
                         textLabels->textColor[2], textLabels->textColor[3]};
  SDL_Surface *textSurface = TTF_RenderText_Blended(
      font, textLabels->text, strlen(textLabels->text), textColor);
  if (textSurface != NULL) {
    // Get size from the surface
    float textWidth = textSurface->w * textLabels->textSize;
    float textHeight = textSurface->h * textLabels->textSize;

    // Convert surface to texture
    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(windowLC->renderer, textSurface);

    int posX = textLabels->posX + (textLabels->sizeX - textWidth) / 2;
    int posY = textLabels->posY + (textLabels->sizeY - textHeight) / 2;

    posX = posX * windowWidth / windowLC->sizeX;
    posY = posY * windowHeight / windowLC->sizeY;
    textWidth = textWidth * windowWidth / windowLC->sizeX;
    textHeight = textHeight * windowHeight / windowLC->sizeY;

    SDL_FRect textRect = {posX, posY, textWidth, textHeight};

    // Draw texture of the text
    SDL_RenderTexture(windowLC->renderer, textTexture, NULL, &textRect);
    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
  }
}

void renderImage(TextLabelLC *textLabels, WindowLC *windowLC, int windowWidth,
                 int windowHeight) {
  // Image Rendering
  SDL_Surface *imageSurface = IMG_Load(textLabels->imagePath);
  if (imageSurface != NULL) {

    // Get size from the surface
    float imageWidth = imageSurface->w;
    float imageHeight = imageSurface->h;

    // Convert surface to texture
    SDL_Texture *imageTexture =
        SDL_CreateTextureFromSurface(windowLC->renderer, imageSurface);

    int posX = textLabels->posX;
    int posY = textLabels->posY;

    posX = posX * windowWidth / windowLC->sizeX;
    posY = posY * windowHeight / windowLC->sizeY;
    imageWidth = textLabels->sizeX * 1. * windowWidth / windowLC->sizeX;
    imageHeight = textLabels->sizeY * 1. * windowHeight / windowLC->sizeY;

    SDL_FRect imageRect = {posX, posY, imageWidth, imageHeight};

    // Draw texture of the text
    SDL_RenderTexture(windowLC->renderer, imageTexture, NULL, &imageRect);
    SDL_DestroySurface(imageSurface);
    SDL_DestroyTexture(imageTexture);
  }
}
