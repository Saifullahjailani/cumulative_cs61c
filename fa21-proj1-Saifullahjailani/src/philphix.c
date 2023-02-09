/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */



/* Buffer Helper functions */

/* Creates Buffer with INITIAL_BUFFER_SIZE */

HashTable *dictionary;


void deleteBucket(HashBucketEntry *bucket){
  if(bucket != NULL){
    free(bucket->key);
    free(bucket->data);
    deleteBucket(bucket->next);
    free(bucket);
  }

}

void deleteTable(HashTable *table){
  for(int i = 0; i < table->size; i++){
    deleteBucket(table->buckets[i]);
  }
  free(table->buckets);
  free(table);
}
/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
#ifndef _PHILPHIX_UNITTEST
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();
  deleteTable(dictionary);
  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
 
  return 0;
}
#endif /* _PHILPHIX_UNITTEST */
#define INITIAL_LEN 100
typedef struct DynamicBuffer
{
  char* buffer;
  int size;
  int index;

} Buffer;

Buffer *createBuffer(){
  Buffer *buff = malloc(sizeof(Buffer));
  buff->buffer = malloc(sizeof(char) * (INITIAL_LEN+1));
  
  buff->size = INITIAL_LEN;
  buff->index = 0; 
  return buff;
}
void deleteBuffer(Buffer *buff){
  if(buff->buffer){
    free(buff->buffer);
  }
  free(buff);
}
void add(Buffer *buff, char c){
  if(buff->index >= buff->size){
    buff->size*=2;
    char *tmp = realloc(buff->buffer, sizeof(char) * (buff->size + 1));
    if(!tmp){
      fprintf(stderr, "reallocation failed");
    }
    buff->buffer = tmp;
  }
  buff->buffer[buff->index] = c;
  buff->index++;
  buff->buffer[buff->index] = '\0';
}

char *resetBuffer(Buffer *buff){
  char *ret = buff->buffer;
  buff->buffer = malloc(sizeof(char) * (INITIAL_LEN+1));
  buff->size = INITIAL_LEN;
  buff->index = 0;
  return ret;
}

void cancat(Buffer *buffer, char *c){
  int len = strlen(c);
  for(int i = 0; i < len; i++){
    add(buffer, c[i]);
}
}
char skipEmptySpace(FILE* f){
  char c;
  while ((c = getc(f)) && (isspace(c)))
  {}
  ungetc(c, f);
  return c;
}

char getKey(FILE* f, Buffer* buff){
  char c;
  while ((c = getc(f)) != EOF)
  {
    if(isalnum(c)){
      add(buff, c);
    } else if(isspace(c)){
      c = skipEmptySpace(f);
      break;
    }
  }
  return c;
}

char getVal(FILE* f, Buffer* buff){
  char c;
  while ((c = getc(f)) != EOF)
  {
    if (isspace(c)){
      c = skipEmptySpace(f);
      break;
    }else{
      add(buff, c);
    }
  }
  return c;
}
/* Task 3 */
void readDictionary(char *dictName) {
FILE *stream = fopen(dictName, "r");
  if(!stream){
    exit(61);
  }
  char c = 'A';
  Buffer *buff = createBuffer();
  while(c != EOF){
  c = getKey(stream, buff);
  if(c == EOF){
    break;
  }
  char *key = resetBuffer(buff);
  c = getVal(stream, buff);
  char *data = resetBuffer(buff);
  insertData(dictionary, key, data);
  }
 deleteBuffer(buff); 
 fclose(stream);
}
char *lowercase(Buffer *buff, int start){
  char *lower = malloc(sizeof(char) * (buff->size));
  for(int i = start; i < buff->index; i++){
    lower[i] = tolower(buff->buffer[i]);
  }
  for(int i = 0; i < start; i++){
    lower[i] = buff->buffer[i];
  }
  
  return lower;
}
char *authenticate(Buffer *buff){
    char *data = findData(dictionary, buff->buffer);
    if(data){
      return data;
    }
  char *lower = lowercase(buff, 1);
  data = findData(dictionary, lower);
  if(data){
    free(lower);
    return data;
  }
  lower[0] = tolower(lower[0]);
  data = findData(dictionary, lower);
  if(data){
    free(lower);
    return data;
  }
  free(lower);
  return NULL;
}

void putToStdout(char *buff){
  int len = strlen(buff);
  for(int i = 0; i < len; i++){
    putchar(buff[i]);
  }
}

void putBuffToStdout(Buffer *buff){
  putToStdout(buff->buffer);
  buff->index = 0;
  buff->buffer[0] = '\0';
}
/* Task 4 */
void processInput() {
  Buffer *buff = createBuffer();
  char c;
  while((c = getc(stdin)) != EOF){
    while ((c != EOF) && isalnum(c))
    {
      add(buff,c);
      c = getc(stdin);
    }
    if(buff->index > 0){
      char *data = authenticate(buff);
      if(data){
        putToStdout(data);
        buff->index = 0;
        buff->buffer[0] = '\0';
      }else{
        putBuffToStdout(buff);
      }
    }
      if(c != EOF){
      putchar(c);
      }
      
  }
  deleteBuffer(buff);
}