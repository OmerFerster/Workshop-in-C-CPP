CC = gcc
CCFLAGS = -Wall -Wextra -Wvla -std=c99
FIRST_OBJECTS = tweets_generator.c markov_chain.c linked_list.c
SECOND_OBJECTS = snakes_and_ladders.c markov_chain.c linked_list.c

tweets: $(FIRST_OBJECTS)
	$(CC) $(CCFLAGS) $^ -o tweets_generator

snake: $(SECOND_OBJECTS)
	$(CC) $(CCFLAGS) $^ -o snakes_and_ladders

all: tweets snake
clean:
	rm *.exe *.o