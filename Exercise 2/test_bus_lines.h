#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"

/**
 * The function checks if The sorted array by Bubble sort algorithm works,
 * returns 1 if array is sorted properly otherwise returns 0
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);
/**
 * The function checks if The sorted array by Quick sort algorithm works,
 * returns 1 if array is sorted properly otherwise returns 0
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);
/**
 * The function checks if the original array got the same BusLines as
 * sorted array, returns 1 if equal and 0 otherwise
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);

#endif //EX2_REPO_TESTBUSLINES_H
