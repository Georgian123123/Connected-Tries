#include "helper.h"
#define LINE_SIZE 256

/*
 * @params
 * line - input
 * books - trie with books
 * authors - trie with authors
 * @value
 * This function add from the input data in our data structures
 */
void handle_add(char *line, Node *books, Node *authors)
{
    char *token;
    char book_name[51] = {0}, author_name[40] = {0};
    int rating, pages;
    Details_leaf details, details_NULL;

    details_NULL.number_of_pages = -1;
    details_NULL.is_book = 0;
    details_NULL.rating = -1;
    details_NULL.root = NULL;
    memset(details_NULL.author, 0, 40);
    memset(details.book, 0, 50);
    
    /* jump over add_book*/
    line += 9;
    /* Get data from the input */
    token = strtok(line, ":");
    strncpy(book_name, token, strlen(token));
    token = strtok(NULL, ":");
    strncpy(author_name, token, strlen(token));
    token = strtok(NULL, ":");
    rating = atoi(token);
    token = strtok(NULL, ":");
    pages = atoi(token);

    /* fill the datas in the book trie */
    details.is_book = 1;
    details.number_of_pages = pages;
    details.rating = rating;
    details.root = NULL;
    memset(details.author, 0, 40);
    strncpy(details.author, author_name, strlen(author_name));
    books = insert(books, book_name, strlen(book_name), details);

    /* 
     * fill the datas in the author trie and also creat at the leaf
     * another trie with books
     */
    details.is_book = 0;
    details.number_of_pages = pages;
    details.rating = rating;
    details.root = NULL;
    strncpy(details.book, book_name, strlen(book_name));
    authors = insert(authors, author_name, strlen(author_name), details);
}

/*
 * @params
 * line - input
 * books - the trie with books
 * @value
 * This function search for a book
 */
void handle_search(char *line, Node *books)
{
    line += 12;
    line[strlen(line) - 1] = '\0';
    Node *to_check = NULL;
    /* Copy the title */
    if (print_search(books, line, strlen(line), &to_check) == 1) {
        fprintf(out, "Informatii recomandare: %s, %s, %d, %d\n", line,
            to_check->details.author, to_check->details.rating, to_check->details.number_of_pages);
    } else {
        fprintf(out, "Cartea %s nu exista in recomandarile tale.\n", line);
    }
}



/*
 * @params
 * parent - node which will be evaluated
 * prefix - searched prefix
 * @value
 * This function will print all the string with this prefix
 */
void autocomplete(Node *parent, char *prefix, int *nr)
{
    /* We found a string */
    if (parent->last_letter) {
        if (*nr > 0)
            fprintf(out, "%s\n", prefix);
        *nr -= 1;
    }
    
    /* All children are null */

    if (isLastNode(parent) || *nr <= 0) {
        return;
    }
  
    for (int i = 0; i < alphabet_letters; i++) {
        if (parent->children[i])
        {
            /* append character */
            prefix[strlen(prefix)] = GET_INDEX_AUX(i);
            
            autocomplete(parent->children[i], prefix, nr);
            
            /* remove last character */
            prefix[strlen(prefix) - 1] = '\0';
        }
    }
}

/*
 * @params
 * line - input
 * books - the books trie
 * @value
 * This function will print all the string that starts with prefix
 */
void handle_auto_book(char *line, Node *books)
{
    line += 12;
    line[strlen(line) - 2] = '\0';


	Node* parent = books;
    /* Check if the prefix is here and find the last character */
    int level;
    int n = strlen(line);
    for (level = 0; level < n; level++)
    {
        int index = GET_INDEX(line[level]);
  
        /* there's no string */
        if (!parent->children[index]) {
            fprintf(out, "Nicio carte gasita.\n");
            return;
        }
  
        parent = parent->children[index];
    }
  
    /* present prefix */
    int isWord = (parent->last_letter == 1);
    
    /* No children */
    int isLast = isLastNode(parent);
  
    /* Prefix present as a word but no more children */
    if (isWord && isLast) {
        fprintf(out, "%s\n", line);
        return;
    }
  
    /* Go autocomplete */
    if (!isLast)
    {   
        char prefix[LINE_SIZE] = {0};
        strncpy(prefix, line, strlen(line));
        int nr = 3;
        autocomplete(parent, prefix, &nr);
        return;
    }
}

void handle_list_authors(char *line, Node *authors)
{
    line += 12;
    line[strlen(line) - 1] = '\0';
    Node *to_check = NULL;
    if (print_search(authors, line, strlen(line), &to_check) == 1) {
        Node *to_print = to_check->details.root;
        char prefix[50] = {0};
        int level = 0;
        display(to_print, prefix, level);
    } else {
        fprintf(out, "Autorul %s nu face parte din recomandarile tale.\n", line);
    }
}

