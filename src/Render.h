#ifndef RENDER_H
#define RENDER_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../BigNumberManager/BigNumberManager.h"
#include "../Hachage/Hachage.h"
#include "../Parser/Parser.h"

#include "GameStruct.h"

void render(WindowLC *windowLC);
void renderTextLabel(TextLabelLC *textLabels, WindowLC *windowLC);
void renderBackgroundAndBorder(TextLabelLC *textLabels, WindowLC *windowLC, int windowWidth, int windowHeight);
void renderText(TextLabelLC *textLabels, WindowLC *windowLC, int windowWidth, int windowHeight);
void renderImage(TextLabelLC *textLabels, WindowLC *windowLC, int windowWidth, int windowHeight);

#endif
