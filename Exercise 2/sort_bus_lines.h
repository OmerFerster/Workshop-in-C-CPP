#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H

/**
 * A struct Type BusLine that contains information of each BusLine:
 * line_number - The BusLine number [1-999]
 * distance - How far the Destination from the university [0 - 1000]
 * duration - How much time it takes to reach the Destination [10 - 100]
 **/
typedef struct BusLine {
    int line_number, distance, duration;
} BusLine;

/**
 * The function use Bubble sort Algorithm to sort all of the BusLines
 * in ascending distance order: the nearest distance to most far one.
 * param BusLine *start: A pointer to first adress in array of BusLines.
 * param BusLine *end: A pointer to last adress in array of BusLines.
 * The function prints the sorted array.
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * The function use Quick sort Algorithm to sort all of the BusLines
 * in ascending duration order: the fastest duration to slowest one.
 * param BusLine *start: A pointer to first adress in array of BusLines.
 * param BusLine *end: A pointer to last adress in array of BusLines.
 * The function uses The partition helper function.
 * The function prints the sorted array.
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * The function works on a specific pivot element, at the end of the function
 * The pivot will be placed in his correct sorted index in array
 * The rest of the array will not be sorted yet.
 */
BusLine *partition (BusLine *start, BusLine *end);

#endif //EX2_REPO_SORTBUSLINES_H
