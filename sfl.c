//Postelnicu Cristina-Marina 313 CA 2023-2024
#include "helper.h"

void init_heap(sfl_t *sfl)
{
	unsigned int i, j;
	//citim adresa de la care incepe heap-ul
	scanf("%x", &sfl->start_address);
	//citim numarul de liste
	scanf("%d", &sfl->nr_lists);
	//numarul de bytes pe care il are fiecare lista
	scanf("%d", &sfl->nr_bytes_lists);
	scanf("%d", &sfl->reconstitution_type);
	/*initializam contoarele pentru numarul de apeluri
	ale functiilor malloc_function, free_function si
	pentru numarul de fragmentari. Aceste contoare vor fi
	afisate in functia dump_memory*/
	sfl->nr_malloc_calls = 0;
	sfl->nr_fragmentations = 0;
	sfl->nr_free_calls = 0;
	/*alocam memorie pentru vectorul de liste care retine
	memoria libera*/
	sfl->heap = malloc(sfl->nr_lists * sizeof(doubly_linked_list_t *));
	//programare defensiva
	if (!sfl) {
		printf("Allocation failed\n");
		free(sfl);
	}
	unsigned int node_size = 8, node_address = 0;
	unsigned int nr_nodes = sfl->nr_bytes_lists / 8;
	/*alocam memorie in vector pentru liste in functie de numarul
	de bytes pe care il au nodurile din fiecare si crestem
	aceasta valoare pe masura ce inaintam in numarul listelor*/
	for (i = 0; i < sfl->nr_lists; i++) {
		sfl->heap[i] = malloc(1 * sizeof(doubly_linked_list_t));
		if (!sfl->heap[i]) {
			printf("Allocation failed\n");
			for (j = 0; j < i; j++)
				free(sfl->heap[i]);
			free(sfl->heap);
		}
		sfl->heap[i]->head = NULL;
		sfl->heap[i]->nr_bytes = node_size;
		sfl->heap[i]->size = 0;
		node_size = node_size * 2;
	}
	/*initializam din nou node_size pentru a putea adauga corespunzator
	nr de bytes al fiecarui nod, folosind functia de adaugare pe pozitia n*/
	node_size = 8;
	node_address = sfl->start_address;
	for (i = 0; i < sfl->nr_lists; i++) {
		for (j = 0 ; j < nr_nodes; j++) {
			dll_add_nth_node(sfl->heap[i], nr_nodes, node_size, node_address);
			/*punem adresele in ordine crescatoare pentru a avea
			zona continua de memorie ceruta in enunt*/
			node_address += node_size;
		}
		node_size *= 2;
		nr_nodes /= 2;
	}
}

