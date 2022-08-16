#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markov_chain.h"

#define POINT "."
#define TRUE 1
#define FALSE 0
#define NOT_IN_LIST 999
#define MAX_TWEET_LEN 1000

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  LinkedList *linked_data = (*markov_chain).database;
  int rand_ind = get_random_number (linked_data->size);
  while (markov_chain->is_last_fun (linked_list_ind (linked_data,
                                                     rand_ind)->data->data)
         == true)
    {
      rand_ind = get_random_number (linked_data->size);
    }
  return linked_list_ind (linked_data, rand_ind)->data;
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  int sum = frequencies_sum (state_struct_ptr);
  return chose_next_state (state_struct_ptr, sum);
}

void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  int i = 1;
  MarkovNode *first = (first_node == NULL)
                      ? get_first_random_node (markov_chain) : first_node;
  markov_chain->print_fun (first->data);
  while (i != max_length)
    {
      if (first->size_of_next_array != 0)
        {
          MarkovNode *next_node = get_next_random_node (first);
          if (markov_chain->is_last_fun (next_node->data) == true)
            {
              markov_chain->print_fun (next_node->data);
              return;
            }
          markov_chain->print_fun (next_node->data);
          first = next_node;
        }
      i++;
    }
}

void free_markov_chain (MarkovChain **markov_chain)
{
  Node *tmp = (*markov_chain)->database->first;
  while (tmp != NULL)
    {
      //free (tmp->data->data);
      (*markov_chain)->free_fun (tmp->data->data);
      free (tmp->data->counter_list);
      free (tmp->data);
      Node *next = tmp->next;
      free (tmp);
      tmp = next;
    }
}

bool add_node_to_counter_list (MarkovNode *first_node,
                               MarkovNode *second_node,
                               MarkovChain *markov_chain)
{
  if (first_node->size_of_next_array == 0)
    {
      first_node->counter_list->markov_node = second_node;
      first_node->counter_list->frequency = 1;
      first_node->size_of_next_array = 1;
      return true;
    }
  int index = is_in_counter_list (first_node, second_node);
  if (index != NOT_IN_LIST)
    {
      (first_node->counter_list + index)->frequency++;
      return true;
    }
  NextNodeCounter *new_counter_list = realloc (first_node->counter_list,
                                               (first_node->size_of_next_array
                                                + 1)
                                               * sizeof (NextNodeCounter));
  if (new_counter_list == NULL)
    {
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
      free_markov_chain (&markov_chain);
      return false;
    }
  first_node->counter_list = new_counter_list;
  (first_node->counter_list
   + first_node->size_of_next_array)->markov_node = second_node;
  (first_node->counter_list + first_node->size_of_next_array)->frequency = 1;
  first_node->size_of_next_array++;
  return true;
}

Node *get_node_from_database (MarkovChain *markov_chain, void *data_ptr)
{
  Node *tmp = markov_chain->database->first;
  while (tmp != NULL)
    {
      if (markov_chain->comp_fun (tmp->data->data, data_ptr) == 0)
        {
          return tmp;
        }
      tmp = tmp->next;
    }
  return NULL;
}

Node *add_to_database (MarkovChain *markov_chain, void *data_ptr)
{
  void *buffer = markov_chain->copy_fun (data_ptr);
  if (buffer == NULL)
    {
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
      free_markov_chain (&markov_chain);
      return NULL;
    }
  Node *node_to_check = get_node_from_database (markov_chain, buffer);
  if (node_to_check == NULL)
    {
      MarkovNode *new_node = malloc (sizeof (MarkovNode));
      if (new_node == NULL)
        {
          fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
          free_markov_chain (&markov_chain);
          return NULL;
        }
      new_node->data = buffer;
      if (add (markov_chain->database, new_node) == 0)
        {
          if (allocate_next_states_array (new_node) == TRUE)
            {
              return markov_chain->database->last;
            }
          else
            {
              free_markov_chain (&markov_chain);
              return NULL;
            }
        }
      else
        {
          fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
          free_markov_chain (&markov_chain);
          return NULL;
        }
    }
  markov_chain->free_fun (buffer);
  return node_to_check;
}

Node *linked_list_ind (LinkedList *ptr, int ind)
{
  int i = 0;
  Node *tmp = ptr->first;
  while (i != ind)
    {
      tmp = tmp->next;
      i++;
    }
  return tmp;
}

int is_in_counter_list (MarkovNode *dest, MarkovNode *check)
{
  NextNodeCounter *head = dest->counter_list;
  for (int i = 0; i < dest->size_of_next_array; i++)
    {
      MarkovNode *tmp = (head + i)->markov_node;
      if (tmp->data == check->data)
        {
          return i;
        }
    }
  return NOT_IN_LIST;
}

int allocate_next_states_array (MarkovNode *markov_node)
{
  markov_node->counter_list = malloc (sizeof (NextNodeCounter));
  if (markov_node->counter_list == NULL)
    {
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
      return FALSE;
    }
  markov_node->counter_list->markov_node = NULL;
  markov_node->counter_list->frequency = 0;
  markov_node->size_of_next_array = 0;
  return TRUE;
}

int get_random_number (int max_number)
{
  return rand () % max_number;
}

int frequencies_sum (MarkovNode *markov_node)
{
  int sum = 0;
  for (int i = 0; i < markov_node->size_of_next_array; i++)
    {
      sum += (markov_node->counter_list + i)->frequency;
    }
  return sum;
}

MarkovNode *chose_next_state (MarkovNode *markov_node, int sum)
{
  int index = get_random_number (sum);
  int i = 0;
  while (index >= 0)
    {
      index -= (markov_node->counter_list + i)->frequency;
      if (index <= -1)
        {
          return (markov_node->counter_list + i)->markov_node;
        }
      i++;
    }
  return NULL;
}

void init_chain (MarkovChain *markov_chain,
                 print_func print, comp_func comp, free_data frees,
                 copy_func cpy, is_last is_last_elem, LinkedList *database)
{
  markov_chain->print_fun = print;
  markov_chain->comp_fun = comp;
  markov_chain->copy_fun = cpy;
  markov_chain->free_fun = frees;
  markov_chain->is_last_fun = is_last_elem;
  markov_chain->database = database;
}
