//Postelnicu Cristina-Marina 313 CA 2023-2024
#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dll_node_t {
	void *data;
	size_t address;
	unsigned int size;
	struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct doubly_linked_list_t {
	dll_node_t *head;
	unsigned int size, nr_bytes;
} doubly_linked_list_t;

typedef struct sfl_t {
	doubly_linked_list_t **heap;
	unsigned int nr_lists, start_address, nr_bytes_lists, reconstitution_type;
	int nr_malloc_calls;
	int nr_fragmentations;
	int nr_free_calls;
} sfl_t;

doubly_linked_list_t *dll_create(void);
void dll_add_nth_node(doubly_linked_list_t *list,
					  unsigned int n, unsigned int size, size_t address);
dll_node_t *dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n);
void dll_free(doubly_linked_list_t **pp_list);
void add_in_order(doubly_linked_list_t *list,
				  unsigned int size, size_t address);
void add_new_list(sfl_t *sfl,
				  unsigned int size, size_t address);

#endif // HELPER_H