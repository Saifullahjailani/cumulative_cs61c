#include "hashtable.h"

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * Helper Functions 
 */
void *getData(HashBucketEntry* , void*, HashTable*);
void resize(int , HashTable* );
/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  

  HashTable *newTable = malloc(sizeof(HashTable));
  if (NULL == newTable) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newTable->size = size;
  newTable->buckets = malloc(sizeof(struct HashBucketEntry *) * size);

  if (NULL == newTable->buckets) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  for (i = 0; i < size; i++) {
    newTable->buckets[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/* Task 1.2 */
void insertData(HashTable *table, void *key, void *data) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  // 2. Allocate a new hash bucket entry struct.
  // 3. Append to the linked list or create it if it does not yet exist.  
  HashBucketEntry *bucket = malloc(sizeof(HashBucketEntry));
  
  bucket->key = key;
  bucket->data = data;
  bucket->next = NULL;
  int index = (*(table->hashFunction))(key) % table->size;
  HashBucketEntry **ptr = &(table->buckets[index]);
  while(ptr && *ptr){
    ptr = &((*ptr)->next);
  }
  *ptr = bucket;
}

/* Task 1.3 */
void *findData(HashTable *table, void *key) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  // 2. Walk the linked list and check for equality with table->equalFunction.
  int index = (*(table->hashFunction))(key) % table->size;
  HashBucketEntry *ptr = table->buckets[index];
  return getData(ptr, key, table);
}

void *getData(HashBucketEntry* bucket, void* key, HashTable *table){
  if(bucket == NULL){
    return NULL;
  }
  int (*cmp)(void*, void*) = table-> equalFunction;
  if((*cmp)(key, bucket->key)){
    return bucket->data;
  }
  return getData(bucket->next, key, table);

}


/* Task 2.1 */
unsigned int stringHash(void *s) {
  char* string = (char*) s;
  unsigned int sum = 0;
  unsigned int epxponent = 1;
  int i = 0;
  while(string[i] != '\0'){
    sum += (epxponent * string[i]);
    i++;
    epxponent *= 10;
  }
  return sum ;
}



/* Task 2.2 */
int stringEquals(void *s1, void *s2) {
  return !strcmp((char*) s1,(char*) s2);
}



