#include <string.h>
#include <stdio.h>
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))
#define EMPTY -1
#define TWO 2
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define TEN 10
#define FINISH 100
#define THREE 3
#define NEW_LINE "\n"
#define INVALID_ARGS "Usage: The program must get 2 Arguments\n"

const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

typedef struct Cell {
    int number; // Cell number is between [1-100]
    int ladder_to;  // ladder_to represents the upgrade of the ladder
    int snake_to;  // snake_to represents the downgrade of the snake
} Cell;

/**
 * The function handles error with Allocations
 * @param error_msg The error msg to print to the user
 * @param database A pointer to A pointer to MarkovChain
 * @return EXIT_FAILURE.
 */
static int handle_error (char *error_msg, MarkovChain **database)
{
  printf ("%s", error_msg);
  if (database != NULL)
    {
      free_markov_chain (database);
    }
  return EXIT_FAILURE;
}
/**
 * The function creates the board game with size BOARD_SIZE square
 * @param cells The given Array of cells to create
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int create_board (Cell *cells[BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
    {
      cells[i] = malloc (sizeof (Cell));
      if (cells[i] == NULL)
        {
          for (int j = 0; j < i; j++)
            {
              free (cells[j]);
            }
          handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
          return EXIT_FAILURE;
        }
      *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
      int from = transitions[i][0];
      int to = transitions[i][1];
      if (from < to)
        {
          cells[from - 1]->ladder_to = to;
        }
      else
        {
          cells[from - 1]->snake_to = to;
        }
    }
  return EXIT_SUCCESS;
}
/**
 * The function fills the database given
 * @param markov_chain The given Markov Chain to fill the data to
 * @return EXIT_SUCCESS or EXIT_FAILURE.
 */
static int fill_database (MarkovChain *markov_chain)
{
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++)
    {
      add_to_database (markov_chain, cells[i]);
    }

  for (size_t i = 0; i < BOARD_SIZE; i++)
    {
      from_node = get_node_from_database (markov_chain, cells[i])->data;

      if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
          index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
          to_node = get_node_from_database (markov_chain, cells[index_to])
              ->data;
          add_node_to_counter_list (from_node, to_node, markov_chain);
        }
      else
        {
          for (int j = 1; j <= DICE_MAX; j++)
            {
              index_to = ((Cell *) (from_node->data))->number + j - 1;
              if (index_to >= BOARD_SIZE)
                {
                  break;
                }
              to_node = get_node_from_database (markov_chain, cells[index_to])
                  ->data;
              add_node_to_counter_list (from_node, to_node, markov_chain);
            }
        }
    }
  // free temp arr
  for (size_t i = 0; i < BOARD_SIZE; i++)
    {
      free (cells[i]);
    }
  return EXIT_SUCCESS;
}
/**
 * The function checks if the Cell is [100] or not
 * @param cell given Cell to check
 * @return True if cell is 100 or above and False otherwise.
 */
static bool game_over (Cell *cell);
/**
 * The function frees the Cell content from Heap memory
 * @param cell The given Cell to free
 */
static void frees_cell (Cell *cell);
/**
 * The function prints the Cell content according given format
 * @param cell The given Cell to print info from
 */
static void print_cell (Cell *cell);
/**
 * The function compare between 2 cells
 * @param first First Cell to compare
 * @param second Second Cell to compare
 * @return 0 if equal Cells and other number otherwise.
 */
static int cmp_cells (Cell *first, Cell *second);
/**
 * The function copies the Cell content to Dynamic Memory Allocation to Heap
 * @param cell The Cell to copy
 * @return The copied Cell from Heap location
 */
static Cell *cpy_cell (Cell *cell);
/**
 * The function generate all ways to the game.
 * @param markov_chain The given MarkovChain with all info
 * @param num_of_ways The number of ways to generate
 */
static void generate_ways (MarkovChain *markov_chain, int num_of_ways);

int main (int argc, char *argv[])
{
  if (argc != THREE)
    {
      fprintf (stdout, INVALID_ARGS);
      return EXIT_FAILURE;
    }
  unsigned int seed = (unsigned int) strtol (argv[1], NULL, TEN);
  srand (seed);
  int num_of_ways = (int) strtol (argv[TWO], NULL, TEN);
  LinkedList linked_list = {NULL, NULL, 0};
  MarkovChain markov_chain;
  init_chain (&markov_chain, (print_func) print_cell,
              (comp_func) cmp_cells, (free_data) frees_cell,
              (copy_func) cpy_cell, (is_last) game_over, &linked_list);
  MarkovChain *pmarkov = &markov_chain;
  if(fill_database (&markov_chain) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  generate_ways (&markov_chain, num_of_ways);
  free_markov_chain (&pmarkov);
  return EXIT_SUCCESS;
}

static bool game_over (Cell *cell)
{
  if (cell->number >= BOARD_SIZE)
    {
      return true;
    }
  return false;
}

static void frees_cell (Cell *cell)
{
  free (cell);
  cell = NULL;
}

static void print_cell (Cell *cell)
{
  if ((cell->ladder_to == EMPTY) && (cell->snake_to == EMPTY))
    {
      if (cell->number == FINISH)
        {
          printf ("[%d]", cell->number);
        }
      else
        {
          printf ("[%d] -> ", cell->number);
        }
    }
  else if ((cell->ladder_to != EMPTY) && (cell->snake_to == EMPTY))
    {
      if (cell->number == FINISH)
        {
          printf ("[%d]", cell->number);
        }
      else
        {
          printf ("[%d]-ladder to %d -> ", cell->number, cell->ladder_to);
        }
    }
  else if ((cell->ladder_to == EMPTY) && (cell->snake_to != EMPTY))
    {
      if (cell->number == FINISH)
        {
          printf ("[%d]", cell->number);
        }
      else
        {
          printf ("[%d]-snake to %d -> ", cell->number, cell->snake_to);
        }
    }
}

static int cmp_cells (Cell *first, Cell *second)
{
  return first->number - second->number;
}

static Cell *cpy_cell (Cell *cell)
{
  Cell *new_cell = malloc (sizeof (Cell));
  if (new_cell == NULL)
    {
      return NULL;
    }
  new_cell->number = cell->number;
  new_cell->snake_to = cell->snake_to;
  new_cell->ladder_to = cell->ladder_to;
  return new_cell;
}

static void generate_ways (MarkovChain *markov_chain, int num_of_ways)
{
  int counter = 0;
  while (counter != num_of_ways)
    {
      fprintf (stdout, "Random Walk %d: ", counter + 1);
      generate_random_sequence (markov_chain,
                                markov_chain->database->first->data,
                                MAX_GENERATION_LENGTH);
      fprintf (stdout, NEW_LINE);
      counter++;
    }
}
