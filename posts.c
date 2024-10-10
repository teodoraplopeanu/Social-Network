#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "posts.h"

void create_post(retea *rt, char *user, char *title)
{
	post **aux = realloc(rt->post_arr->arr, (rt->post_arr->size + 1) *
						 sizeof(post *));
	DIE(!aux, MEM_ERR);
	rt->post_arr->arr = aux;

	rt->post_arr->arr[rt->post_arr->size] = malloc(sizeof(post));

	post *current = rt->post_arr->arr[rt->post_arr->size];
	DIE(!current, MEM_ERR);

	current->events = calloc(1, sizeof(tree));
	DIE(!current->events, MEM_ERR);

	current->id = rt->id;
	rt->id++;

	current->likes = 0;

	current->user_id = get_user_id(user);

	current->title = calloc(1, MAX_TITLE_SIZE);
	DIE(!current->title, MEM_ERR);
	memcpy(current->title, title, strlen(title));

	for (int i = 0; i < MAX_PEOPLE; i++)
		current->liked[i] = 0;

	current->likes = 0;

	current->events->parent_id = 0;

	rt->post_arr->size++;

	printf("Created \"%s\" for %s\n", title, user);
}

//functia returneaza un repost cu un anume id, pornind de la o postare (root)
post *get_parent(post *root, int id)
{
	if (root->id == id)
		return root;
	if (!root->events)
		return NULL;
	post *current = NULL;
	for (int i = 0; i < root->events->n_children; i++) {
		current = get_parent(root->events->children[i], id);
		if (current)
			break;
	}
	return current;
}

void create_repost(retea *rt, char *name, char *id_post, char *id_repost)
{
	int id_p = atoi(id_post);

	int id_r;
	if (id_repost)
		id_r = atoi(id_repost);
	else
		id_r = 0;

	//facem o noua postare/repostare
	post *new = calloc(1, sizeof(post));
	new->id = rt->id;
	rt->id++;
	new->user_id = get_user_id(name);
	for (int i = 0; i < MAX_PEOPLE; i++)
		new->liked[i] = 0;
	new->likes = 0;
	//events e alocat dar e totul pe NULL
	new->events = calloc(1, sizeof(tree));
	DIE(!new->events, MEM_ERR);

	int i = 0;
	for (; i < rt->post_arr->size; i++)
		if (rt->post_arr->arr[i]->id == id_p)
			break;
	post *post_p = rt->post_arr->arr[i];

	if (!id_r) {
		// la postarea cu idx i vom pune repostul
		post_p->events->n_children++;

		post **aux = realloc(post_p->events->children,
							 post_p->events->n_children * sizeof(post *));
		DIE(!aux, MEM_ERR);
		post_p->events->children = aux;
		post_p->events->children[post_p->events->n_children - 1] = new;
		new->events->parent_id = post_p->id;
	} else {
		//gasim parintele la noul repost
		post *repost_p = get_parent(post_p, id_r);
		//redimensionam vectorul de fii si adaugam la final
		repost_p->events->n_children++;
		new->events->parent_id = repost_p->id;
		post **aux = realloc(repost_p->events->children,
							repost_p->events->n_children * sizeof(post *));
		DIE(!aux, MEM_ERR);
		repost_p->events->children = aux;
		repost_p->events->children[repost_p->events->n_children - 1] = new;
	}
	printf("Created repost #%d for %s\n", rt->id - 1, name);
}

void print_reposts(post *repost) {
	printf("Repost #%d by %s\n", repost->id, get_user_name(repost->user_id));

	if (!repost->events->n_children)
		return;

	for (int i = 0; i < repost->events->n_children; i++)
		print_reposts(repost->events->children[i]);
}

void get_reposts(retea *rt, int id_p, int id_rp)
{
	int idx = 0;
	while (rt->post_arr->arr[idx] && rt->post_arr->arr[idx]->id != id_p)
		idx++;

	if (id_rp == -1) {
		printf("\"%s\" - Post by %s\n", rt->post_arr->arr[idx]->title,
			   get_user_name(rt->post_arr->arr[idx]->user_id));

		for (int i = 0; i < rt->post_arr->arr[idx]->events->n_children; i++)
			print_reposts(rt->post_arr->arr[idx]->events->children[i]);
	} else {
		post *found_repost = get_parent(rt->post_arr->arr[idx], id_rp);

		print_reposts(found_repost);
	}
}

