Echipa tema 3 SD:
teodora.plopeanu
bogdan.popescu2811

**Nume: Teodora-Anca Plopeanu**
**Grupă: 312CA**
**ID: teodora.plopeanu**

**Nume: Bogdan-Stefan Popescu**
**Grupă: 312CA**
**ID: bogdan.popescu2811**

# Tema 3 - Facebook

Aceasta tema implementeaza un internetwork, asemanator retelei de socializare Facebook.
Structurile de date utilizate in rezolvare au fost: grafuri, arbori, coada.

Am construit urmatoarea structura de date a network-ului:
~~~
typedef struct retea
{
	int **db_matrix;		// matricea de adiacenta a grafului 
	int size;				// dimensiunea
	int id;					// id-ul curent, ce va fi atribuit urmatoarei postari
							// create, urmand a fi incrementat
	post_arr_t *post_arr;	// array ce contine toate postarile facute in retea
} retea;
~~~

## Task 1 - Retea de prietenie

Pentru acest task am implementat urmatoarele functii:
* **Agauga prieten**
		`add <nume-1> <nume-2>`
		Creeaza o legatura bidirectionala intre doi utilizatori. Pune 1 in matricea de adiacenta cu utilizatorii retelei, de dimensiune MAX_PEOPLE x MAX_PEOPLE.
		
* **Elimina prieten** 
	`remove <nume-1> <nume-2>`
	Elimina legatura dintre doi utilizatori, cu alte cuvinte pune 0 in matricea de adiacenta.

* **Afla distanta**
	`distance <nume-1> <nume-2>`
	Afla distanta dintre doi utilizatori, cu ajutorul unui algoritm ce functioneaza pe baza unei cozi, in sesul parcurgerii in latime a grafului (BFS).

* **Sugestii**
	`suggestions <nume>`
	Găsește și afișează toți prietenii prietenilor, care nu sunt deja prieteni cu acel utilizator.
	db_matrix[nume][sugestie] == 0 && exista friend, a.i. (db_matrix[nume][friend] == 1 && db_matrix[friend][sugestie] == 1)

* **Prieteni comuni**
	 `common <nume-1> <nume-2>`
	 Afiseaza prietenii comuni dintre user-1 si user-2.
	 In db_matrix, trebuie sa avem 1 si pe [common_friend][user-1] si pe [common_friend][user-2].

* **Număr de prieteni**
	`friends <nume>`
	Afiseaza numarul de prieteni ai unei persoane, adica suma liniei corespunzatoare lui din matrice (numarul de valori de 1).

* **Cel mai popular**
	`popular <nume>`
	Afiseaza userul cu cele mai multe conexiuni dintre user si prietenii lui. 
	Pentru ajutor, am implementat urmatoarea functie ce returneaza numarul de prieteni al unii user:
	~~~
	int get_nr_of_friends(retea *rt, char *user)
	{
		int idx = get_user_id(user);
		int nr = 0;
		for (int i = 0; i < MAX_PEOPLE; i++)
			nr += rt->db_matrix[idx][i];
		return nr;
	} ~~~

## Task 2 - Postări și reposturi

Pentru acest task, am implementat urmatoarea structura de date, numita `post`, ce foloseste structura `tree` pentru a construi structura arborescenta de repost-uri:

~~~
struct tree_t
{
	int parent_id, n_children;
	post **children;
};

typedef struct post
{
	int id, user_id, likes;
	char *title;
	struct tree_t *events;
	int liked[MAX_PEOPLE];
} post;
~~~

Reposturile au aceeasi structura ca si postarile.

Postarile retelei sunt retinute in campul `post_arr` din `rt` (reteaua), ce are, la randul sau, urmatoarea structura:

~~~
typedef struct post_arr_t
{
	post **arr;		// vector de pinteri la postari
	int size;		// dimensiunea
} post_arr_t;
~~~

Acest task se foloseste de urmatoarele functii:

### Creeaza postare
`create <nume> <titlu>`
Creeaza postarea cu titlul `titlu`, in numele user-ului `nume`.
Aloca memorie pentru o structura de tip `post` si ii completeaza campurile cu valorile corespunzatoare.

### Repost
`repost <nume> <post-id> [repost-id]`
Creeaza un repost la respectiva postare, ce poate fi, la randul ei, un repost. Cu alte cuvinte, creeaza o structura de tip `post`, pe care o adauga in vectorul `events->children`, al (re)postarii originale. In ajutor, am implementat functia `get_parent`, ce cauta si returneaza un repost cu un anumte id, pornind de la postarea initiala (root):
~~~
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
~~~


### Primul repost comun (LCA)
`common-repost <post> <repost-id-1> <repost-id-2>`

