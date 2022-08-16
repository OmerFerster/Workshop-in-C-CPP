#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markov_chain.h"

#define TRUE 1
#define FALSE 0
#define MAX_LINE_SIZE 1000
#define MAX_TWEET_LEN 20
#define POINT "."
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
int check_corpus_path (char *path);
/**
 * The function checks if user handed 3 or 4 arguments as needed.
 * @param args_number The number of user given args
 * @return True if we get 3/ 4 args and False otherwise.
 */
int valid_num_of_args (int args_number);
/**
 * The function checks if all the args given are valid or not
 * @param args_num The number of arguments handed
 * @param path The input file path
 * @return True if all valid and False otherwise.
 */
int check_all_args (int args_num, char *path);
/**
 * The function adds states to database and every state adds its NextArray
 * in file to NextNodeCounter array
 * @param fp The file pointer
 * @param markov_chain The Markov Chain database
 * @param amount The amount of words to read from fp if 0 read all file
 */
void fill_database_helper (FILE *fp, MarkovChain *markov_chain, int amount);
/**
 * The function fill all the states to the database from the given text file
 * every state contains his own NextCounterArray also.
 * @param fp The given file pointer
 * @param words_to_read The amount of words to read from the file
 * @param markov_chain The markov chain database to add the data to.
 */
void fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain);
/**
 * The function generate all the tweets given from the input file text
 * @param markov_chain The database Chain with all data needed
 * @param tweet_num The number of tweets to generate
 */
void generate_tweets (MarkovChain *markov_chain, int tweet_num);

void print_list (LinkedList *database);

void print_counter_list (MarkovNode *markov_node);

///////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
  unsigned int seed = (unsigned int) strtol (argv[1], NULL, TEN);
  srand (seed);
  int num_of_tweets = (int) strtol (argv[2], NULL, TEN);
  if (check_all_args (argc, argv[3]) == FALSE)
    {
      return EXIT_FAILURE;
    }
  FILE *fp = fopen (argv[3], "r");
  if (fp == NULL)
    {
      return EXIT_FAILURE;
    }
  int amount_to_read = (argc == FOUR) ? 0 : (int) strtol (argv[4], NULL, TEN);
  LinkedList linked_list = {NULL, NULL, 0};
  MarkovChain markov_chain = {&linked_list};
  MarkovChain *pmarkov = &markov_chain;
  fill_database (fp, amount_to_read, pmarkov);
  generate_tweets (pmarkov, num_of_tweets);
  fclose (fp);
  free_markov_chain (&pmarkov);
  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////

int check_corpus_path (char *path)
{
  FILE *text_file = fopen (path, "r");
  if (text_file == NULL)
    {
      return FALSE;
    }
  fclose (text_file);
  return TRUE;
}

int valid_num_of_args (int args_number)
{
  if ((args_number == FOUR) || (args_number == FIVE))
    {
      return TRUE;
    }
  return FALSE;
}

int check_all_args (int args_num, char *path)
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

void fill_database_helper (FILE *fp, MarkovChain *markov_chain, int amount)
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
              return;
            }
          Node *new_node1 = add_to_database (markov_chain, word);
          word = strtok (NULL, TOK);
          actual_words++;
          if ((word != NULL) && (new_node1 != NULL)
              && (ends_with_point (new_node1->data->data) == FALSE))
            {
              Node *new_node2 = add_to_database (markov_chain, word);
              if ((new_node2 == NULL)
                  || (add_node_to_counter_list
                          (new_node1->data, new_node2->data)
                      == FALSE))
                {
                  free_markov_chain (&markov_chain);
                }
            }
        }
    }
  rewind (fp);
}

void fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  rewind (fp);
  fill_database_helper (fp, markov_chain, words_to_read);
}

void generate_tweets (MarkovChain *markov_chain, int tweet_num)
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

void print_list (LinkedList *database)
{
  Node *tmp = database->first;
  while (tmp != NULL)
    {
      fprintf (stdout, "Markov Node Data: %s\n", tmp->data->data);
      tmp = tmp->next;
    }
  fprintf (stdout, "%d\n", database->size);
}

void print_counter_list (MarkovNode *markov_node)
{
  if (markov_node == NULL)
    {
      printf ("Markov Node is NULL");
    }
  for (int i = 0; i < markov_node->size_of_next_array; i++)
    {
      NextNodeCounter *h = (markov_node->counter_list + i);
      printf ("next is: %s and his freq is: %d\n",
              h->markov_node->data, h->frequency);
    }
  printf ("array length: %d\n", markov_node->size_of_next_array);
  printf ("sum freqs: %d\n", frequencies_sum (markov_node));
}
