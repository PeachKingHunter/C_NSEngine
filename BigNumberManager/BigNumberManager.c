#include "BigNumberManager.h"

// BigNumber are char **
// Fonction that create an number equal to 0.
BigNumber *createEmptyNum(int size){
  // Special Case
  if(size == -1){
    char *number = (char *) malloc(sizeof(char));
    if(number == NULL)
      return NULL;

    number[0] = '\0';
    BigNumber *bigNumber = (BigNumber *) malloc(sizeof(BigNumber));
    if(bigNumber == NULL)
      return NULL;

    bigNumber->number = number;
    bigNumber->size = 1;
    bigNumber->usedSize = 1;
    return bigNumber;
  }

  // Minimal Size
  if (size < 2)
    size = 2;

  // Create the number
  char *number = (char *) malloc(sizeof(char) * size); // size-1 free slots and one for the end of the string and one for 0
  if(number == NULL){
    return NULL;
  }
  number[0]= '0';
  number[size-1] = '\0';

  // Empty Slots
  for(int i=1; i< size-1; i++)
    number[i]= '.';

  // Create the BigNumber struct 
  BigNumber *bigNumber = (BigNumber *) malloc(sizeof(BigNumber));
  if(bigNumber == NULL) {
    free(number);
    return NULL;
  }
  bigNumber->number = number;
  bigNumber->size = size;
  bigNumber->usedSize = 1;
  return bigNumber;
}


//Fonction that convert an const char * to an Big Number
void *createNumByConst(char *string){
  //printf("%s->Enter\n", string);
  // Verif entry
  if(string == NULL)
    return createEmptyNum(-1); 

  // Copy the string into number.
  char *number;
  int size = strlen(string) + 3;
  if(string[0] >= '0' && string[0] <= '9'){
    //printf("%s->size:%d\n", string, size);
    number = (char *) malloc(sizeof(char) * size);
    if(number == NULL){
      return NULL;
    }

    // Copy Of string into number
    for(int i=0; i<size-3; i++){
      number[i] = string[size-3-1-i];
    }

    number[size-0-1] = '\0';
    number[size-1-1] = '.';
    number[size-2-1] = '.';
  } else {
    number = strdup(string);
    if(number == NULL){
      return NULL;
    }
    size -= 3;
  }

  // Create the BigNumber struct 
  BigNumber *bigNumber = (BigNumber *) malloc(sizeof(BigNumber));
  if(bigNumber == NULL){
    free(number);
    return NULL;
  }
  bigNumber->number = number;
  bigNumber->size = size;
  if(number[0] >= '0' && number[0] <= '9'){
    bigNumber->usedSize = size - 3; // 3

  } else {
    bigNumber->usedSize = size;

  }
  return bigNumber;
}


BigNumber *copyNum(BigNumber *bn){
  BigNumber *copy = (BigNumber *) malloc(sizeof(BigNumber));
  if(copy == NULL)
    return NULL;
  copy->size = bn->size;
  copy->usedSize = bn->usedSize;

  copy->number = (char *) malloc(sizeof(char) * (bn->size));
  for(int i=0; i < bn->size; i++){
    copy->number[i] = bn->number[i];
  }
  //copy->number = strdup(bn->number);
  return copy;
}


// Fonction that print a bigNumber in the terminal
void printNum(BigNumber *bigNum){
  //printf("The number is: ");

  // Loop on the string and print each character.
  char *temp = bigNum->number;
  for(int i=bigNum->usedSize-1; i>= 0; i--) {
    printf("%c", temp[i]);
  }

  printf("\n");
}


void *createNumByInt(int num){
  char buffer[500];
  sprintf(buffer, "%d", num);
  return createNumByConst(buffer);
}


BigNumber *createSmallRandomNumber(int min, int max){
  // Minimal Size
  int size = 1;
  int temp = max;
  while(temp > 0){
    temp /= 10;
    size++;
  }

  // Create the number
  char *number = (char *) malloc(sizeof(char) * size); // size-1 free slots and one for the end of the string and one for 0
  if(number == NULL){
    return NULL;
  }
  number[size-1] = '\0';

  // Empty Slots
  for(int i=0; i< size-1; i++){
    int rdm = rand()%(max%10-min%10+1) + min%10;
    min /= 10;
    max /= 10;
    number[i]= '0' + rdm;
  }

  // Create the BigNumber struct 
  BigNumber *bigNumber = (BigNumber *) malloc(sizeof(BigNumber));
  if(bigNumber == NULL) {
    free(number);
    return NULL;
  }
  bigNumber->number = number;
  bigNumber->size = size;
  bigNumber->usedSize = size-1;
  return bigNumber;
 }


int calculateUsedSize(BigNumber *number){
  // Verif entry
  if(number == NULL)
    return -1;

  char *temp = number->number + number->size - 2;
  int i = number->size-1;
  while ( (*temp == '\0' || *temp == '.' || *temp == '0') && i > 1) {
    if(*temp == '0')
      *temp = '.';
    i--;
    temp--;
  }
  return i;
}


