#ifndef OBJECTCREATOR_H
#define OBJECTCREATOR_H

#include "SDL3/SDL.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "../Parser/Parser.h"
#include "CodeInterpretor.h"
#include "GameStruct.h"
#include "Render.h"
#include "WindowLC.h"

#include <stdbool.h>
#include <time.h>

int sizeOperand(Line *line, void *obj, int objType, int *sizeX, int *sizeY);
int textOperand(Line *line, void *obj, int objType, char **text);
int onClickOperand(Line *line, void *obj, FilePiece **onClickScript, Line **callLine, Category *allCategories, int objType);
int posOperand(Line *line, int *posX, int *posY, void *obj, int objType);
int showOperand(Line *line, bool *isVisible, void *obj, int objType);
int hideOperand(Line *line, bool *isVisible, void *obj, int objType);
int borderColorOperand(Line *line, int *borderColor, void *obj, int objType);
int backgroundColorOperand(Line *line, int *backgroundColor, void *obj, int objType);
int textColorOperand(Line *line, int *textColor, void *obj, int objType);
int textSizeOperand(Line *line, int *textSize, void *obj, int objType);
int objTypeOperand(Line *line, WindowLC **window, int sizeX, int sizeY,
                   FilePiece *fp, bool isVisible, void **obj, int *objType,
                   char **text, FilePiece **onClickScript, Line **callLine,
                   int *borderColor, int *backgroundColor, int *textColor,
                   int textSize);


#endif
