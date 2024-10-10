#include "users.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *db_path = "users.db";

static char **users;
static uint16_t users_number;

void init_users(void)
{
	FILE *users_db = fopen(db_path, "r");

	if (!users_db) {
		perror("Error reading users.db");
		return;
	}

	fscanf(users_db, "%hu", &users_number);

	users = malloc(users_number * sizeof(char *));

	char temp[32];
	for (uint16_t i = 0; i < users_number; i++) {
		fscanf(users_db, "%s", temp);
		int size = strlen(temp);

		users[i] = malloc(size + 1);
		strcpy(users[i], temp);
	}

	fclose(users_db);
}

uint16_t get_user_id(char *name)
{
	if (!users)
		return -1;

	for (uint16_t i = 0; i < users_number; i++)
		if (!strcmp(users[i], name))
			return i;

	return -1;
}

char *get_user_name(uint16_t id)
{
	if (id >= users_number)
		return NULL;

	return users[id];
}

void free_users(void)
{
	for (size_t i = 0; i < users_number; i++)
		free(users[i]);

	free(users);
}

post_arr_t *init_post_array(void)
{
	post_arr_t *post_arr = calloc(MAX_PEOPLE, sizeof(struct post_arr_t));
	post_arr->arr = malloc(MAX_PEOPLE * sizeof(post *));
	post_arr->size = 0;
	return post_arr;
}

retea *init_retea(void)
{
	retea *rt = malloc(sizeof(retea));
	rt->db_matrix = malloc(sizeof(int *) * MAX_PEOPLE);
	DIE(!rt->db_matrix, MEM_ERR);

	for (int i = 0; i < MAX_PEOPLE; i++) {
		rt->db_matrix[i] = calloc(MAX_PEOPLE, sizeof(int));
		if (!rt->db_matrix[i]) {
			printf(MEM_ERR);
			for (int j = 0; j < i; j++)
				free(rt->db_matrix[j]);
			exit(-1);
		}
	}
	rt->id = 1;
	rt->size = MAX_PEOPLE;

	rt->post_arr = init_post_array();
	return rt;
}

void free_post(post **post_p)
{
	post *aux = *post_p;

	for (int i = 0; i < (*post_p)->events->n_children; i++)
		free_post(&(*post_p)->events->children[i]);

	//free(aux->liked);
	free(aux->events->children);
	free(aux->events);
	free(aux->title);
	free(aux);
}

void free_retea(retea **rt)
{
	retea *aux = *rt;
	for (int i = 0; i < MAX_PEOPLE; i++)
		free(aux->db_matrix[i]);
	free(aux->db_matrix);

	for (int i = 0; i < (*rt)->post_arr->size; i++)
		free_post(&(*rt)->post_arr->arr[i]);

	if ((*rt)->post_arr->arr)
		free((*rt)->post_arr->arr);

	free((*rt)->post_arr);
	free(*rt);
}
