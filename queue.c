#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	/* TODO */
	queue_t *q = malloc(sizeof(queue_t));
	q->buff = calloc(1, max_size * sizeof(void *));
	q->max_size = max_size;
	q->data_size = data_size;
	q->read_idx = 0;
	q->write_idx = 0;
	q->size = 0;
	return q;
}

/*
 * Functia intoarce numarul de elemente din coada al carei pointer este trimis
 * ca parametru.
 */
unsigned int
q_get_size(queue_t *q)
{
	return q->size;
}

/*
 * Functia intoarce 1 daca coada este goala si 0 in caz contrar.
 */
unsigned int
q_is_empty(queue_t *q)
{
	if (q->size)
		return 0;
	else
		return 1;
}

/*
 * Functia intoarce primul element din coada, fara sa il elimine.
 */
void *
q_front(queue_t *q)
{
	return q->buff[q->read_idx];
}

/*
 * Functia scoate un element din coada. Se va intoarce 1 daca operatia s-a
 * efectuat cu succes (exista cel putin un element pentru a fi eliminat) si
 * 0 in caz contrar.
 */
int
q_dequeue(queue_t *q)
{
	if (!q->size)
		return 0;
	free(q->buff[q->read_idx]);
	q->buff[q->read_idx] = 0;
	q->size--;
	q->read_idx = (q->read_idx + 1) % q->max_size;

	return 1;
}

/*
 * Functia introduce un nou element in coada. Se va intoarce 1 daca
 * operatia s-a efectuat cu succes (nu s-a atins dimensiunea maxima)
 * si 0 in caz contrar.
 */
int
q_enqueue(queue_t *q, void *new_data)
{
	if (q->size == q->max_size)
		return 0;
	q->buff[q->write_idx] = calloc(1, sizeof(int));
	memcpy(q->buff[q->write_idx], new_data, sizeof(int));
	q->write_idx = (q->write_idx + 1) % q->max_size;
	q->size++;
	return 1;
}

/*
 * Functia elimina toate elementele din coada primita ca parametru.
 */
void
q_clear(queue_t *q)
{
	for (unsigned int  i = 0; i < q->max_size; i++) {
		if (q->buff[i])
			free(q->buff[i]);
		q->buff[i] = NULL;
	}
}

/*
 * Functia elibereaza toata memoria ocupata de coada.
 */
void
q_free(queue_t *q)
{
	q_clear(q);
	free(q->buff);
}
