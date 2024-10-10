#ifndef FEED_H
#define FEED_H
#include "users.h"

/**
 * Function that handles the calling of every command from task 3
*/

void handle_input_feed(retea *rt, char *input);

void friends_repost(retea *rt, char *username, int id_post);

void view_profile(retea *rt, char *username);

void get_feed(retea *rt, char *username, int feed_size);

#endif // FEED_H
