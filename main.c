#include "Trie.h"
#include "helper.h"

#define LINE_SIZE 256
int main(int argc, char *argv[])
{
    Node *books = new_node('\0');
    Node *authors = new_node('\0');
    FILE *in;
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w+");
    char line[LINE_SIZE] = {0};

    while (fgets(line, LINE_SIZE, in)) {
        char *verify = strstr(line, ":");
        if (strncmp(line, "add_book", 8) == 0) {
            handle_add(line, books, authors);
        } else if (strncmp(line, "search_book", 11) == 0
                    && line[strlen(line) - 2] == '~') {
            handle_auto_book(line, books);
        } else if (strncmp(line, "search_book", 11) == 0) {
            handle_search(line, books);
        } else if (strncmp(line, "list_author", 11) == 0
                    && line[strlen(line) - 2] == '~') {
            handle_auto_authors(line, authors);
            continue;
        } else if (strncmp(line, "list_author", 11) == 0) {
            handle_list_authors(line, authors);
        } else if (strncmp(line, "search_by_author", 16) == 0
                    && line[strlen(line) - 2] == '~'
                    && verify == NULL) {
            char *line2 = line;
            line2 += 5;
            handle_auto_authors(line2, authors);
            continue;
        } else if (strncmp(line, "search_by_author", 16 ) == 0
                    && line[strlen(line) - 2] == '~'
                    && verify != NULL) {
            handle_search_(line, authors);
            continue;
        } else if (strncmp(line, "search_by_author", 16) == 0) {
            handle_search_by(line, authors);
        } else if (strncmp(line, "delete_book", 11) == 0) {
            delete_book(line, authors, books);
        }
    }
    return 0;
}