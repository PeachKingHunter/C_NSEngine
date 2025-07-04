#ifndef hachage
#define hachage

#define TABLE_SIZE 128
#define TOMBSTONE (( void *) -1)

#include <stddef.h> //Pour les NULL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hashentry {
char * key ;
void * value ;
} HashEntry ;

typedef struct hashmap {
    int size ;
    HashEntry * table ;
} HashMap;

unsigned long simple_hash(const char *str);
char *trim(char *str);

HashMap *hashmap_create();
void hashmap_destroy(HashMap *map, void (*freeValueFunc)(void *value));

void saveHashmap(char *fileName, HashMap *map, char *(*toStringValueFunc)(void *value));
void loadHashmap(char *fileName, HashMap *map, void (*freeValueFunc)(void *value), void *(*toStructOfValueFunc)(char *value));

int hashmap_insert(HashMap *map, const char *key, void *value, void (*freeValueFunc)(void *value));
void *hashmap_get(HashMap *map, const char *key); //Return value
void *hashmap_remove(HashMap *map, const char *key, void (*freeValueFunc)(void *value));

#endif
