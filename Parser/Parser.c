#include "Parser.h"
#include <stdio.h>

// Récurcive
Category *parse(const char *dirPath) {
  if (dirPath == NULL)
    return NULL;

  // To return the Struct
  Category *allCategory = NULL;

#ifdef _WIN32
  // --------------------------------------------------------------------
  // ------------------ Windows implementation --------------------------

  char searchPath[512];
  snprintf(searchPath, sizeof(searchPath), "%s\\*", dirPath);

  // ------------------------ Files --------------------  
  
  WIN32_FIND_DATA findData;
  HANDLE hFind = FindFirstFile(searchPath, &findData);

  if (hFind == INVALID_HANDLE_VALUE)
    return NULL;

  do {
    const char *name = findData.cFileName;

    // Skip "." and ".."
    if (name[0] == '.' &&
        (name[1] == '\0' || (name[1] == '.' && name[2] == '\0')))
      continue;

    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s\\%s", dirPath, name);

    // Manage For if it's a file
    if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
      Category *toAdd = parseFile(fullPath);
      allCategory = fuseCategory(allCategory, toAdd);
    }
  } while (FindNextFile(hFind, &findData));

  FindClose(hFind);
  // ----------------- Copy above but for Folders ---------------
  
  hFind = FindFirstFile(searchPath, &findData);

  if (hFind == INVALID_HANDLE_VALUE)
    return NULL;

  do {
    const char *name = findData.cFileName;

    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
      continue;

    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s\\%s", dirPath, name);

    // Manage For if it's a folder
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      Category *toAdd = parse(fullPath);
      allCategory = fuseCategory(allCategory, toAdd);
    }
  } while (FindNextFile(hFind, &findData));

  FindClose(hFind);
  // ------------------------------------------------------------

