#ifndef RENDER_H
#define RENDER_H

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Parser/Parser.h"
#include "../Hachage/Hachage.h"
#include "../BigNumberManager/BigNumberManager.h"

#include "GameStruct.h"

void render(WindowLC *windowLC);
void renderTextLabel(TextLabelLC *textLabels, WindowLC *windowLC);


#endif
