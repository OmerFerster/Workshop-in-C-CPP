#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort_bus_lines.h"
#include "test_bus_lines.h"

#define TEN 10
#define NINE_NINE_NINE 999
#define ALPH 1000
#define MEA 100
#define TRUE 1
#define FALSE 0
#define MAX_SIZE 60
#define PRINTS_INFO "%d,%d,%d\n"

/*
 * The function checks if the BusLine line number is between [1,999]
 * param line_number - The BusLine line number according to the user input.
 * returns True if valid and False otherwise.
 */
int checks_line_number (int line_number);
/*
 * The function checks if the BusLine distance is between [0,1000]
 * param distance - The BusLine distance according to the user input.
 * returns True if valid and False otherwise.
 */
int checks_distance (int distance);
/*
 * The function checks if the BusLine duration is between [10,100]
 * param duration - The BusLine duration according to the user input.
 * returns True if valid and False otherwise.
 */
int checks_duration (int duration);
/*
 * The function checks if the user entered only 1 argument and checks if
 * the argument is one of test/bubble/quick Mode.
 * param arc - The number of args from the user
 * param *arg[] - An array of arguments from the user
 * returns True if valid and False otherwise.
 */
int checks_args (int num_of_args, char *arg[]);
/*
 * The function checks if the BusLine Info is valid or not.
 * param line_number - The BusLine line number according to the user input.
 * param distance - The BusLine distance according to the user input.
 * param duration - The BusLine duration according to the user input.
 * returns True if valid and False otherwise.
 */
int is_valid_bus (int line_number, int distance, int duration);
/*
 * The "test" Mode function, checks if all the tests (1-4) works as expected.
 * The function uses Dynamic memory allocation.
 * param *arr - The address of the first place in the Array of BusLines.
 * param size - The size of the Array of BusLines.
 * returns EXIT_FAILURE if fails & EXIT_SUCCESS otherwise.
 */
int do_test (BusLine *arr, int size);
/*
 * The "bubble" Mode function, sorts the BusLine Array with Bubble sort algo.
 * param *arr - The address of the first place in the Array of BusLines.
 * param size - The size of the Array of BusLines.
 */
void do_bubble (BusLine *arr, int size);
/*
 * The "quick" Mode function, sorts the BusLine Array with Quick sort algo.
 * param *arr - The address of the first place in the Array of BusLines.
 * param size - The size of the Array of BusLines.
 */
void do_quick (BusLine *arr, int size);
/*
 * The function sort the array according the user argument.
 */
int handle_input (BusLine *arr, char arg[], int i);
/*
 * The function gets the number of Lines from the user as K argument
 * until the user puts a valid positive whole number.
 */
long get_k (long k);
/*
 * The function get the number of lines from the user input stdin
 */
long get_number_of_lines ();

int main (int argc, char *argv[])
{
  if (checks_args (argc, argv) == FALSE)
    {
      return EXIT_FAILURE;
    }
  long first_k = get_number_of_lines ();
  long k = get_k (first_k);
  BusLine *arr = malloc (k * sizeof (BusLine));  // Dynamic Memory Allocation
  if (arr == NULL)
    {
      return EXIT_FAILURE;
    }
  int i = 0;
  while (i != k)
    {
      char lines_info[MAX_SIZE];
      fprintf (stdout, "Enter line info. Then enter\n");
      if ((fgets (lines_info, sizeof (lines_info), stdin)) == NULL)
        {
          return EXIT_FAILURE;
        }
      int line_number, distance, duration;
      sscanf (lines_info, "%d,%d,%d", &line_number, &distance, &duration);
      if (is_valid_bus (line_number, distance, duration) == TRUE)
        {
          BusLine new_bus = {line_number, distance, duration};
          arr[i] = new_bus;
          i++;
        }
    }
  handle_input (arr, argv[1], i);
  free (arr);
  return EXIT_SUCCESS;
}

int checks_line_number (int line_number)
{
  if (line_number >= 1 && line_number <= NINE_NINE_NINE)
    {
      return TRUE;
    }
  fprintf (stdout, "ERROR: Line number should be an integer between"
                   " 1 and 999 (includes).\n");
  return FALSE;
}

int checks_distance (int distance)
{
  if (distance >= 0 && distance <= ALPH)
    {
      return TRUE;
    }
  fprintf (stdout, "ERROR: Distance should be an integer"
                   " between 0 and 1000 (includes).\n");
  return FALSE;
}