int isEqualToZero(BigNumber *number) {
  return number->usedSize == 1 && number->number[0] == '0';
}


int max(int n1, int n2){
  if (n1 > n2)
    return n1;
  return n2;
}

BigNumber *addNumInto(BigNumber *num1, BigNumber *num2){
  // Verif entry
  if(num1 == NULL || num2 == NULL)
    return NULL;

  // Calculate the size needed in num1 for the adding.
  int resSize = max(num1->usedSize, num2->usedSize);

  // Variables for after
  num1->usedSize = resSize;
  int cout = 0;
  char *resNum = num1->number;

  // Verif the space for the number
  if(num1->size < resSize+2){ // +1 for cout
    resNum = (char *) malloc(sizeof(char) * (resSize+3+1));
    if(resNum == NULL){
      return NULL;
    }
    resNum[resSize+3] = '\0';
    resNum[resSize+2] = '.';
    resNum[resSize+1] = '.';
    resNum[resSize+0] = '.';
    num1->size = resSize+3+1;
  }
  
  // Calculate while both numbers are not finished
  char *number1 = num1->number;
  char *number2 = num2->number;
  int i = 0;
  while(number1[i] != '.' && number2[i] != '.' && number1[i] != '\0' && number2[i] != '\0'){
    int temp = (number1[i] - '0') + (number2[i] - '0') + cout;
    if(temp > 9){
      temp -= 10;
      cout = 1;
    } else {
      cout = 0;
    }

    resNum[i] = temp + '0'; 
    i++;
  }
  
  // In The Case where the second number is smallest than the first
  int j=i;
  while(number1[j] != '.' && number1[j] != '\0'){
    int temp = (number1[j] - '0') + cout;
    if(temp > 9){
      temp -= 10;
      cout = 1;
    } else {
      cout = 0;
    }

    resNum[j] = temp + '0'; 
  j++;
  }

  // In The Case where the first number is smallest than the second 
  while(number2[i] != '.' && number2[i] != '\0'){
    int temp = (number2[i] - '0') + cout;
    if(temp > 9){
      temp -= 10;
      cout = 1;
    } else {
      cout = 0;
    }

    resNum[i] = temp + '0';
    i++;
  }
  
  i = max(i, j);

  // In the case where the number increase in size
  if(cout != 0){
    resNum[i] = cout + '0';
    num1->usedSize += 1;
    i++;
    cout = 0;
  }

  // fill empty slot
  while(resNum[i] != '.' && resNum[i] != '\0'){
    resNum[i] = '.';
    i++;
  }
  
  if(resNum != num1->number)
    free(num1->number);

  num1->number = resNum;
  return num1;
}


BigNumber *minusNumInto(BigNumber *num1, BigNumber *num2){
  // Verif entry
  if(num1 == NULL || num2 == NULL)
    return NULL;

  // Variables for after
  int cout = 0;
  char *resNum = num1->number;

  // If num1 is smallest than num2 -> return 0 in num1
  if(num2->usedSize > num1->usedSize){
    num1->number[0] = '0';
    for(int i=1; i < num1->usedSize; i++) {
      num1->number[i] = '.';
    }
    num1->usedSize = 1;
    return num1;
  }

  // If num1 is smallest than num2 -> return 0 in num1
  if(num2->usedSize == num1->usedSize && num1->number[num1->usedSize-1] < num2->number[num2->usedSize-1]){
    num1->number[0] = '0';
    for(int i=1; i < num1->usedSize; i++) {
      num1->number[i] = '.';
    }
    num1->usedSize = 1;
    return num1;
  }

  // Calculate while both numbers are not finished
  char *number1 = num1->number;
  char *number2 = num2->number;
  int i = 0;
  while(number1[i] != '.' && number2[i] != '.' && number1[i] != '\0' && number2[i] != '\0'){
    int temp = (number1[i] - '0') - (number2[i] - '0') - cout;
    if(temp < 0){
      temp += 10;
      cout = 1;
    } else {
      cout = 0;
    }

    resNum[i] = temp + '0'; 
    i++;
  }

  // In The Case where the second number is smallest than the first
  int j=i;
  while(number1[j] != '.' && number1[j] != '\0'){
    int temp = (number1[j] - '0') - cout;
    if(temp < 0){
      temp += 10;
      cout = 1;
    } else {
      cout = 0;
    }

    resNum[j] = temp + '0'; 
    j++;
  }

  // In The Case where the first number is smallest than the second 
  while(number2[i] != '.' && number2[i] != '\0'){
    int temp = - (number2[i] - '0') - cout;
    if(temp < 0){
      temp += 10;
      cout = 1;
    } else {
      cout = 0;
    }

    resNum[i] = temp + '0';
    i++;
  }
 
  i = max(i, j); 

  // fill empty slot
  while(resNum[i] != '.' && resNum[i] != '\0'){
    resNum[i] = '.';
    i++;
  }
  
  num1->usedSize = calculateUsedSize(num1);
  return num1;
}


