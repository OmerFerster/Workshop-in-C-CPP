#ifndef _MARKOV_CHAIN_H
#define _MARKOV_CHAIN_H
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"

typedef struct MarkovNode MarkovNode;
typedef struct NextNodeCounter NextNodeCounter;
typedef struct MarkovChain MarkovChain;

struct MarkovNode {
    char *data;
    NextNodeCounter *counter_list;  // array of Next words
    int size_of_next_array;
};

struct NextNodeCounter {
    MarkovNode *markov_node;
    int frequency;
};

struct MarkovChain {
    LinkedList *database;
};

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain The Chain will all the data
 * @return A random MarkovNode from the database
 */
MarkovNode *get_first_random_node (MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain (MarkovChain **ptr_chain);

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool
add_node_to_counter_list (MarkovNode *first_node, MarkovNode *second_node);

/**
* Check if data_ptr is in database. If so,
 * return the markov_node wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr);

/**
* If data_ptr in markov_chain, return it's markov_node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node *add_to_database (MarkovChain *markov_chain, char *data_ptr);

/////////////////////////////////////////////////////

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number maximal number to return (not including)
 * @return Random number
 */
int get_random_number (int max_number);
/**
 * The function checks which state is in index "ind" in the LinkedList.
 * @param ptr A pointer to the LinkedList
 * @param ind The specific index from list
 * @return A state from the index given.
 */
Node *linked_list_ind (LinkedList *ptr, int ind);
/**
 * The function checks if the MarkovNode is in the counter_list or not
 * @param dest The array to check membership
 * @param check The MarkovNode given
 * @return i ind of elem in list if Node is there and False otherwise
 */
int is_in_counter_list (MarkovNode *dest, MarkovNode *check);
/**
 * The function allocates The markov node the array of the next state counter
 * @param markov_node The node to allocate list to
 * @return True if succeeed and False otherwise
 */
int allocate_next_states_array (MarkovNode *markov_node);
/**
 * The function checks the sum of all NextNodeCounter array frequencies
 * @param markov_node The MarkovNode to sum his counter_list
 * @return The sum of all frequencies
 */
int frequencies_sum (MarkovNode *markov_node);
/**
 * The function choose the next state from the NextNodeCounter
 * @param markov_node The node to get data from his NextNodeCounter array
 * @param sum The sum of all frequencies
 * @return The chosen next state MarkovNode from array & NULL if failed
 */
MarkovNode *chose_next_state (MarkovNode *markov_node, int sum);
/**
 * The function checks if the given state ends with a POINT .
 * @param state The given state to check
 * @return True if does and False otherwise
 */
int ends_with_point (char *state);

#endif /* markovChain_h */