Găsește primul repost comun al două reposturi.

 Creeaza doi vectori, cate unul pentru fiecare repost, ce retin, in ordine inversa, nodul din care pornim si toti parintii lui din ierarhia de reposturi, pana la postarea originala. Lowest Common Ancestor va fi, in acest caz, primul nod ce se regaseste in ambii vectori.

### Like
`like <nume> <post-id> [repost-id]`

Adaugă un like la o postare sau repostare.

In acest sens, ne-au foat utile campurile `likes` si `liked[]` din cadrul structurii `post`.
`likes` retine numarul total de like-uri, in timp ce `liked` este un vector de frecventa ce retine ce utilizatori au dat like respectivei postari.

Ne este util, deoarece, atunci cand un utilizator da de doua ori like aceleiasi postari, se considera ca fiind un dislike.

### Ratio
`ratio <post-id>`

Dacă un repost are mai multe like-uri decât postarea originală, spunem că această i-a dat ratio. Astfel, acautam daca exista vreo repostare din ierarhia events, ce are mai multe like-uri decat postarea originala:
~~~
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
~~~
Functia `get_max_likes` cauta maximul si id-ul respectiv, recursiv, in ierarhie:

~~~
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
~~~

### Delete Post/Repost

`delete <post-id> [repost-id]`

Șterge o postare sau un repost. Odată ce o postare sau un repost a fost șters, toate reposturile care depind de aceasta vor fi șterse.


## Task 3 - Feed
Aici ne vom folosi de toate structurile anterior definite.

Avem urmatoarele functii:

### Feed
`feed <nume> <feed-size>`

Afișează cele mai recente  `<feed-size>`  postări ale unui utilizator și ale prietenilor lui.

 Ia la rand, in ordine descrescatoare, ultimele id-uri adaugate in retea. Cauta postarea resoectiva si verifica daca aceatsa apartine user-ului repsectiv sau a unui prieten de-al lui. In caz afirmativ, afisam postarea si decrementam contorul de postari ce mai trebuie gasite.


### View profile
`view-profile <nume>`

Afișează toate postările și repostările unui utilizator.

Ia la rand toate postarile si rpeosturile din retea si verifica daca acestea apartin acestu utilizator. In caz afirmativ, le afiseaza.

### Friends that reposted

`friends-repost <nume> <post-id>`

Afișează toți prietenii unui utilizator care au dat repost la postarea cu  `<post-id>`-ul primit.

Ia la rand toate repost-urile din events-ul acelei postari si verifica daca au fost facute de un prieten.

### Clică

`common-group <nume>`

Găseste și afișeaza cel mai mare grup de prieteni care îl conține pe un anumit utilizator.

Am folosit, in implementare, algoritmul **Bron-Kerbosch**, ce foloseste trei colectii de noduri: X, R si B.
Găsește clicurile maxime care includ toate vârfurile din R, unele dintre vârfurile din P și niciunul dintre vârfurile din X. În fiecare apel la algoritm, P și X sunt mulțimi disjunse a căror unire constă din acele vârfuri care formează clicuri atunci când sunt adăugate la R. Cu alte cuvinte, P ∪ X este mulțimea de vârfuri care sunt unite la fiecare element al lui R. Când P și X sunt ambele goale, nu există alte elemente care pot fi adăugate la R, deci R este un clica maximă și algoritmul dă rezultate R. (Wikipedia)

Am implementat algoritmul recursiv:
~~~
void clica_bron(retea *rt, int *r, int *p, int *x, int r_size,
				int p_size, int x_size, int *final, int *final_size)
{
	if (p_size == 0 && x_size == 0) {
		if (r_size > *final_size) {
			*final_size = r_size;
			for (int i = 0; i < r_size; i++)
				final[i] = r[i];
		}
		return;
	}

	for (int i = 0; i < p_size; i++) {
		int v = p[i];
		int *newr = calloc(MAX_PEOPLE, sizeof(int));
		int *newp = calloc(MAX_PEOPLE, sizeof(int));
		int *newx = calloc(MAX_PEOPLE, sizeof(int));

		//newr
		for (int j = 0; j < r_size; j++)
			newr[j] = r[j];

		newr[r_size] = v;
		//newp
		int newp_size = 0;
		for (int j = 0; j < p_size; j++) {
			if (rt->db_matrix[v][p[j]])
				newp[newp_size++] = p[j];
		}

		// newx
		int newx_size = 0;
		for (int j = 0; j < x_size; j++) {
			if (rt->db_matrix[v][x[j]])
				newx[newx_size++] = x[j];
		}

		clica_bron(rt, newr, newp, newx, r_size + 1, newp_size, newx_size,
				   final, final_size);

		for (int j = 0; j < p_size; j++) {
			if (p[j] == v) {
				for (int k = j; k < p_size - 1; k++)
					p[k] = p[k + 1];
				break;
			}
		}
		p_size--;
		x[x_size++] = v;
		free(newr);
		free(newp);
		free(newx);
	}
}
~~~