#else
  // --------------------------------------------------------------------
  // -------------------- Linux implementation --------------------------
  //---------------------------- Files ----------------------------------

  // Open the folder
  DIR *dir = opendir(dirPath);
  if (dir == NULL)
    return NULL;

  // Go through children for files
  struct dirent *entry = readdir(dir);
  while (entry != NULL) {
    // Skip "." and ".."
    if (entry->d_name[0] == '.' &&
        (entry->d_name[1] == '\0' ||
         (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
      entry = readdir(dir);
      continue;
    }
    // Manage For if it's an file
    if (entry->d_type == DT_REG) {
      // printf("File: %s\n", entry->d_name);
      char newFilePath[500];
      strcpy(newFilePath, dirPath);
      strcat(newFilePath, entry->d_name);
      Category *toAdd = parseFile(newFilePath);
      allCategory = fuseCategory(allCategory, toAdd);
    }

    entry = readdir(dir);
  }

  closedir(dir);

  //---------------- Copy Of Above but for folders ---------------------

  // Open the folder
  dir = opendir(dirPath);
  if (dir == NULL)
    return NULL;

  // Go through children for folders
  entry = readdir(dir);
  while (entry != NULL) {
    // Skip "." and ".."
    if (entry->d_name[0] == '.' &&
        (entry->d_name[1] == '\0' ||
         (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
      entry = readdir(dir);
      continue;
    }
    // Manage For if it's an folder
    if (entry->d_type == DT_DIR) {
      // printf("Folder: %s\n", entry->d_name);

      char newDirPath[500];
      strcpy(newDirPath, dirPath);
      strcat(newDirPath, entry->d_name);
      strcat(newDirPath, "/");

      Category *toAdd = parse(newDirPath);
      allCategory = fuseCategory(allCategory, toAdd);
    }

    entry = readdir(dir);
  }

  closedir(dir);

  //------------------------------------------------------------
#endif

  return allCategory;
}

char *strndup(const char *s, size_t n) {
  // Find the length of the string, but not more than `n` characters
  size_t len = strnlen(s, n);

  // Allocate memory for the new string (+1 for the null terminator)
  char *new_str = malloc(len + 1);
  if (new_str == NULL) {
    return NULL; // Memory allocation failed
  }

  // Copy the string up to `n` characters
  strncpy(new_str, s, len);
  new_str[len] = '\0'; // Ensure null termination

  return new_str;
}

Category *fuseCategory(Category *allCategory, Category *toAdd) {
  while (toAdd != NULL) {
    Category *search = searchCategory(allCategory, toAdd->name);
    if (search == NULL) {
      Category *tmp = toAdd;
      toAdd = toAdd->next;

      tmp->next = allCategory;
      allCategory = tmp;
    } else {
      FilePiece *fp = search->files;
      if (fp == NULL) {
        printf("Error In Script an empty Category as been detected !!");
        return NULL;
      }
      while (fp->next != NULL) {
        fp = fp->next;
      }
      fp->next = toAdd->files;

      Category *tmp = toAdd;
      toAdd = toAdd->next;
      free(tmp->name);
      free(tmp);
    }
  }
  return allCategory;
}

Category *searchCategory(Category *allCategories, char *name) {
  while (allCategories != NULL) {
    if (strcmp(allCategories->name, name) == 0)
      return allCategories;
    allCategories = allCategories->next;
  }
  return NULL;
}

/*Category *parseFile(char *filePath){
  if(filePath == NULL)
    return NULL;

  FILE *file = fopen(filePath, "r");
  if(file == NULL)
    return NULL;

  // Count lines of this category
  long actuPos = ftell(file);
  printf("Initial pos: %ld\n", actuPos);

  Category *category = NULL;
  // For each lines
  char line[500];
  int lineInCategory = 0;
  while(fgets(line, 499, file) != NULL){
    printf("moving pos: %ld\n", ftell(file));
    long actuPos = ftell(file);
    if(line[0] == '.'){
      printf("---> Start category: %s", line);
      // Case where changing of category
      lineInCategory = 0;
      Category *tmp = category;
      category = (Category *) malloc(sizeof(Category));
      if(category == NULL){
        freeCategory(category);
        printf("Error parseFile: create category\n");
        fclose(file);
        return NULL;
      }

      // Get away the line return
      int tmpSize = strlen(line);
      if(line[tmpSize-1] == '\n')
        line[tmpSize-1] = '\0';

      category->name = strdup(line+1);
      category->next = tmp;

      FilePiece *filePiece = (FilePiece *) malloc(sizeof(FilePiece));
      category->files = filePiece;
      if(filePiece == NULL){
        printf("Error parseFile: create filePiece\n");
        freeCategory(category);
        fclose(file);
        return NULL;
      }
      filePiece->next = NULL;

      char *temp = filePath + strlen(filePath)-1;
      while(*temp != '/' && *temp != '\\')
        temp--;
      filePiece->fileName = strdup(temp+1);

      // Count lines of this category
      //long actuPos = ftell(file);
      if (actuPos < 0) {
        perror("ftell failed\n");
        fclose(file);
        return NULL;
      }
      printf("last pos: %ld\n", actuPos);
      printf("last Line: %s\n", line);
      int nbLine = 0;
      while(fgets(line, 499, file) != NULL){
        if(line[0] == '.')
          break;
        if(line[0] != '\n' && line[0] != '/')
          nbLine++;
      }

      printf("actu pos: %ld\n", ftell(file));
      printf("actu Line: %s", line);
      fseek(file, actuPos, SEEK_SET);
      printf("after pos: %ld\n", ftell(file));
      printf("after Line: %s", line);

      filePiece->data = (Line **) malloc(sizeof(Line *) * (nbLine+1));
      if(filePiece->data == NULL){
        printf("Error parseFile: create data of fp\n");
        freeCategories(category);
        fclose(file);
        return NULL;
      }
      filePiece->nbLine = nbLine;

    } else if(line[0] != '\n' && line[0] != '/') {
      printf("%s\n", line);
      // Case where we call parseLine with our line
      if(lineInCategory < category->files->nbLine){
        category->files->data[lineInCategory] = parseLine(line);
        lineInCategory++;
      } else {
        printf("Error: Exeed Array Size\n");
      }
    }
  }

  fclose(file);
  return category;
}*/

Category *parseFile(char *filePath) {
  if (filePath == NULL)
    return NULL;

  FILE *file = fopen(filePath, "r");
  if (file == NULL)
    return NULL;

  Category *category = NULL;
  char line[500];

  while (fgets(line, sizeof(line), file) != NULL) {
    if (line[0] == '.') {
      printf("---> Start category: %s", line);

      // Create new category
      Category *tmp = category;
      category = (Category *)malloc(sizeof(Category));
      if (category == NULL) {
        freeCategory(tmp);
        fclose(file);
        return NULL;
      }

      // Clean up newline from category name
      int len = strlen(line);
      if (line[len - 1] == '\n')
        line[len - 1] = '\0';

      category->name = strdup(line + 1);
      category->next = tmp;

      FilePiece *filePiece = (FilePiece *)malloc(sizeof(FilePiece));
      if (filePiece == NULL) {
        printf("Error: could not allocate filePiece\n");
        freeCategory(category);
        fclose(file);
        return NULL;
      }
      category->files = filePiece;
      filePiece->next = NULL;

      // Extract filename
      char *temp = filePath + strlen(filePath) - 1;
      while (temp > filePath && *temp != '/' && *temp != '\\')
        temp--;
      filePiece->fileName = strdup(temp + 1);

      // Dynamic buffer to store category lines
      char **linesBuffer = NULL;
      int linesBufferSize = 0;
      int linesBufferCap = 8;
      linesBuffer = malloc(sizeof(char *) * linesBufferCap);
      if (!linesBuffer) {
        perror("malloc linesBuffer failed");
        freeCategory(category);
        fclose(file);
        return NULL;
      }

      // Read until next category or EOF
      long curPos;
      while ((curPos = ftell(file)), fgets(line, sizeof(line), file)) {
        if (line[0] == '.')
          break;
        if (line[0] == '\n' || line[0] == '/')
          continue;

        if (linesBufferSize >= linesBufferCap) {
          linesBufferCap *= 2;
          char **tmpBuf = realloc(linesBuffer, sizeof(char *) * linesBufferCap);
          if (!tmpBuf) {
            perror("realloc failed");
            for (int i = 0; i < linesBufferSize; i++)
              free(linesBuffer[i]);
            free(linesBuffer);
            freeCategory(category);
            fclose(file);
            return NULL;
          }
          linesBuffer = tmpBuf;
        }

        linesBuffer[linesBufferSize++] = strdup(line);
      }

      // Allocate and parse lines
      filePiece->nbLine = linesBufferSize;
      filePiece->data = malloc(sizeof(Line *) * linesBufferSize);
      if (!filePiece->data) {
        perror("malloc filePiece->data failed");
        for (int i = 0; i < linesBufferSize; i++)
          free(linesBuffer[i]);
        free(linesBuffer);
        freeCategory(category);
        fclose(file);
        return NULL;
      }

      for (int i = 0; i < linesBufferSize; i++) {
        filePiece->data[i] = parseLine(linesBuffer[i]);
        free(linesBuffer[i]);
      }

      free(linesBuffer);

      // If last read line was a new category marker, reprocess it in next
      // iteration
      if (line[0] == '.')
        fseek(file, curPos, SEEK_SET); // This is safe, one-time rewind
    }
  }

  fclose(file);
  return category;
}

void printCategories(Category *category) {
  while (category != NULL) {
    FilePiece *fp = category->files;
    printf("Category: %s\n", category->name);
    while (fp != NULL) {
      for (int i = 0; i < fp->nbLine; i++) {
        printf("\t");
        for (int j = 0; j < fp->data[i]->wordsLength; j++) {
          printf("%s ", fp->data[i]->words[j]);
        }
        printf("\n");
      }

      fp = fp->next;
    }
    category = category->next;
  }
}

Line *copyLine(Line *line) {
  Line *new = (Line *)malloc(sizeof(Line));
  if (new == NULL)
    return NULL;

  new->wordsLength = line->wordsLength;
  new->words = (char **)malloc(sizeof(char *) * new->wordsLength);
  if (new->words == NULL) {
    free(new);
    return NULL;
  }

  for (int i = 0; i < new->wordsLength; i++) {
    new->words[i] = strdup(line->words[i]);
  }

  return new;
}

FilePiece *copyFilePiece(FilePiece *fp) {
  // Verif entry
  if (fp == NULL)
    return NULL;

  // Create new
  FilePiece *new = (FilePiece *)malloc(sizeof(FilePiece));
  if (new == NULL)
    return NULL;

  // Copy data
  new->fileName = strdup(fp->fileName);
  new->nbLine = fp->nbLine;
  new->next = NULL;
  new->data = (Line **)malloc(sizeof(Line *) * fp->nbLine);
  for (int i = 0; i < fp->nbLine; i++) {
    new->data[i] = copyLine(fp->data[i]);
  }

  return new;
}

Category *freeCategory(Category *category) {
  if (category == NULL)
    return NULL;

  Category *toReturn = category->next;

  freeFilePiece(category->files);

  free(category->name);

  free(category);
  return toReturn;
}

void freeCategories(Category *category) {
  while (category != NULL) {
    category = freeCategory(category);
  }
}

// Parse a line
Line *parseLine(char *string) {
  if (string == NULL)
    return NULL;

  Line *line = (Line *)malloc(sizeof(Line));
  if (line == NULL)
    return NULL;

  int inWord = 0;
  int inWordSpe = 0;
  int startWord = -1;
  int i = 0;

  // Count nb words
  int nbWord = 0;
  while (string[i] != '\0') {
    if (string[i] == '"') {
      if (inWordSpe == 0) {
        nbWord++;
        inWordSpe = 1;
      } else {
        inWordSpe = 0;
      }

    } else if (inWordSpe == 0 && string[i] == '=' && i > 1) {
      if (string[i - 1] == '=') {
        nbWord++;
      }

    } else if (inWordSpe == 0 && string[i] != ' ' && string[i] != ':' &&
               string[i] != '\n' && string[i] != '(' && string[i] != ')' &&
               string[i] != ';' && string[i] != '+' && string[i] != '=' &&
               string[i] != '\t' && string[i] != ',') {
      if (inWord == 0) {
        inWord = 1;
        nbWord++;
      }
    } else {
      inWord = 0;
    }

    i++;
  }
  // printf("%d\n", nbWord);

  // Create array for all words
  char **words = (char **)malloc(sizeof(char *) * nbWord);
  int j = 0;
  if (words == NULL) {
    free(line);
    return NULL;
  }

  // Parse the line
  inWord = 0;
  inWordSpe = 0;
  i = 0;
  int tempNBWord = 0;
  while (tempNBWord < nbWord) {
    if (string[i] == '"') {
      if (inWordSpe == 0) {
        if (inWord == 1) {
          words[j] = strndup(string + startWord, i - startWord);
          // printf("%s\n", words[j]);
          j++;
          inWord = 0;
          tempNBWord++;
        }

        startWord = i + 1;
        inWordSpe = 1;
      } else {
        words[j] = strndup(string + startWord, i - startWord);
        // printf("%s\n", words[j]);
        j++;
        tempNBWord++;

        inWord = 0;
        inWordSpe = 0;
      }

    } else if (inWordSpe == 0 && string[i] == '=' && i > 1) {
      if (string[i - 1] == '=') {
        words[j] = strndup(string + i, 1);
        // printf("%s\n", words[j]);
        j++;
        inWord = 0;
        tempNBWord++;
      }

    } else if (inWordSpe == 0 && string[i] != ' ' && string[i] != ':' &&
               string[i] != '\n' && string[i] != '(' && string[i] != ')' &&
               string[i] != ';' && string[i] != '+' && string[i] != '=' &&
               string[i] != '\t' && string[i] != ',') {
      if (inWord == 0) {
        inWord = 1;
        startWord = i;
      }
    } else {
      if (inWord == 1) {
        words[j] = strndup(string + startWord, i - startWord);
        // printf("%s\n", words[j]);
        j++;
        inWord = 0;
        tempNBWord++;
      }
    }

    i++;
  }

  line->words = words;
  line->wordsLength = nbWord;
  return line;
}

void freeLine(Line *line) {
  if (line == NULL)
    return;

  for (int i = 0; i < line->wordsLength; i++) {
    free(line->words[i]);
  }
  free(line->words);
  free(line);
}

void freeFilePiece(FilePiece *fp) {
  while (fp != NULL) {

    free(fp->fileName);
    for (int i = 0; i < fp->nbLine; i++) {
      freeLine(fp->data[i]);
    }
    free(fp->data);

    FilePiece *tmp = fp;
    fp = fp->next;
    free(tmp);
  }
}
