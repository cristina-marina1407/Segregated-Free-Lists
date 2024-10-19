//Postelnicu Cristina-Marina 313 CA 2023-2024
#include "helper.h"

//functia creeaza o lista dublu inlantuita
doubly_linked_list_t *dll_create(void)
{
	doubly_linked_list_t *list = malloc(1 * sizeof(doubly_linked_list_t));
	list->nr_bytes = 0;
	list->head = NULL;
	list->size = 0;
	return list;
}

/*functia dll_add_nth_node adauga un nod in lista pe pozitia n, iar daca n este
mai mare decat list->size adauga elementul pe ultima pozitie*/
void dll_add_nth_node(doubly_linked_list_t *list,
					  unsigned int n, unsigned int size, size_t address)
{
	unsigned int i;
	dll_node_t *new_node = malloc(sizeof(dll_node_t));
	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->data = NULL;
	new_node->address = address;
	new_node->size = size;
	if (!list->head) {
		list->head = new_node;
		new_node->next = NULL;
		new_node->prev = NULL;
		list->size = list->size + 1;
	} else if (n == 0) {
		new_node->next = list->head;
		new_node->prev = NULL;
		list->head->prev = new_node;
		list->head = new_node;
		list->size = list->size + 1;
	} else if (n >= list->size) {
		dll_node_t *current_node = list->head;
		while (current_node->next)
			current_node = current_node->next;
		current_node->next = new_node;
		new_node->prev = current_node;
		new_node->next = NULL;
		list->size = list->size + 1;
	} else {
		dll_node_t *current_node = list->head;
		i = 0;
		while (i < n - 1) {
			current_node = current_node->next;
			i++;
		}
		new_node->next = current_node->next;
		current_node->next->prev = new_node;
		current_node->next = new_node;
		new_node->prev = current_node;
		list->size = list->size + 1;
	}
}

/*functia dll_remove_nth_node sterge un element de pe pozitia n
din lista si returneaza nodul pe care l a sters*/
dll_node_t *dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n)
{
	if (!list->head) {
		return malloc(sizeof(dll_node_t));
	} else if (n == 0) {
		dll_node_t *to_remove = list->head;
		list->head = to_remove->next;
		list->size = list->size - 1;
		return to_remove;
	} else if (n >= list->size) {
		dll_node_t *current_node = list->head;
		while (current_node->next)
			current_node = current_node->next;
		dll_node_t *to_remove = current_node;
		current_node->prev->next = NULL;
		list->size = list->size - 1;
		return to_remove;
	}
	unsigned int i = 0;
	dll_node_t *current_node = list->head;
	for (i = 0; i < n - 1; i++)
		current_node = current_node->next;
	dll_node_t *to_remove = current_node->next;
	to_remove->prev->next = to_remove->next;
	if (to_remove->next)
		to_remove->next->prev = current_node;
	list->size = list->size - 1;
	return to_remove;
}

/*functia dll_free elibereaza memoria folosita pentru
o lista dublu inlantuita*/
void dll_free(doubly_linked_list_t **pp_list)
{
	dll_node_t *aux1 = (*pp_list)->head;
	while (aux1) {
		dll_node_t *aux2 = aux1;
		aux1 = aux1->next;
		if (aux2->data)
			free(aux2->data);
		free(aux2);
	}
	free(*pp_list);
}

/*functia add_in_order este folosita pentru a adauga noduri
intr o lista in ordinea adreselor, pentru a mentine ordinea
crescatoare dupa apelarea functiilor malloc_function si
free_function*/
void add_in_order(doubly_linked_list_t *list,
				  unsigned int size, size_t address)
{
	unsigned int i = 0;
	dll_node_t *current_node = list->head;
	while (i < list->size) {
		if (address < current_node->address) {
			break;
			} else {
				i++;
				current_node = current_node->next;
			}
	}
	dll_add_nth_node(list, i, size, address);
}

/*functia add_new_list adauga o noua lista in vectorul de liste
daca nu exista o lista care sa stocheze nodurile cu un numar
de bytes rezultata dupa fragmentare*/
void add_new_list(sfl_t *sfl,
				  unsigned int size, size_t address)
{
	unsigned int i = 0, j = 0;
	while (sfl->heap[i]->nr_bytes < size)
		i++;
	sfl->nr_lists++;
	//realocam vectorul de liste
	sfl->heap = realloc(sfl->heap,
						sfl->nr_lists * sizeof(doubly_linked_list_t *));

	j = 0;
	/*mutam listele cu cate p pozitie pentru a adauga lista cea noua
	in ordine creescatoare*/
	for (j = sfl->nr_lists - 1; j > i; j--)
		sfl->heap[j] = sfl->heap[j - 1];
	sfl->heap[i] = dll_create();
	sfl->heap[j]->nr_bytes = size;
	//adaugam nodul in lista nou creata
	dll_add_nth_node(sfl->heap[j], 0, size, address);
}