int checks_duration (int duration)
{
  if (duration >= TEN && duration <= MEA)
    {
      return TRUE;
    }
  fprintf (stdout, "ERROR: Duration should be an integer"
                   " between 10 and 100 (includes).\n");
  return FALSE;
}

int checks_args (int num_of_args, char *arg[])
{
  if (num_of_args != 2)
    {
      fprintf (stdout, "USAGE: The program must get 1 argument\n");
      return FALSE;
    }
  else
    {
      if ((strcmp (arg[1], "test") == 0) || (strcmp (arg[1], "quick") == 0)
          || (strcmp (arg[1], "bubble") == 0))
        {
          return TRUE;
        }
      fprintf (stdout, "USAGE: The program arg must be test/bubble/quick\n");
      return FALSE;
    }
}

int is_valid_bus (int line_number, int distance, int duration)
{
  if ((checks_line_number (line_number) == TRUE) &&
      (checks_distance (distance) == TRUE) &&
      (checks_duration (duration) == TRUE))
    {
      return TRUE;
    }
  return FALSE;
}

int do_test (BusLine *arr, int size)
{
  BusLine *copy1 = malloc (size * sizeof (BusLine));  // Dynamic Memory Alloc
  if (copy1 == NULL)
    {
      return EXIT_FAILURE;
    }
  BusLine *copy2 = malloc (size * sizeof (BusLine));  // Dynamic Memory Alloc
  if (copy2 == NULL)
    {
      return EXIT_FAILURE;
    }
  memcpy (copy1, arr, size * sizeof (BusLine));  // 1st copy
  memcpy (copy2, arr, size * sizeof (BusLine));  // 2nd copy
  is_sorted_by_distance (copy1, copy1 + (size - 1)); // 1st Test
  if (is_equal (copy1, copy1 + (size - 1), arr, arr + (size - 1)) == TRUE)
    {
      fprintf (stdout, "TEST 2 PASSED:"
                       " The array has the same items after sorting\n");
    }
  else
    {
      fprintf (stdout, "TEST 2 FAILED: The array hasn't have the"
                       " same items after sorting\n");
    }
  is_sorted_by_duration (copy2, copy2 + (size - 1)); // 3rd Test
  if (is_equal (copy2, copy2 + (size - 1), arr, arr + (size - 1)) == TRUE)
    {
      fprintf (stdout, "TEST 4 PASSED:"
                       " The array has the same items after sorting\n");
    }
  else
    {
      fprintf (stdout, "TEST 4 FAILED: The array hasn't have the"
                       " same items after sorting\n");
    }
  free (copy1);
  free (copy2);
  return EXIT_SUCCESS;
}

void do_bubble (BusLine *arr, int size)
{
  BusLine *end = arr + size - 1;
  bubble_sort (arr, arr + (size - 1));
  while (arr <= end)
    {
      printf (PRINTS_INFO, (*arr).line_number,
              (*arr).distance, (*arr).duration);
      arr++;
    }
}

void do_quick (BusLine *arr, int size)
{
  BusLine *end = arr + size - 1;
  quick_sort (arr, arr + (size - 1));
  while (arr <= end)
    {
      printf (PRINTS_INFO, (*arr).line_number,
              (*arr).distance, (*arr).duration);
      arr++;
    }
}

int handle_input (BusLine *arr, char arg[], int i)
{
  if (strcmp (arg, "test") == 0)
    {
      do_test (arr, i);
      return TRUE;
    }
  else if (strcmp (arg, "bubble") == 0)
    {
      do_bubble (arr, i);
      return TRUE;
    }
  else if (strcmp (arg, "quick") == 0)
    {
      do_quick (arr, i);
      return TRUE;
    }
  return FALSE;
}

long get_k (long k)
{
  while (k <= 0)
    {
      char new_buffer[MAX_SIZE];
      fprintf (stdout, "ERROR: The number must be larger than 0\n");
      fprintf (stdout, "Enter number of lines. Then enter\n");
      if ((fgets (new_buffer, sizeof (new_buffer), stdin)) == NULL)
        {
          return EXIT_FAILURE;
        }
      k = strtol (new_buffer, NULL, TEN);
    }
  return k;
}

long get_number_of_lines ()
{
  char buffer[MAX_SIZE];
  fprintf (stdout, "Enter number of lines. Then enter\n");
  if ((fgets (buffer, sizeof (buffer), stdin)) == NULL)
    {
      return EXIT_FAILURE;
    }
  else
    {
      return strtol (buffer, NULL, TEN);
    }
}