/*functia malloc_function adauga memoria alocata in lista
dublu inlantuita allocated*/
void malloc_function(sfl_t *sfl, doubly_linked_list_t *allocated)
{
	unsigned int nr_bytes, i = 0, j = 0, max = 0;
	size_t node_left_address;
	int list_exists = 0, pos = 0;
	//citim numarul de bytes pe care dorim sa il alocam
	scanf("%d", &nr_bytes);
	/*parcurgem vectorul de liste pentru a afla daca numarul de bytes
	pe care dorim sa il alocam este mai mare decat numarul de bytes al
	nodurilor disponibile in fiecare lista*/
	for (i = 0; i < sfl->nr_lists; i++) {
		if (sfl->heap[i]->nr_bytes > max && sfl->heap[i]->head)
			max = sfl->heap[i]->nr_bytes;
	}
	if (nr_bytes > max) {
		printf("Out of memory\n");
		return;
	}
	sfl->nr_malloc_calls++;
	for (i = 0; i < sfl->nr_lists; i++) {
		/*daca nr bytes citit coincide cu nr de bytes al unui nod
		adaugam acest nod in allocated in ordinea crescatoare a adreselor*/
		if (nr_bytes == sfl->heap[i]->nr_bytes && sfl->heap[i]->head) {
			/*stergem nodul din lista corespunzatoare din vector si
			il returnam in sfl_node pentru a ii pastra datele*/
			dll_node_t *sfl_node = dll_remove_nth_node(sfl->heap[i], 0);
			if (!allocated->head) {
				//daca nu a fost alocat niciun nod, adaugam pe prima pozitie
				dll_add_nth_node(allocated, 0, sfl_node->size,
								 sfl_node->address);
				free(sfl_node);
				return;
			}
			add_in_order(allocated, sfl_node->size, sfl_node->address);
			free(sfl_node);
			return;
		} else if (nr_bytes < sfl->heap[i]->nr_bytes && sfl->heap[i]->head) {
			sfl->nr_fragmentations++;
			dll_node_t *sfl_node = dll_remove_nth_node(sfl->heap[i], 0);
			unsigned int bytes_left;
			//calculam numarul de bytes care ramane liber dupa fragmentare
			bytes_left = sfl->heap[i]->nr_bytes - nr_bytes;
			if (!allocated->head)
				/*adaugam nodul cu numarul de bytes citit si adresa nodului
				pe care il fragmentam*/
				dll_add_nth_node(allocated, 0, nr_bytes, sfl_node->address);
			else
				add_in_order(allocated, nr_bytes, sfl_node->address);
			/*calculam adresa blocului ramas liber ce trebuie adaugat inapoi
			in vectorul de liste in locul corespunzator*/
			node_left_address = sfl_node->address + nr_bytes;
			/*parcurgem vectorul de liste pentru a afla daca exista o lista
			ce retine noduri cu numarul de bytes ramasi*/
			while (j < sfl->nr_lists) {
				if (bytes_left == sfl->heap[j]->nr_bytes) {
					list_exists = 1;
					//pastram pozitia la care trebuie adaugat apoi nodul
					pos = j;
					break;
				}
				j++;
			}
			if (list_exists == 1) {
				add_in_order(sfl->heap[pos], bytes_left, node_left_address);
				free(sfl_node);
				return;
			}
			add_new_list(sfl, bytes_left, node_left_address);
			free(sfl_node);
			return;
		}
	}
}

//functia free_function elibereaza blocul aflat la o anumita adresa
void free_function(sfl_t *sfl, doubly_linked_list_t *allocated)
{
	size_t address;
	unsigned int i = 0, j = 0;
	int verif = 0;
	//citim adresa pe care dorim sa o eliberam
	scanf("%lx", &address);
	/*parcuregem lista allocated pentru a afla daca a fost
	alocata aceasta adresa*/
	dll_node_t *current_node = allocated->head;
	for (i = 0; i < allocated->size; i++) {
		if (current_node->address == address) {
			verif = 1;
			break;
		}
		current_node = current_node->next;
	}
	if (verif == 0) {
		printf("Invalid free\n");
		return;
	}
	sfl->nr_free_calls++;
	dll_node_t *allocated_node = dll_remove_nth_node(allocated, i);
	/*adaugam nodul in vectorul de liste in lista corespunzatoare
	sau cream aceasta lista in caz ca nu exista*/
	while (j < sfl->nr_lists) {
		if (sfl->heap[j]->nr_bytes == allocated_node->size) {
			add_in_order(sfl->heap[j], allocated_node->size,
						 allocated_node->address);
			free(allocated_node->data);
			free(allocated_node);
			return;
		}
		j++;
	}
	add_new_list(sfl, allocated_node->size, allocated_node->address);
	free(allocated_node->data);
	free(allocated_node);
}