void handle_auto_authors(char *line, Node *authors)
{
    line += 12;
    line[strlen(line) - 2] = '\0';
	Node *parent = authors;
    /* Check if the prefix is here and find the last character */
    int level;
    int n = strlen(line);
    for (level = 0; level < n; level++)
    {
        int index = GET_INDEX(line[level]);

        /* there's no string */
        if (!parent->children[index]) {
            fprintf(out, "Niciun autor gasit.\n");
            return;
        }
        parent = parent->children[index];
    }
  
    /* present prefix */
    int isWord = (parent->last_letter == 1);
    
    /* No children */
    int isLast = isLastNode(parent);
  
    /* Prefix present as a word but no more children */
    if (isWord && isLast) {
        fprintf(out, "%s\n", line);
        return;
    }
  
    /* Go autocomplete */
    if (!isLast)
    {   
        char prefix[LINE_SIZE] = {0};
        strncpy(prefix, line, strlen(line));
        int nr = 3;
        autocomplete(parent, prefix, &nr);
        return;
    }
}

void handle_search_by(char *line, Node *authors)
{   
    char author[40], book[51], *token;
    memset(author, 0, 40);
    memset(book, 0, 50);
    line += 17;
    line[strlen(line) - 1] = '\0';

    token = strtok(line, ":");
    strncpy(author, token, strlen(token));
    token = strtok(NULL, ":");
    strncpy(book, token, strlen(token));
    Node *to_check = NULL;

    if (print_search(authors, author, strlen(author), &to_check) == 1) {
        Node *books = to_check->details.root;
        
        to_check = NULL;
        /* Copy the title */
        if (print_search(books, book, strlen(book), &to_check) == 1) {
            fprintf(out, "Informatii recomandare: %s, %s, %d, %d\n", book,
                to_check->details.author, to_check->details.rating, to_check->details.number_of_pages);
        } else {
            fprintf(out, "Cartea %s nu exista in recomandarile tale.\n", book);
        }
    } else {
        fprintf(out, "Autorul %s nu face parte din recomandarile tale.\n", author);
    }
}

void handle_search_(char *line, Node *authors)
{
    char author[40], book[51], *token;
    memset(author, 0, 40);
    memset(book, 0, 50);
    line += 17;
    line[strlen(line) - 2] = '\0';

    token = strtok(line, ":");
    strncpy(author, token, strlen(token));
    token = strtok(NULL, ":");
    strncpy(book, token, strlen(token));
    Node *to_check = NULL;

    if (print_search(authors, author, strlen(author), &to_check) == 1) {
        Node *books = to_check->details.root;
        Node *parent = books;
        /* Check if the prefix is here and find the last character */
        int level;
        int n = strlen(book);
        for (level = 0; level < n; level++)
        {
            int index = GET_INDEX(book[level]);
    
            /* there's no string */
            if (!parent->children[index]) {
                fprintf(out, "Nicio carte gasita.\n");
                return;
            }
    
            parent = parent->children[index];
        }
    
        /* present prefix */
        int isWord = (parent->last_letter == 1);
        
        /* No children */
        int isLast = isLastNode(parent);
    
        /* Prefix present as a word but no more children */
        if (isWord && isLast) {
            fprintf(out, "%s\n", line);
            return;
        }
    
        /* Go autocomplete */
        if (!isLast)
        {   
            char prefix[LINE_SIZE] = {0};
            strncpy(prefix, book, strlen(book));
            int nr = 3;
            autocomplete(parent, prefix, &nr);
            return;
        }
    } else {
        fprintf(out, "Autorul %s nu face parte din recomandarile tale.\n", author);
    }
}

/* Function to count number of words */
int wordCount(Node *parent)
{
    int result = 0;
  
    // Leaf denotes end of a word
    if (parent->last_letter)
        result++;
      
    for (int i = 0; i < alphabet_letters; i++)    
      if (parent->children[i])
         result += wordCount(parent->children[i]);
    return result;   
}

/* Function to delete a book
 * First delete from books, after that from books from
 * the author, and after that if the size of the trie is 0
 * remove from the authors the author.
 */
void delete_book(char *line, Node *authors, Node *books)
{
    char author[40];
    memset(author, 0, 40);
    line += 12;
    line[strlen(line) - 1] = '\0';

    Node *to_check = NULL;
    if (search(books, line, strlen(line), &to_check) == 1) {
        strncpy(author, to_check->details.author, strlen(to_check->details.author));
        to_check = NULL;
        if (print_search(authors, author, strlen(author), &to_check) == 1) {
            free_trie(to_check->details.root, line, strlen(line));
            if (wordCount(to_check->details.root) == 0) {
                free_node(to_check->details.root);
                free_trie(authors, author, strlen(author));
            }
        }
        free_trie(books, line, strlen(line));
    } else {
        fprintf(out, "Cartea %s nu exista in recomandarile tale.\n", line);
    }
}