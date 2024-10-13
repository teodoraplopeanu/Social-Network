# Social Network

This project implements a social network similar to Facebook. The solution uses data structures such as graphs, trees, and queues.

### Network Data Structure

~~~
typedef struct network {
    int **db_matrix;        // adjacency matrix of the graph
    int size;               // size of the network
    int id;                 // ID for new posts
    post_arr_t *post_arr;   // array of posts
} network;
~~~

## Friendship Network

Functions for managing user connections:
### Add Friend
`add <nume-1> <nume-2>`
Establishes a bidirectional connection.
		
### Remove Friend
`remove <nume-1> <nume-2>`
Removes the connection.

### Get Distance
`distance <nume-1> <nume-2>`
Calculates the distance using BFS.

### Suggestions
`suggestions <nume>`
Finds friends of friends who are not yet friends with the user.

### Common Friends
`common <nume-1> <nume-2>`
Displays mutual friends.

### Number of Friends
`friends <nume>`
Shows the number of friends.

### Most Popular User
`popular <nume>`
Displays the user with the most connections.

## Posts and Reposts

Structure for posts using a tree hierarchy:

~~~
typedef struct post {
    int id, user_id, likes;
    char *title;
    struct tree_t *events;
    int liked[MAX_PEOPLE];
} post;

typedef struct post_arr_t {
    post **arr;     // array of posts
    int size;       // array size
} post_arr_t;
~~~

### Post Management Functions

### Create Post
`create <nume> <titlu>`
Creates a post for the user.

### Repost
`repost <nume> <post-id> [repost-id]`
Creates a repost and adds it to the original post's events.

### First Common Repost (LCA)
`common-repost <post> <repost-id-1> <repost-id-2>`
Finds the Lowest Common Ancestor repost.

### Like
`like <nume> <post-id> [repost-id]`
Adds/removes a like based on the user's action.

### Ratio
`ratio <post-id>`
Checks if any repost has more likes than the original post.

### Delete Post/Repost
`delete <post-id> [repost-id]`
Deletes a post and its dependent reposts.


## Feed
Utilizes all previous structures and implements these functionalities:

### Feed
`feed <nume> <feed-size>`
Displays recent posts of a user and their friends.

### View profile
`view-profile <nume>`
Shows all posts and reposts of a user.

### Friends that reposted
`friends-repost <nume> <post-id>`
Displays friends who reposted a specific post.

### Clique
`common-group <nume>`
Finds the largest group of friends containing the user using the Bron-Kerbosch algorithm.
