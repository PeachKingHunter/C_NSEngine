#ifndef RENDER_H
#define RENDER_H

#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Parser/Parser.h"
#include "../Hachage/Hachage.h"
#include "../BigNumberManager/BigNumberManager.h"

#include "WindowLC.h"

void render(WindowLC *windowLC);

#endif
