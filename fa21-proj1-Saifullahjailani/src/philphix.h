#ifndef _PHILPHIX_H
#define _PHILPHIX_H





/*Returns a char* from buffer*/

/* The pointer to store the dictionary for replacement entries */
extern struct HashTable *dictionary;

extern void readDictionary(char *dictName);

extern void processInput();

#endif /* _PHILPHIX_H */