
// CircularArray.h
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef short bool;
#define true  1
#define false 0

#define STRINGMAX  81   /* max number of chars in a string */
#define MAXSTRINGS 10   /* max number of strings in the circular array */

typedef char StringType[STRINGMAX];

// Declare global variables with extern
extern int HistoryFront;
extern int HistoryRear;
extern int HistoryCount;
extern StringType HistoryData[MAXSTRINGS];

void HistoryInit(void);
void HistoryAdvance(int *IndexPtr);
bool HistoryEmpty(void);
bool HistoryFull(void);
void HistoryInsert(StringType str);
void HistoryLookup(int number, StringType str);

#endif
