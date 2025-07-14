#ifndef BigNumberManager_H
#define BigNumberManager_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Structure For Big Numbers
typedef struct {
  char *number;
  int size;
  int usedSize;
} BigNumber;

// Create Numbers 
BigNumber *createEmptyNum(int size);
void *createNumByConst(char *string); // Return an BigNumber but in void for compatibility with other function
BigNumber *copyNum(BigNumber *bn);
void *createNumByInt(int num);
BigNumber *createSmallRandomNumber(int min, int max);

// Others
void printNum(BigNumber *bigNum);
char *getStrOfNum(BigNumber *num);
char *getStrOfNumNotFormated(void *num);

int calculateUsedSize(BigNumber *number);
int max(int n1, int n2);
void deleteNum(void *num);

// Arithmetic
BigNumber *addNumInto(BigNumber *num1, BigNumber *num2);
BigNumber *minusNumInto(BigNumber *nu1, BigNumber *num2);
BigNumber *multNumInto(BigNumber *nu1, BigNumber *num2);
BigNumber *divNumInto(BigNumber *num1, BigNumber *num2);

// Comparison
int areNumsEqual(BigNumber *num1, BigNumber *num2);
int isNumSup(BigNumber *num1, BigNumber *num2);
int isNumInf(BigNumber *num1, BigNumber *num2);

#endif