//functia write scrie in nodurile alocate
int  write(doubly_linked_list_t *allocated)
{
	size_t address;
	char line[300], string[200], number[10], data[200], aux[200], new_data[200];
	char *token1;
	unsigned int nr_bytes, i, a, verif = 0;
	scanf("%lx", &address);
	fgets(line, sizeof(line), stdin);
	token1 = strtok(line, "\"");
	if (token1) {
		strncpy(string, token1, sizeof(string));
		string[strlen(string)] = '\0';
	}
	token1 = strtok(NULL, "\"");
	//stocam in data stringul pe care dorim sa l scriem
	if (token1) {
		strncpy(data, token1, sizeof(data));
		string[strlen(data)] = '\0';
	}
	token1 = strtok(NULL, " ");
	if (token1) {
		strncpy(number, token1, sizeof(number));
		number[strlen(number)] = '\0';
	}
	//dupa ce obtinem numarul de bytes, il transformam in numar de tip int
	nr_bytes = atoi(number);
	//aflam daca adresa la care dorim sa scriem a fost alocata
	dll_node_t *current_node = allocated->head;
	for (i = 0; i < allocated->size; i++)  {
		if (address == current_node->address) {
			verif = 1;
			break;
		}
		current_node = current_node->next;
	}
	if (!verif)
		return -1;
	/*daca nr de bytes dat este mai mare decat stringul, scriem numarul de bytes
	al stringului*/
	a = strlen(data);
	if (nr_bytes > a)
		nr_bytes = a;
	if (nr_bytes <= current_node->size) {
		/*daca exista deja date scrise in nod, suprascriem cu noile date
		nr de bytes al acestora*/
		if (current_node->data) {
			strcpy(aux, current_node->data + strlen(data));
			strcat(data, aux);
			free(current_node->data);
			current_node->data = malloc(strlen(data) * sizeof(char) + 1);
			memcpy(current_node->data, data, strlen(data) * sizeof(char) + 1);
			((char *)current_node->data)[strlen(data)] = '\0';
			return 0;
		}
		current_node->data = malloc(nr_bytes * sizeof(char) + 1);
		memcpy(current_node->data, data, nr_bytes * sizeof(char) + 1);
		((char *)current_node->data)[nr_bytes] = '\0';
	} else {
		/*daca nr de bytes pe care trebuie sa l scriem este mai mic decat
		nodurile alocate, scriem datele pe parcursul a mai multor noduri*/
		while (nr_bytes) {
			size_t next = 0;
			if (!current_node)
				break;
			if (current_node->next)
				next = current_node->next->address;
			if (current_node->address + current_node->size != next && next)
				return -1;
			if (current_node->data)
				free(current_node->data);
			current_node->data = malloc(current_node->size * sizeof(char) + 1);
			memcpy(current_node->data, data,
				   current_node->size * sizeof(char) + 1);
			strcpy(new_data, data + current_node->size);
			strcpy(data, new_data);
			((char *)current_node->data)[current_node->size] = '\0';
			nr_bytes = nr_bytes - current_node->size;
			current_node = current_node->next;
		}
	}
	return 0;
}

//functia read printeaza ce este scris in nodurile alocate
int read(doubly_linked_list_t *allocated)
{
	size_t address;
	unsigned int nr_bytes, i;
	int verif = 0;
	char aux[100];
	scanf("%lx", &address);
	scanf("%d", &nr_bytes);
	//aflam daca adresa de la care dorim sa printam a fost alocata
	dll_node_t *current_node = allocated->head;
	for (i = 0; i < allocated->size; i++)  {
		if (address == current_node->address) {
			verif = 1;
			break;
		}
		current_node = current_node->next;
	}

	if (!verif)
		return -1;
	/*in functie de numarul de bytes printam un nod sau mai
	multe noduri consecutive*/
	if (nr_bytes <= current_node->size) {
		if (current_node->data) {
			strncpy(aux, current_node->data, nr_bytes);
			aux[nr_bytes] = '\0';
			printf("%s\n", aux);
		}
	} else {
		while (nr_bytes) {
			size_t next = 0;
			if (!current_node)
				break;
			if (current_node->next)
				next = current_node->next->address;
			if (current_node->address + current_node->size != next && next)
				return -1;
			printf("%s", (char *)current_node->data);
			nr_bytes = nr_bytes - current_node->size;
			current_node = current_node->next;
		}
		printf("\n");
	}
	return 0;
}

