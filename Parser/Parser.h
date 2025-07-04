#ifndef PARSER_H
#define PARSER_H

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structure for a line
typedef struct line{
  char **words;
  int wordsLength;
} Line;

typedef struct file_piece{
  char *fileName;
  Line **data;
  int nbLine;
  struct file_piece *next;
} FilePiece;

// Structure for a file by category in it
typedef struct Category{
  char *name;
  FilePiece *files;
  struct Category *next;
} Category;

// Other
Category *fuseCategory(Category *cat1, Category *cat2);
Category *searchCategory(Category *allCategories, char *name);
void printCategories(Category *category);
Line *copyLine(Line *line);
FilePiece *copyFilePiece(FilePiece *fp);

// Parsing
Category *parse(char *dirPath);
Category *parseFile(char *file);
Line *parseLine(char *string);

// Free
void freeLine(Line *line);
Category *freeCategory(Category *category);
void freeCategories(Category *category);
void freeFilePiece(FilePiece *fp);

#endif

