#include "Hachage.h"

unsigned long simple_hash(const char *str) {
    // Verif Entrée
    if (str == NULL)
        return 0;

    // Calcul du Hash pour la clé str
    unsigned long hash = 0;
    while (*str != '\0') {
        hash = (hash * 31) + *str;
        str += 1;
    }

    return hash % TABLE_SIZE;
}


// Fonction donnée
char *trim(char *str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }
    return str;
}

HashMap *hashmap_create() {
    // Allocation de la hashmap et de ses composantes
    HashMap *map = (HashMap *) malloc(sizeof(HashMap));
    if (map == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    map->size = TABLE_SIZE;

    map->table = (HashEntry *) malloc(sizeof(HashEntry) * TABLE_SIZE);
    if (map->table == NULL) {
        perror("Erreur d'allocation mémoire pour le tableau");
        free(map);
        return NULL;
    }

    // Initialisation des valeurs du tableau de la hashmap à NULL
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->table[i].key = NULL;
        map->table[i].value = NULL;
    }
    return map;
}


void hashmap_destroy(HashMap *map, void (*freeValueFunc)(void *value)) {
    // Vérification de l'entrée
    if (map == NULL) 
        return;

    for (int i = 0; i < map->size; i++) {
        if (map->table[i].key != NULL && map->table[i].key != TOMBSTONE) {
            free(map->table[i].key);
            map->table[i].key = NULL;
        }
        if (map->table[i].value != NULL || (int *) map->table[i].value != NULL) {
            if(freeValueFunc != NULL)
              freeValueFunc(map->table[i].value);
            map->table[i].value = NULL;
        }
    }
    free(map->table);
    free(map);
}



void saveHashmap(char *fileName, HashMap *map, char *(*toStringValueFunc)(void *value) ){
  // Verif des entrées
  if(fileName == NULL || map == NULL)
    return ;

  // Ouvre le fichier en écriture
  FILE *file = fopen(fileName, "wb");
  if(file == NULL)
    return ;

  // Parcours de du tableau
  for(int i=0; i < map->size; i++){
    HashEntry *hashEntry = (map->table)+i;
    if(hashEntry->key != NULL && hashEntry->key != TOMBSTONE){
      char *str = toStringValueFunc(hashEntry->value);
      if(str != NULL){
        fprintf(file, "%s -> %s\n", hashEntry->key, str);
        //printf("%s -> %s\n", hashEntry->key, str);
        free(str);
      }
    }
  }

  fclose(file);
}


void loadHashmap(char *fileName, HashMap *map, void (*freeValueFunc)(void *value), void *(*toStructOfValueFunc)(char *value)){
  // Verif des entrées
  if(fileName == NULL || map == NULL)
    return ;

  // Ouvre le fichier en écriture
  FILE *file = fopen(fileName, "r");
  if(file == NULL)
    return ;

  char buffer[9999];
  while(fgets(buffer, 9999, file) != NULL){
    char name[500];
    char value[500];
    if(sscanf(buffer, "%s -> %s ", name, value) == 2){
      //printf("|%s|\n", value);
      hashmap_insert(map, name, toStructOfValueFunc(value), freeValueFunc);
    }
  }

  fclose(file);
}



int hashmap_insert(HashMap *map, const char *key, void *value, void (*freeValueFunc)(void *value)) {
  // Vérification des entrées
  if (map == NULL || key == NULL)
      return -1;

  // Récupération de la position d'insertion
  unsigned long pos = simple_hash(key);

  // Insertion dans la hashmap avec probing linéaire
  for (int i=0; i < TABLE_SIZE; i++) {
    if (map->table[(pos+i)%TABLE_SIZE].key == NULL || map->table[(pos+i)%TABLE_SIZE].key == TOMBSTONE) {
        map->table[(pos+i)%TABLE_SIZE].key = strdup(key);
        map->table[(pos+i)%TABLE_SIZE].value = value;
        return (pos+i)%TABLE_SIZE;
    } else if(map->table[(pos+i)%TABLE_SIZE].key != NULL) {
      if(strcmp(map->table[(pos+i)%TABLE_SIZE].key, key) == 0){
        freeValueFunc(map->table[(pos+i)%TABLE_SIZE].value); 
        map->table[(pos+i)%TABLE_SIZE].value = value;    
        return (pos+i)%TABLE_SIZE;
      }
    }

  }
  return -1;
}


// Renvoie la value et non la HashEntry en tant que void *
void *hashmap_get(HashMap *map, const char *key) {
    // Vérification des entrées
    if (map == NULL || key == NULL)
        return NULL;

    // Position de l'élément à partir de la clé
    unsigned long index = simple_hash(key);

    // Récupère toujours avec le probing linéaire pour la gestion des collisions
    for (int i = 0; i < TABLE_SIZE; i++) {
        int try = (index + i) % TABLE_SIZE;
        HashEntry *entry = &map->table[try];
        if (entry->key == NULL)
            return NULL;
        if (entry->key != NULL && entry->key != TOMBSTONE && strcmp(entry->key, key) == 0)
            return entry->value;
    }
    return NULL;
}


//Return value
void *hashmap_remove(HashMap *map, const char *key, void (*freeValueFunc)(void *value)) {
    // Vérification des entrées
    if (map == NULL || key == NULL)
        return NULL;

    // Position de l'élément s'il n'y a pas de collision
    unsigned long index = simple_hash(key);

    for (int i = 0; i < map->size; i++) {
        int current = (index + i) % map->size;
        HashEntry *entry = &map->table[current];
        if (entry->key == NULL)
            return NULL;
        if (entry->key != NULL && entry->key != TOMBSTONE && strcmp(entry->key, key) == 0) {
            free(entry->key);
            if(freeValueFunc != NULL)
              freeValueFunc(map->table[i].value);
            entry->key = TOMBSTONE;
            entry->value = NULL;
            return entry->value; // Return the value
        }
    }
    return NULL;
}






