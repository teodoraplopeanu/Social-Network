#ifndef POSTS_H
#define POSTS_H

#include "users.h"
#include "friends.h"

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_posts(retea *rt, char *input);

void free_post(post **post_p);

post_arr_t *init_post_array(void);

void create_post(retea *rt, char *user, char *title);

post *get_parent(post *root, int id);

void create_repost(retea *rt, char *name, char *id_post, char *id_repost);

void get_reposts(retea *rt, int id_p, int id_rp);

void like(retea *rt, char *user, int id_p, int id_rp);

void get_likes(retea *rt, int id_p, int id_rp);

void ratio(retea *rt, int id_post);

void get_common_parent(retea *rt, int id_p, int idx1, int idx2);

void delete(retea *rt, int id_p, int id_rp);

#endif // POSTS_H
