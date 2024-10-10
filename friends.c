#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "queue.h"

void add_friend(retea *rt, char *friend1, char *friend2)
{
	int idx1 = get_user_id(friend1);
	int idx2 = get_user_id(friend2);
	rt->db_matrix[idx1][idx2] = 1;
	rt->db_matrix[idx2][idx1] = 1;
	printf("Added connection %s - %s\n", friend1, friend2);
}

void remove_friend(retea *rt, char *friend1, char *friend2)
{
	int idx1 = get_user_id(friend1);
	int idx2 = get_user_id(friend2);
	rt->db_matrix[idx1][idx2] = 0;
	rt->db_matrix[idx2][idx1] = 0;
	printf("Removed connection %s - %s\n", friend1, friend2);
}

void get_distance(retea *rt, char *friend1, char *friend2)
{
	// initializare vectori si coada
	int *viz = calloc(MAX_PEOPLE, sizeof(int));
	DIE(!viz, MEM_ERR);
	int *d = malloc(MAX_PEOPLE * sizeof(int));
	DIE(!d, MEM_ERR);

	for (int i = 0; i < MAX_PEOPLE; i++)
		d[i] = MAX_PEOPLE + 1;

	queue_t *q  = q_create(sizeof(int), MAX_PEOPLE);

	int src = get_user_id(friend1);
	int dest = get_user_id(friend2);
	int v;

	// punem primul element in coada
	viz[src] = 1;
	d[src] = 0;
	q_enqueue(q, &src);
	while (!q_is_empty(q)) {
		// luam primul element din coada
		v = *((int *)q_front(q));
		q_dequeue(q);
		// luam fiecare vecin nevizitat
		for (int i = 0; i < MAX_PEOPLE; i++) {
			if (rt->db_matrix[v][i] && v != i) {
				if (!viz[i]) {
					viz[i] = 1;
					d[i] = d[v] + 1;
					q_enqueue(q, &i);
				}
			}
		}
	}
	// afisam rezultatul
	if (d[dest] == MAX_PEOPLE + 1)
		printf("There is no way to get from %s to %s\n", friend1, friend2);
	else
		printf("The distance between %s - %s is %d\n", friend1,
			   friend2, d[dest]);

	free(viz);
	free(d);
	q_free(q);
	free(q);
}

void get_suggestions(retea *rt, char *user)
{
	int id = get_user_id(user);
	int ok = 0;
	int *suggestion = calloc(1, MAX_PEOPLE * sizeof(int));
	DIE(!suggestion, MEM_ERR);

	for (int i = 0; i < MAX_PEOPLE; i++) {
		if (rt->db_matrix[id][i]) {
			for (int j = 0; j < MAX_PEOPLE; j++) {
				if (rt->db_matrix[i][j] && !rt->db_matrix[id][j] && j != id) {
					suggestion[j] = 1;
					ok = 1;
				}
			}
		}
	}

	if (!ok) {
		printf("There are no suggestions for %s\n", user);
	} else {
		char *name;
		printf("Suggestions for %s:\n", user);
		for (int i = 0; i < MAX_PEOPLE; i++) {
			if (suggestion[i]) {
				name = get_user_name(i);
				printf("%s\n", name);
			}
		}
	}

	free(suggestion);
}

void get_common_friends(retea *rt, char *friend1, char *friend2)
{
	int idx1 = get_user_id(friend1);
	int idx2 = get_user_id(friend2);
	int ok = 0;
	int *common = calloc(MAX_PEOPLE, sizeof(int));
	DIE(!common, MEM_ERR);

	for (int i = 0; i < MAX_PEOPLE; i++) {
		if (rt->db_matrix[idx1][i] == 1 && rt->db_matrix[idx2][i] == 1 &&
			idx1 != i && idx2 != i) {
			ok = 1;
			common[i] = 1;
		}
	}

	if (!ok) {
		printf("No common friends for %s and %s\n", friend1, friend2);
	} else {
		char *name;
		printf("The common friends between %s and %s are:\n", friend1, friend2);
		for (int i = 0; i < MAX_PEOPLE; i++) {
			if (common[i]) {
				name = get_user_name(i);
				printf("%s\n", name);
			}
		}
	}
	free(common);
}

// afisam in handle_input pt ca folosim la popular
int get_nr_of_friends(retea *rt, char *user)
{
	int idx = get_user_id(user);
	int nr = 0;
	for (int i = 0; i < MAX_PEOPLE; i++)
		nr += rt->db_matrix[idx][i];
	return nr;
}

void show_nr_of_friends(retea *rt, char *user)
{
	int nr = get_nr_of_friends(rt, user);
	printf("%s has %d friends\n", user, nr);
}

void get_popular(retea *rt, char *user)
{
	int idx = get_user_id(user);
	int nr, max = -1, poz;
	char *name;

	for (int i = 0; i < MAX_PEOPLE; i++) {
		if (rt->db_matrix[idx][i]) {
			name = get_user_name(i);
			nr = get_nr_of_friends(rt, name);
			if (nr > max) {
				poz = i;
				max  = nr;
			}
		}
	}

	nr = get_nr_of_friends(rt, user);

	if (max <= nr) {
		printf("%s is the most popular\n", user);
	} else {
		name = get_user_name(poz);
		printf("%s is the most popular friend of %s\n", name, user);
	}
}

void handle_input_friends(retea *rt, char *input)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	char *par1 = strtok(NULL, "\n ");
	char *par2 = strtok(NULL, "\n ");
	if (!cmd)
		return;

	if (!strcmp(cmd, "add"))
		add_friend(rt, par1, par2);
	else if (!strcmp(cmd, "remove"))
		remove_friend(rt, par1, par2);
	else if (!strcmp(cmd, "suggestions"))
		get_suggestions(rt, par1);
	else if (!strcmp(cmd, "distance"))
		get_distance(rt, par1, par2);
	else if (!strcmp(cmd, "common"))
		get_common_friends(rt, par1, par2);
	else if (!strcmp(cmd, "friends"))
		show_nr_of_friends(rt, par1);
	else if (!strcmp(cmd, "popular"))
		get_popular(rt, par1);
	free(commands);
}
