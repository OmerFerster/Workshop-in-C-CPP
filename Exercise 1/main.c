#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

int num_of_args (int num);
int test_invalid (char str[]);
int command_invalid (char str[]);
int k_invalid (char k[]);
int file_invalid (char input[], char output[]);
int run_all_tests ();

int main (int argc, char *argv[])
{
  if (num_of_args (argc) == FALSE)
    {
      return EXIT_FAILURE;
    }
  if (argc == 2 && (test_invalid (argv[1]) == FALSE))
    {
      return EXIT_FAILURE;
    }
  else if (argc == 2 && (test_invalid (argv[1]) == TRUE))
    {
      if (run_all_tests () == FALSE)
        {
          return EXIT_FAILURE;
        }
      else
        {
          return EXIT_SUCCESS;
        }
    }
  if (command_invalid (argv[1]) == FALSE)
    {
      return EXIT_FAILURE;
    }
  if (k_invalid (argv[2]) == FALSE)
    {
      return EXIT_FAILURE;
    }
  if (file_invalid (argv[3], argv[4]) == FALSE)
    {
      return EXIT_FAILURE;
    }
  long k = strtol (argv[2], NULL, 10);
  FILE *input_file = fopen (argv[3], "r");
  FILE *output_file = fopen (argv[4], "w");
  if (strcmp (argv[1], "encode") == 0)
    {
      char line[1024];
      while (fgets (line, sizeof (line), input_file) != NULL)
        {
          encode (line, (int) k);
          fputs (line, output_file);
        }
      fclose (output_file);
      fclose (input_file);
    }
  else if (strcmp (argv[1], "decode") == 0)
    {
      char line[1024];
      while (fgets (line, sizeof (line), input_file) != NULL)
        {
          decode (line, (int) k);
          fputs (line, output_file);
        }
      fclose (output_file);
      fclose (input_file);
    }
  return EXIT_SUCCESS;
}

int num_of_args (int num)
{
  if (num != 2 && num != 5)
    {
      fprintf (stderr, "The program receives 1 or 4 arguments only.\n");
      return FALSE;
    }
  return TRUE;
}

int test_invalid (char str[])
{
  int res = strcmp (str, "test");
  if (res == 1 || strlen (str) != 4)
    {
      fprintf (stderr, "Usage: cipher test\n");
      return FALSE;
    }
  return TRUE;
}

int command_invalid (char str[])
{
  int res1 = strcmp (str, "encode");
  int res2 = strcmp (str, "decode");
  if (res1 != 0 && res2 != 0)
    {
      fprintf (stderr, "The given command is invalid.\n");
      return FALSE;
    }
  return TRUE;
}

int k_invalid (char k[])
{
  int ind = 0;
  int temp = 1;
  while (k[ind] != 0 && k[temp] != 0)
    {
      if (k[ind] == 46 || isdigit (k[temp]) == 0)
        {
          fprintf (stderr, "The given shift value is invalid.\n");
          return FALSE;
        }
      ind++;
      temp++;
    }
  return TRUE;
}

int file_invalid (char input[], char output[])
{
  FILE *input_file = fopen (input, "r");
  FILE *output_file = fopen (output, "w");
  if (input_file == NULL || output_file == NULL)
    {
      fprintf (stderr, "The given file is invalid.\n");
      return FALSE;
    }
  fclose (input_file);
  return TRUE;
}

int run_all_tests ()
{
  if (test_encode_non_cyclic_lower_case_positive_k () == FALSE)
    {
      return FALSE;
    }
  if (test_encode_cyclic_lower_case_special_char_positive_k () == FALSE)
    {
      return FALSE;
    }
  if (test_encode_non_cyclic_lower_case_special_char_negative_k () == FALSE)
    {
      return FALSE;
    }
  if (test_encode_cyclic_lower_case_negative_k () == FALSE)
    {
      return FALSE;
    }
  if (test_encode_cyclic_upper_case_positive_k () == FALSE)
    {
      return FALSE;
    }
  if (test_decode_non_cyclic_lower_case_positive_k () == FALSE)
    {
      return FALSE;
    }
  if (test_decode_cyclic_lower_case_special_char_positive_k () == FALSE)
    {
      return FALSE;
    }
  if (test_decode_non_cyclic_lower_case_special_char_negative_k () == FALSE)
    {
      return FALSE;
    }
  if (test_decode_cyclic_lower_case_negative_k () == FALSE)
    {
      return FALSE;
    }
  if (test_decode_cyclic_upper_case_positive_k () == FALSE)
    {
      return FALSE;
    }
  return TRUE;
}