BigNumber *multNumInto(BigNumber *num1, BigNumber *num2){
  if(!num1 || !num2)
    return NULL;

  //if(num2 == 0 Case TODO

  BigNumber *res = createEmptyNum(num1->usedSize+num2->usedSize + 2);
  if(res == NULL)
    return NULL;

  BigNumber *one = createNumByConst("1");
  if(one == NULL){
    deleteNum(res);
    return NULL;
  }

  BigNumber *tmp = copyNum(num2); 
  while(!isEqualToZero(tmp)){
    addNumInto(res, num1);
    minusNumInto(tmp, one); // Need Copy Fonction to not alter num2
  }

  num1->size = res->size;
  num1->usedSize = res->usedSize;
  free(num1->number);
  num1->number = res->number;
  free(res);
  deleteNum(one);
  deleteNum(tmp);
  return num1;
}


void deleteNum(void *num){
  if(num == NULL)
    return;

  free(((BigNumber *)num)->number);
  free(num);
}

// Display like 51.2E+9
char *getStrOfNum(BigNumber *num){
  if(num == NULL)
    return "ERROR getStrOfNum with num == NULL\n";

  //If it's an string not an number
  if(num->size > 0){
    if(num->number[0] < '0' || num->number[0] > '9'){
      return strdup(num->number);
    }
  }

  if(num->usedSize < 1){
    printf("Error");
    return NULL;
  }

  // What will be display
  int naturalDisplay = (num->usedSize-1)%3+1;
  printf("naturalDisplay: %d\n", naturalDisplay);
  int power = num->usedSize - naturalDisplay;
  printf("power: %d\n", power);

  int neededSize = naturalDisplay + 1; // +1 for \0

  // Space for the power of 10
  int temp = power;
  while(temp > 0){
    temp /= 10;
    neededSize++;
  }

  // E+ and decimal
  if(power > 0)
    neededSize += 2 + 2;


  // Create the string
  char *string = (char *) malloc(sizeof(char) * neededSize);
  string[neededSize-1] = '\0';
  int i=0;

  // fill it with the natural part
  temp = num->usedSize-1;
  while(naturalDisplay > 0){
    string[i] = num->number[temp];

    temp--;
    i++;
    naturalDisplay--;
  }

  //If not sup to 999
  if(power < 1)
    return string;


  // If Decimal part exist as value sup to 999
  string[i] = '.';
  i++;

  string[i] = num->number[temp];
  temp--;
  i++;

  if(power == 0){
    printf("Enter power==0");
    string[i-2] = '\0';
    return string;
  }

  // Power of 10
  string[i] = 'E';
  i++;

  string[i] = '+';
  i++;

  // Start by the end for write the power of 10
  i = neededSize - 2;
  temp = power;
  while(temp > 0){
    string[i] = temp%10 + '0';
    temp /= 10;
    i--;
  } 

  return string;
}

char *getStrOfNumNotFormated(void *num){
  if(num == NULL)
    return NULL;

  BigNumber *tmp = ((BigNumber *)num);
  char *string = (char *) malloc(sizeof(char) * (tmp->usedSize+1));
  if(string == NULL)
    return NULL;

  //bigNumber->usedSize = calculateUsedSize(bigNumber);
  //tmp->usedSize = calculateUsedSize(tmp);
  for(int i=0; i < tmp->usedSize; i++){
    string[i] = tmp->number[tmp->usedSize - i - 1];
  }
  string[tmp->usedSize] = '\0';

  return string;
}

int areNumsEqual(BigNumber *num1, BigNumber *num2){
  if(num1 == NULL || num2 == NULL){
    printf("Error Args NULL for areNumsEqual");
    return 0;
  }

  //printf("usedSize': %d\n", num1->usedSize);
  //printf("usedSize': %d\n", num2->usedSize);
  //if(num1->usedSize != num2->usedSize){
  //  return 0;
  //}

  for(int i=0; i < num2->usedSize; i++){
    if(num1->number[i] != num2->number[i])
      return 0;
  }

  return 1;
}


int isNumSup(BigNumber *num1, BigNumber *num2){
  // Verif entries
  if(num1 == NULL || num2 == NULL){
    printf("Error Args NULL for isNumSup");
    return 0;
  }

  // usedSize -> one biggest than the other ?
  if(num1->usedSize > num2->usedSize) 
    return 1;

  if(num1->usedSize < num2->usedSize) 
    return 0;

  // number by number
  for(int i=0; i < num1->usedSize; i++){
    if(num1->number[num1->usedSize -i -1] > num2->number[num1->usedSize -i -1])
      return 1;
    
    if(num1->number[num1->usedSize -i -1] < num2->number[num1->usedSize -i -1])
      return 0;

  }

  printf("isNumSup: Egal je crois pas sûre");
  return 0;
}


int isNumInf(BigNumber *num1, BigNumber *num2){
  return isNumSup(num2, num1);
}

