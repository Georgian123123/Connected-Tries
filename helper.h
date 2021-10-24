#ifndef __HELPER__
#define __HELPER__ 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Trie.h"

/* Helper functions */
void handle_add(char *line, Node *books, Node *authors);
void handle_search(char *line, Node *books);
void handle_auto_book(char *line, Node *books);
void autocomplete(Node *parent, char *prefix, int *nr);
void handle_auto_book(char *line, Node *books);
void handle_list_authors(char *line, Node *authors);
void handle_auto_authors(char *line, Node *authors);
void handle_search_by(char *line, Node *authors);
void handle_search_(char *line, Node *authors);
void delete_book(char *line, Node *authors, Node *books);
#endif /* HELPER */