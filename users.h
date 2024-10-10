#ifndef USERS_H
#define USERS_H
#include <stdint.h>
#include <errno.h>

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

#define MAX_TITLE_SIZE 280
#define MEM_ERR "Eroare la alocarea memoriei.\n"
#define MAX_PEOPLE 550

typedef struct tree_t tree;

typedef struct post_t
{
	int id, user_id, likes;
	char *title;
	struct tree_t *events;
	int liked[MAX_PEOPLE];
} post;

struct tree_t
{
	int parent_id, n_children;
	post **children;
};

typedef struct post_arr_t
{
	post **arr;		// vector de pinteri la postari
	int size;		// dimensiunea
} post_arr_t;

typedef struct retea
{
	int **db_matrix;		// matricea de adiacenta a grafului 
	int size;				// dimensiunea
	int id;					// id-ul curent, ce va fi atribuit urmatoarei postari
							// create, urmand a fi incrementat
	post_arr_t *post_arr;	// array ce contine toate postarile facute in retea
} retea;

/**
 * Initializes the user list
*/
void init_users(void);

/**
 * Find the user_id of a user by it's name
 *
 * @param name - The name of the user
 * @return the id of the user, of -1 if name is not found
*/
uint16_t get_user_id(char *name);

/**
 * Find the user_id of a user by it's name
 *
 * @param id - The id of a user
 * @return the name of a user, of NULL if not found
*/
char *get_user_name(uint16_t id);

/**
 * Frees the user list
*/
void free_users(void);

#endif
