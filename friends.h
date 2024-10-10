#include <errno.h>
#include "users.h"

#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 100

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/

void free_retea(retea **rt);

retea *init_retea(void);

void add_friend(retea *rt, char *friend1, char *friend2);

void remove_friend(retea *rt, char *friend1, char *friend2);

void get_distance(retea *rt, char *friend1, char *friend2);

void get_suggestions(retea *rt, char *user);

void get_common_friends(retea *rt, char *friend1, char *friend2);

int get_nr_of_friends(retea *rt, char *user);

void show_nr_of_friends(retea *rt, char *user);

void get_popular(retea *rt, char *user);

void handle_input_friends(retea *rt, char *input);

#endif // FRIENDS_H