void like(retea *rt, char *user, int id_p, int id_rp)
{
	int user_id = get_user_id(user);
	int idx = 0;
	while (rt->post_arr->arr[idx]->id != id_p)
		idx++;

	// Original post
	if (id_rp == -1) {
		post *p = rt->post_arr->arr[idx];
		// Like
		if (!p->liked[user_id]) {
			p->liked[user_id] = 1;
			p->likes++;
			printf("User %s liked post \"%s\"\n", user, p->title);
		} else {
			// Dislike
			p->liked[user_id] = 0;
			p->likes--;
			printf("User %s unliked post \"%s\"\n", user, p->title);
		}
	} else {
		// Repost
		post *found_repost = get_parent(rt->post_arr->arr[idx], id_rp);
		// Like
		post *root = rt->post_arr->arr[idx];
		if (!found_repost->liked[user_id]) {
			found_repost->liked[user_id] = 1;
			found_repost->likes++;
			printf("User %s liked repost \"%s\"\n", user, root->title);
		} else {
			// Dislike
			found_repost->liked[user_id] = 0;
			found_repost->likes--;
			printf("User %s unliked repost \"%s\"\n", user, root->title);
		}
	}
}

void get_likes(retea *rt, int id_p, int id_rp)
{
	int idx = 0;
	while (rt->post_arr->arr[idx]->id != id_p)
		idx++;
	post *root = rt->post_arr->arr[idx];
	if (id_rp == -1) {
		printf("Post \"%s\" has %d likes\n", root->title, root->likes);
	} else {
		post *repost = get_parent(root, id_rp);
		printf("Repost #%d has %d likes\n", repost->id, repost->likes);
	}
}

void get_max_likes(int *curr_max, int *id_max, post *curr_repost)
{
	if (curr_repost->likes > *curr_max) {
		*curr_max = curr_repost->likes;
		*id_max = curr_repost->id;
	}

	if (curr_repost->events->n_children == 0)
		return;

	for (int i = 0; i < curr_repost->events->n_children; i++)
		get_max_likes(curr_max, id_max, curr_repost->events->children[i]);
}

void ratio(retea *rt, int id_post) {
	int idx = 0;
	while (rt->post_arr->arr[idx]->id != id_post)
		idx++;

	post *original_post = rt->post_arr->arr[idx];
	int curr_max = original_post->likes;
	int id_max = id_post;

	for (int i = 0; i < original_post->events->n_children; i++)
		get_max_likes(&curr_max, &id_max, original_post->events->children[i]);

	if (curr_max > original_post->likes)
		printf("Post %d got ratio'd by repost %d\n", id_post, id_max);
	else
		printf("The original post is the highest rated\n");
}

void get_common_parent(retea *rt, int id_p, int idx1, int idx2)
{
	int idx = 0;
	while (rt->post_arr->arr[idx]->id != id_p)
		idx++;
	post *root = rt->post_arr->arr[idx];

	int v1[550] = {-1}, v2[550] = {-1}, k1 = 0, k2 = 0;

	post *p1 = get_parent(root, idx1);
	post *p2 = get_parent(root, idx2);

	while (p1) {
		v1[k1++] = p1->id;
		p1 = get_parent(root, p1->events->parent_id);
	}
	while (p2) {
		v2[k2++] = p2->id;
		p2 = get_parent(root, p2->events->parent_id);
	}

	for (int i = 0; i < k1; i++)
		if (v1[i] == idx2) {
			printf("The first common repost of %d and %d is %d\n",
				   idx1, idx2, idx2);
			return;
		}

	for (int i = 0; i < k2; i++)
		if (v2[i] == idx1) {
			printf("The first common repost of %d and %d is %d\n",
				   idx1, idx2, idx1);
			return;
		}

	for (int i = 0; i < k1; i++) {
		for (int j = 0; j < k2; j++)
			if (v1[i] == v2[j]) {
				printf("The first common repost of %d and %d is %d\n",
					   idx1, idx2, v2[j]);
				return;
			}
	}
}

