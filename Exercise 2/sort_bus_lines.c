#include "sort_bus_lines.h"

void bubble_sort (BusLine *start, BusLine *end)
{
  const BusLine **origin_start_adress = (const BusLine **) start;
  while (start != end)
    {
      while (start != end)
        {
          if ((*start).distance > (*(start + 1)).distance)
            {
              BusLine temp1 = *start;
              BusLine temp2 = *(start + 1);
              *(start + 1) = temp1; // The swapping process
              *start = temp2;
            }
          start++;

        }
      end -= 1;
      start = (BusLine *) origin_start_adress;
    }
}

void quick_sort (BusLine *start, BusLine *end)
{
  if (start < end)
    {
      BusLine *new_pivot = partition (start, end);
      quick_sort (start, new_pivot - 1);
      quick_sort (new_pivot + 1, end);
    }
}

BusLine *partition (BusLine *start, BusLine *end)
{
  BusLine *pivot = end;
  BusLine *left = start - 1;
  BusLine *unseen = start;
  while (unseen < pivot)
    {
      if ((*unseen).duration <= (*pivot).duration)
        {
          left++;
          BusLine temp1 = *left;
          BusLine temp2 = *unseen;
          *unseen = temp1; // The swapping process
          *left = temp2;
        }
      unseen++;
    }
  left++;
  BusLine temp1 = *left;
  BusLine temp2 = *unseen;
  *unseen = temp1; //swap pivot with left
  *left = temp2;
  return left;
}
