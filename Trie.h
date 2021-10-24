#ifndef TRIE_H
#define TRIE_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define alphabet_letters 68
#define LEAF 1
#define NOT_LEAF 0

FILE *out;
/* Macro to check errors got from OCW */
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

/*
 * Define the Trie Node
 * Each Node will have 26 children as the number of letters
 * from the alphabet.
 * We will add a flag to see if the letter is a leaf 
 */
typedef struct Node Node;
typedef struct Details_leaf Details_leaf;

struct Details_leaf {
	char author[40];
    int rating;
    int number_of_pages;
	int is_book;
	char book[50];
	Node *root;
};
struct Node {
    char letter;
    Node *children[alphabet_letters];
    int last_letter;
    Details_leaf details;
};

/* Function to insert a new node */
Node *new_node(char letter);

/* Free function for the parent node */
void free_node(Node *node);

/* Insert a new word in trie function */
Node *insert(Node *parent, char *word, int length_word, Details_leaf details);

/* Function to search a word in a trie */
int search(Node *parent, char *word, int length_word, Node **to_check);

/* Function that searches for a branch */
int branching_occurs(Node *parent, char *word);

/* Function to check the leaf node */
int is_leaf_node(Node *parent, char* word, int length_word);

/* Function the check the longest prefix */
char *longest_prefix(Node *parent, char *word, int length_word);

/* Function to remove the words that start with word */
Node *free_trie(Node *parent, char *word, int length_word);

/* Function to print the trie */
void print(Node *parent);

/* Function to print a word if it exists */
int print_search(Node* parent, char* word, int length_word, Node **to_check);

/* Function to get the index of an character */
int GET_INDEX(char c);

/* Function to check if a node has children */
int isLastNode(Node *parent);

/* Reverse function for GET_INDEX */
int GET_INDEX_AUX(int c);

/* Display trie lexicographic */
void display(Node *parent, char prefix[], int level);
#endif  /* _TRIE_H */