//functia dump_memory printeaza stadiul in care se afla memoria
void dump_memory(sfl_t *sfl, doubly_linked_list_t *allocated)
{
	unsigned int i, j, nr_free_blocks = 0;
	unsigned int total = 0, total_allocated = 0, total_free = 0;
	/*calculam memoria libera care exista in vectorul de liste,
	cat si numarulde noduri din acesta */
	for (i = 0; i < sfl->nr_lists; i++) {
		if (sfl->heap[i]->head)
			nr_free_blocks += sfl->heap[i]->size;
		total_free += sfl->heap[i]->size * sfl->heap[i]->nr_bytes;
	}
	//calculam numarul de bytes alocat in lista allocated
	dll_node_t *current_node = allocated->head;
	for (i = 0; i < allocated->size; i++) {
		total_allocated += current_node->size;
		current_node = current_node->next;
	}
	//memoria totala se obtine din suma acestora
	total = total_allocated + total_free;
	/*printam valorile calculate mai sus, cat si contoarele actualizate
	in malloc_function si free_function*/
	printf("+++++DUMP+++++\n");
	printf("Total memory: %d bytes\n", total);
	printf("Total allocated memory: %d bytes\n", total_allocated);
	printf("Total free memory: %d bytes\n", total_free);
	printf("Free blocks: %d\n", nr_free_blocks);
	printf("Number of allocated blocks: %d\n", allocated->size);
	printf("Number of malloc calls: %d\n", sfl->nr_malloc_calls);
	printf("Number of fragmentations: %d\n", sfl->nr_fragmentations);
	printf("Number of free calls: %d\n", sfl->nr_free_calls);
	for (i = 0; i < sfl->nr_lists; i++) {
		if (sfl->heap[i]->head) {
			//printam toate blocurile libere, dimensiunea si adresa lor
			printf("Blocks with %d bytes - %d free block(s) :",
				   sfl->heap[i]->nr_bytes, sfl->heap[i]->size);
			dll_node_t *current_node = sfl->heap[i]->head;
			for (j = 0; j < sfl->heap[i]->size; j++) {
				printf(" 0x%lx", current_node->address);
				current_node = current_node->next;
			}
			printf("\n");
		}
	}
	dll_node_t *current_node_allocated = allocated->head;
	printf("Allocated blocks :");
	for (i = 0; i < allocated->size; i++) {
		//printam dimensiunea si adresa blocurilor alocate
		printf(" (0x%lx - %d)", current_node_allocated->address,
			   current_node_allocated->size);
		current_node_allocated = current_node_allocated->next;
	}
	printf("\n-----DUMP-----\n");
}

//functia destroy_heap elibereaza vectorul de liste si lista de alocate
void destroy_heap(sfl_t *sfl, doubly_linked_list_t *allocated)
{
	unsigned int i;
	for (i = 0; i < sfl->nr_lists; i++)
		if (sfl->heap[i])
			dll_free(&sfl->heap[i]);
	free(sfl->heap);
	dll_free(&allocated);
}

int main(void)
{
	int exit = 0;
	char command[100];
	//citim de la tastatura comanda
	scanf("%s", command);
	/*declaram vectorul de liste si lista dublu inlantuita care retine
	zonele ocupate de memorie*/
	sfl_t sfl;
	doubly_linked_list_t *allocated;
	//cream lista de memorie alocata
	allocated = dll_create();
	while (strcmp(command, "DESTROY_HEAP") != 0) {
		if (strcmp(command, "INIT_HEAP") == 0) {
			init_heap(&sfl);
		} else if (strcmp(command, "MALLOC") == 0) {
			malloc_function(&sfl, allocated);
		} else if (strcmp(command, "FREE") == 0) {
			free_function(&sfl, allocated);
		} else if (strcmp(command, "READ") == 0) {
			exit = read(allocated);
			if (exit == -1) {
				printf("Segmentation fault (core dumped)\n");
				dump_memory(&sfl, allocated);
				break;
			}
		} else if (strcmp(command, "WRITE") == 0) {
			exit = write(allocated);
			if (exit == -1) {
				printf("Segmentation fault (core dumped)\n");
				dump_memory(&sfl, allocated);
				break;
			}
		} else if (strcmp(command, "DUMP_MEMORY") == 0) {
			dump_memory(&sfl, allocated);
		}
		//citim o noua comanda
		scanf("%s", command);
	}
	destroy_heap(&sfl, allocated);
	return 0;
}
