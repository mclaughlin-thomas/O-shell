#include "CircularArray.h"

int HistoryFront = 0;
int HistoryRear = 0;
int HistoryCount = 0;
StringType HistoryData[MAXSTRINGS];

/* The following function initializes the circular array. */
void HistoryInit(void)
   {
   HistoryFront = 0;
   HistoryRear = MAXSTRINGS - 1;
   HistoryCount = 0;
   }


/* This function advances, with wrap-around, the index number pointed to by IndexPtr. */
void HistoryAdvance(int *IndexPtr)
   {
   *IndexPtr = *IndexPtr + 1;
   if (*IndexPtr == MAXSTRINGS)
      *IndexPtr = 0;
   }


/* The following function returns true when and only when no data is in the circular array. */
bool HistoryEmpty(void)
   {
   if (HistoryCount == 0)
      return true;
   else
      return false;
   }


/* The following function returns true when the circular array is full. */
bool HistoryFull(void)
   {
   if (HistoryCount == MAXSTRINGS)
      return true;
   else
      return false;
   }


/* This function inserts the string str into the circular array, at the rear. If the circular array is full,
 * it writes over the oldest item in the array, the one at the front, and adjusts front and rear as needed. */
void HistoryInsert(StringType str)
   {
   if (HistoryFull())
      {
      HistoryAdvance(& HistoryFront);   // Throw the oldest string away.
      HistoryAdvance(& HistoryRear);
      strcpy(HistoryData[HistoryFront], str);
      }
   else
      {
      HistoryAdvance(& HistoryFront);
      strcpy(HistoryData[HistoryFront], str);
      HistoryCount++;
      }
   }


/* The following function looks up the string in the circular array that is at logical position number.
 * Thus, if number is 1, the function returns the latest string added to the circular array in the str parameter.
 * If number is HistoryCount, the function returns the oldest string in the circular array in the str parameter. */
void HistoryLookup(int number, StringType str)
   {
   int location;

   if ((number < 1) || (number > HistoryCount))
      strcpy(str, "Error: invalid number");
   else
      {
      location = HistoryFront - number + 1;
      if (location < 0)
         location = location + MAXSTRINGS;
      strcpy(str, HistoryData[location]);
      }
   }

