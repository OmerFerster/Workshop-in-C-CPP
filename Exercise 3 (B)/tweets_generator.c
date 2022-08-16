#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markov_chain.h"

#define TRUE 1
#define FALSE 0
#define MAX_LINE_SIZE 1000
#define MAX_TWEET_LEN 20
#define POINT "."
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define TEN 10
#define NEW_LINE "\n"
#define TOK "\n\r\t "
#define INVALID_CORPUS_PATH "Error: The text corpus file is invalid\n"
#define INVALID_ARGS "Usage: The program must get 3 / 4 Arguments\n"

/**
 * The function checks if the given Text corpus path is valid or not.
 * @param path The given path to the Test corpus from the user input
 * @return True if valid and False otherwise.
 */
static int check_corpus_path (char *path);
/**
 * The function checks if user handed 3 or 4 arguments as needed.
 * @param args_number The number of user given args
 * @return True if we get 3/ 4 args and False otherwise.
 */
static int valid_num_of_args (int args_number);
/**
 * The function checks if all the args given are valid or not
 * @param args_num The number of arguments handed
 * @param path The input file path
 * @return True if all valid and False otherwise.
 */
static int check_all_args (int args_num, char *path);
/**
 * The function adds states to database and every state adds its NextArray
 * in file to NextNodeCounter array
 * @param fp The file pointer
 * @param markov_chain The Markov Chain database
 * @param amount The amount of words to read from fp if 0 read all file
 */
static int fill_database_helper (FILE *fp,
                                  MarkovChain *markov_chain, int amount);
/**
 * The function fill all the states to the database from the given text file
 * every state contains his own NextCounterArray also.
 * @param fp The given file pointer
 * @param words_to_read The amount of words to read from the file
 * @param markov_chain The markov chain database to add the data to.
 * return EXIT_SUCCESS if okay and EXIT_FAILURE otherwise.
 */
static int fill_database (FILE *fp, MarkovChain *markov_chain, int amount);
/**
 * The function generate all the tweets given from the input file text
 * @param markov_chain The database Chain with all data needed
 * @param tweet_num The number of tweets to generate
 */
static void generate_tweets (MarkovChain *markov_chain, int tweet_num);
/**
 * The function frees the given string from Heap Memory
 * @param str The given string to free
 */
static void frees_string (char *str);
/**
 * The function prints the given string
 * @param str The given string to print
 */
static void print_string (char *str);
/**
 * The function copies the given string to another Dynamic allocated string
 * @param str The given string to copy
 * returns The copied string from Heap Memory
 */
static void *copy_string (char *str);
/**
 * The function checks if the given state ends with a POINT .
 * @param state The given state to check
 * @return True if does and False otherwise
 */
static bool ends_with_point (void *state);

int main (int argc, char *argv[])
{
  if(valid_num_of_args (argc) == FALSE) {return EXIT_FAILURE;}
  unsigned int seed = (unsigned int) strtol (argv[1], NULL, TEN);
  srand (seed);
  int num_of_tweets = (int) strtol (argv[TWO], NULL, TEN);
  if (check_all_args (argc, argv[THREE]) == FALSE)
    {
      return EXIT_FAILURE;
    }
  FILE *fp = fopen (argv[THREE], "r");
  if (fp == NULL)
    {
      return EXIT_FAILURE;
    }
  int amount = (argc == FOUR) ? 0 : (int) strtol (argv[FOUR], NULL, TEN);
  LinkedList linked_l = {NULL, NULL, 0};
  MarkovChain markov_chain;
  init_chain (&markov_chain, (print_func) print_string,
              (comp_func) strcmp, (free_data) frees_string,
              (copy_func) copy_string, (is_last) ends_with_point, &linked_l);
  MarkovChain *pmarkov = &markov_chain;
  if(fill_database (fp, &markov_chain, amount) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  fclose (fp);
  generate_tweets (&markov_chain, num_of_tweets);
  free_markov_chain (&pmarkov);
  return EXIT_SUCCESS;
}

static int check_corpus_path (char *path)
{
  FILE *text_file = fopen (path, "r");
  if (text_file == NULL)
    {
      return FALSE;
    }
  fclose (text_file);
  return TRUE;
}

static int valid_num_of_args (int args_number)
{
  if ((args_number == FOUR) || (args_number == FIVE))
    {
      return TRUE;
    }
  return FALSE;
}

static int check_all_args (int args_num, char *path)
{
  if (valid_num_of_args (args_num) == FALSE)
    {
      fprintf (stdout, INVALID_ARGS);
      return FALSE;
    }
  if (check_corpus_path (path) == FALSE)
    {
      fprintf (stdout, INVALID_CORPUS_PATH);
      return FALSE;
    }
  return TRUE;
}

static int fill_database_helper (FILE *fp,
                                  MarkovChain *markov_chain, int amount)
{
  int actual_words = 0;
  char line[MAX_LINE_SIZE];
  while ((fgets (line, sizeof (line), fp) != NULL))
    {
      char *word = strtok (line, TOK);
      actual_words++;
      while (word != NULL)
        {
          if ((amount > 0) && (actual_words > amount))
            {
              return EXIT_SUCCESS;
            }
          Node *new_node1 = add_to_database (markov_chain, word);
          word = strtok (NULL, TOK);
          actual_words++;
          if ((word != NULL) && (new_node1 != NULL)
              && (ends_with_point (new_node1->data->data) == false))
            {
              Node *new_node2 = add_to_database (markov_chain, word);
              if ((new_node2 == NULL)
                  || (add_node_to_counter_list
                          (new_node1->data, new_node2->data, markov_chain)
                      == FALSE))
                {
                  free_markov_chain (&markov_chain);
                  return EXIT_FAILURE;
                }
            }
        }
    }
  rewind (fp);
  return EXIT_SUCCESS;
}

static int fill_database (FILE *fp, MarkovChain *markov_chain, int amount)
{
  rewind (fp);
  if(fill_database_helper (fp, markov_chain, amount) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

static void generate_tweets (MarkovChain *markov_chain, int tweet_num)
{
  int counter = 0;
  while (counter != tweet_num)
    {
      fprintf (stdout, "Tweet %d: ", counter + 1);
      generate_random_sequence (markov_chain, NULL, MAX_TWEET_LEN);
      fprintf (stdout, NEW_LINE);
      counter++;
    }
}

static void frees_string (char *str)
{
  free (str);
  str = NULL;
}

static void print_string (char *str)
{
  if (ends_with_point (str) == true)
    {
      fprintf (stdout, "%s", str);
      return;
    }
  fprintf (stdout, "%s ", str);
}

static void *copy_string (char *str)
{
  char *dest = malloc (strlen (str) + 1);
  if (dest == NULL)
    {
      return NULL;
    }
  strcpy (dest, str);
  return dest;
}

static bool ends_with_point (void *state)
{
  char *word = state;
  while (*word != 0)
    {
      word++;
    }
  word--;
  if (strcmp (word, POINT) == 0)
    {
      return true;
    }
  return false;
}
