#include <stdio.h>
#include "test_bus_lines.h"

#define TRUE 1
#define FALSE 0

int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  bubble_sort (start, end);
  while (start < end)
    {
      if ((*start).distance > (*(start + 1)).distance)
        {
          fprintf (stdout, "TEST 1 FAILED: The array isn't sorted well\n");
          return FALSE;
        }
      start++;
    }
  fprintf (stdout, "TEST 1 PASSED: The array is sorted by distance\n");
  return TRUE;
}

int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  quick_sort (start, end);
  while (start < end)
    {
      if ((*start).duration > (*(start + 1)).duration)
        {
          fprintf (stdout, "TEST 3 FAILED: The array isn't sorted well\n");
          return FALSE;
        }
      start++;
    }
  fprintf (stdout, "TEST 3 PASSED: The array is sorted by duration\n");
  return TRUE;
}
/*
 * The function returns True if the buses got the exact same info and
 * False otherwise, checks line number, duration and distance values.
 */
int same_info (BusLine old, BusLine new);

int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original)
{
  int i;
  int found_him = FALSE;
  while (start_original <= end_original)
    {
      i = 0;
      found_him = FALSE;
      while (start_sorted <= end_sorted)
        {
          if (same_info (*start_sorted, *start_original) == TRUE)
            {
              found_him = TRUE;
              start_sorted++;
            }
          else
            {
              start_sorted++;
            }
          i++;
        }
      if (found_him == FALSE)
        {
          return FALSE;
        }
      else
        {
          start_sorted -= i;
        }
      start_original++;
    }
  return TRUE;
}

int same_info (BusLine old, BusLine new)
{
  if ((old.line_number == new.line_number) && (old.distance == new.distance)
      && (old.duration == new.duration))
    {
      return TRUE;
    }
  return FALSE;
}