void delete(retea *rt, int id_p, int id_r)
{
	int idx = 0;
	while (rt->post_arr->arr[idx]->id != id_p)
		idx++;
	if (id_r == -1) {
		printf("Deleted \"%s\"\n", rt->post_arr->arr[idx]->title);
		free_post(&rt->post_arr->arr[idx]);
		for (int i = idx; i < rt->post_arr->size - 1; i++)
			rt->post_arr->arr[i] = rt->post_arr->arr[i + 1];
		rt->post_arr->size--;
		if (rt->post_arr->size) {
			post **aux = realloc(rt->post_arr->arr, rt->post_arr->size *
								 sizeof(post *));
			DIE(!aux, MEM_ERR);
			rt->post_arr->arr = aux;
		}
	} else {
		post *current = get_parent(rt->post_arr->arr[idx], id_r);
		post *parent = get_parent(rt->post_arr->arr[idx],
								  current->events->parent_id);
		printf("Deleted repost #%d of post \"%s\"\n", current->id,
			   rt->post_arr->arr[idx]->title);
		int id_free = 0;
		while (parent->events->children[id_free]->id != id_r)
			id_free++;
		free_post(&current);
		for (int i = id_free; i < parent->events->n_children - 1; i++)
			parent->events->children[i] = parent->events->children[i + 1];
		parent->events->n_children--;
		if (parent->events->n_children) {
			post **aux = realloc(parent->events->children,
								 parent->events->n_children * sizeof(post *));
			DIE(!aux, MEM_ERR);
			parent->events->children = aux;
		}
	}
}

void handle_input_posts(retea *rt, char *input)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;
	if (!strcmp(cmd, "create")) {
		char *par1 = strtok(NULL, "\n ");
		char *par2 = strtok(NULL, "\"\n");
		create_post(rt, par1, par2);
	} else if (!strcmp(cmd, "repost")) {
		char *par1 = strtok(NULL, "\n ");
		char *par2 = strtok(NULL, "\n ");
		char *par3 = strtok(NULL, "\n ");
		create_repost(rt, par1, par2, par3);
	} else if (!strcmp(cmd, "common-repost")) {
		char *par1 = strtok(NULL, "\n ");
		char *par2 = strtok(NULL, "\n ");
		char *par3 = strtok(NULL, "\n ");

		int id_p = atoi(par1);
		int idx1 = atoi(par2);
		int idx2 = atoi(par3);

		get_common_parent(rt, id_p, idx1, idx2);
	} else if (!strcmp(cmd, "like")) {
		char *par1 = strtok(NULL, "\n ");
		char *par2 = strtok(NULL, "\n ");
		char *par3 = strtok(NULL, "\n ");

		int id1 = -1, id2 = -1;
		if (par2)
			id1 = atoi(par2);
		if (par3)
			id2 = atoi(par3);

		like(rt, par1, id1, id2);
	} else if (!strcmp(cmd, "ratio")) {
		char *par1 = strtok(NULL, "\n ");
		int id_p = atoi(par1);
		ratio(rt, id_p);
	} else if (!strcmp(cmd, "delete")) {
		char *par1 = strtok(NULL, "\n ");
		char *par2 = strtok(NULL, "\n ");

		int id1 = -1, id2 = -1;
		if (par1)
			id1 = atoi(par1);
		if (par2)
			id2 = atoi(par2);

		delete(rt, id1, id2);
	} else if (!strcmp(cmd, "get-likes")) {
		char *par1 = strtok(NULL, "\n ");
		char *par2 = strtok(NULL, "\n ");

		int id1 = -1, id2 = -1;
		if (par1)
			id1 = atoi(par1);
		if (par2)
			id2 = atoi(par2);

		get_likes(rt, id1, id2);
	} else if (!strcmp(cmd, "get-reposts")) {
		char *par1 = strtok(NULL, "\n ");
		int id_p = atoi(par1);
		char *par2 = strtok(NULL, "\n ");
		int id_rp = -1;
		if (par2)
			id_rp = atoi(par2);
		get_reposts(rt, id_p, id_rp);
	}

	free(commands);
}
