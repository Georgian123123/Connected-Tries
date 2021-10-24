#include "Trie.h"

int isLastNode(Node *parent)
{
    for (int i = 0; i < alphabet_letters; i++)
        if (parent->children[i])
            return 0;
    return 1;
}

void display(Node *parent, char prefix[], int level)
{
    /* If node is leaf node, it indicates end
     * of string, so a null character is added
     * and string is displayed
     */
    if (parent->last_letter == LEAF) {
        prefix[level] = '\0';
        fprintf(out, "%s\n", prefix);
    }
  
    int i;
    for (i = 0; i < alphabet_letters; i++) 
    {
        /* if NON NULL child is found
         * add parent key to str and
         * call the display function recursively
         * for child node
         */
        if (parent->children[i]) 
        {
            prefix[level] = GET_INDEX_AUX(i);
            display(parent->children[i], prefix, level + 1);
        }
    }
}

int GET_INDEX(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a';
    else if (c >= 'A' && c <= 'Z')
        return c - 'A' + 26;
    else if (c >= '0' && c <= '9')
        return c - '0' + 52;
    else if (c == '.')
        return c - '.' + 62;
    else if (c == '-')
        return c - '-' + 63;
    else if (c == '\'')
        return c - '\'' + 64;
    else if (c == '?')
        return c - '?' + 65;
    else if (c == '!')
        return c - '!' + 66;
    else if (c == ' ')
        return c - ' ' + 67;
    return -1;
}

int GET_INDEX_AUX(int c)
{   
    if (c >= 0 && c < 26)
        return c + 97;
    else if (c >= 26 && c < 52)
        return c - 26 + 65;
    else if (c >= 52 && c < 62)
        return c - 52 + 48;
    else if (c == 62)
        return 46;
    else if (c == 63)
        return 45;
    else if (c == 64)
        return 39;
    else if (c == 65)
        return 63;
    else if (c == 66)
        return 33;
    else if (c == 67)
        return 32;
    return -1;
}

/*
 * @params
 * letter - the letter which the node should contain
 * @value
 * This function will return the new created node
 */
Node *new_node(char letter)
{
    /* Alloc memory for the new node */
    Node *new_nd = malloc(sizeof(Node));
    DIE(new_nd == NULL, "Failed to alloc new node");

    /* initialise its children */
    for (int i = 0; i < alphabet_letters; ++i)
        new_nd->children[i] = NULL;
    
    /* no leaf */
    new_nd->last_letter = NOT_LEAF;
    /* set the letter */
    new_nd->letter = letter;

    return new_nd;
}

/*
 * @params
 * node - the node which will be freed
 * @value
 * This function will free the node children by children
 * as a recursive function
 * In the end will free the entire node
 */
void free_node(Node *node)
{
    /* We will traverse each children and we will
     * call separate this function.
     * After that we will free the memory for the node
     */
    for (int i = 0; i < alphabet_letters; ++i)
        if (node->children[i])
            free_node(node->children[i]);
    
    free(node);
}

/*
 * @params
 * parent - the above letter which the word will start
 * word - the word that will be inserted
 * length_word - length of the world
 * @value
 * This function will insert a word in the trie
 */
Node *insert(Node *parent, char *word, int length_word, Details_leaf details)
{
    /* We'll use an auxiliary pointer to not destroy the above word */
    Node *temporary_node = parent;

    for (int i = 0; i < length_word; ++i) {
        /*
         * I'll asume that the the letters are lower case letter
         * After that I will use an algorithm to convert the words
         */
        int index = GET_INDEX(word[i]);

        if (!temporary_node->children[index])
            /* If the node doesn't exist, just create it */
            temporary_node->children[index] = new_node(word[i]);
        
        /* move the bellow level since we found a match between prefixes */
        temporary_node = temporary_node->children[index];
    }
    /* At the end of the word mark it as a leaf */
    temporary_node->last_letter = LEAF;
    if (details.is_book == 0) {
        details.is_book = 1;
        if (temporary_node->details.root == NULL) {
            temporary_node->details.root = new_node('\0');
        }
        temporary_node->details.root = insert(temporary_node->details.root, details.book, strlen(details.book), details);
    } else {
        memcpy(&temporary_node->details, &details, sizeof(details));
    }
    return parent;
}

/*
 * @params
 * parent - the current node
 * word - the word to be searched
 * length_word - the length of the word
 * @value
 * This function will search for a word in the Trie
 */
int search(Node *parent, char *word, int length_word, Node **to_check)
{
    /* We'll search for the word in the trie */
    Node *temporary_node = parent;

    /* Iterate through the word to check its letters */
    for (int i = 0; i < length_word; ++i) {
        /* Get the index */
        int index = GET_INDEX(word[i]);

        /* If we don't find a match just return 0*/
        if (!temporary_node->children[index])
            return 0;

        /* Move to the bellow level */
        temporary_node = temporary_node->children[index];
    }

    /* Check if we are at the final and if the temporary node is still in trie */
    if (temporary_node != NULL && temporary_node->last_letter == LEAF) {
        *to_check = temporary_node;
        return 1;
    }

    return 0;
}

/*
 * @params
 * parent - the current node
 * word - the word with the branching will be searched
 * @value
 * Return the last position where the branching occurs
 */
int branching_occurs(Node *parent, char *word)
{
    /* We'll need to check if there's a branching at the last character
     * of the word.
     */
    Node *temporary_node = parent;

    /* Get the length of the word */
    int length_word = strlen(word);
    if (length_word == 0)
        return 0;
    
    /* Go to the largest index where branching occurs */
    int old_index = 0;
    for (int i = 0; i < length_word; ++i) {
        /* Get the index of the word */
        int index = GET_INDEX(word[i]);
        /* Check if the child exists */    
        if (temporary_node->children[index]) {
            /* Child exists, so we'll check if a branching exists */
            for (int j = 0; j < alphabet_letters; j++) {
                if (j != index && temporary_node->children[j]) {
                    /* Found another children so here's a branch */
                    old_index = i + 1;
                    break;
                }
            }
            /* move one level bellow */
            temporary_node = temporary_node->children[index];
        }
    }
    return old_index;
}

/*
 * @params
 * parent - the current node
 * word - the word which will be searched for the leaf
 * length_word - the length of the word
 * @value
 * This function checks if this word contains the last letter
 */
int is_leaf_node(Node *parent, char* word, int length_word) {
    Node *temporary_node = parent;

    /* Traverse the word */
    for (int i = 0; i < length_word; ++i) {
        /* Get the index of the word */
        int index = GET_INDEX(word[i]);
        /* Check if the children exists */
        if (temporary_node->children[index]) {
            /* If so go a level bellow */
            temporary_node = temporary_node->children[index];
        }
    }
    return temporary_node->last_letter;
}

/*
 * @params
 * parent - the current node
 * word - the word for the longest prefix
 * length_word - the length of the word
 * @value
 * This function will return the longest prefix
 */
char *longest_prefix(Node *parent, char *word, int length_word)
{   
    /* Check if the word exists */
    if (!word || word[0] == '\0') {
        return NULL;
    }
    
    /*
     * Initially, the longest prefix will be the word itself
     * After that, we'll go backtracking from the deepest index
     * to a point of a divergence, if there's one
     */

    char *longest = malloc(length_word + 1 * sizeof(char));
    DIE(longest == NULL, "Failed to allocate longest word");

    /* Copy the word */
    strncpy(longest, word, length_word);
    longest[length_word] = '\0';
    /* Check if there's a branch
     * In case of NO, the longest word it'll be the original string
     */
    int branch_index = branching_occurs(parent, longest) - 1;
    if (branch_index >= 0) {
        /*
         * Found a branch
         * Update the index to the longest match
         * Update the longest prefix by the branch index length
         */
        longest[branch_index + 1] = '\0';
        longest = realloc(longest, (branch_index + 1) * sizeof(char));
        DIE(longest == NULL, "Failed to reallocate the longest prefix");
    }
    return longest;
}

/*
 * @params
 * parent - the current node
 * word - the word which will be removed
 * length_word - the length of the word
 * @value
 * This function will remove every word with word prefix
 */
Node *free_trie(Node *parent, char *word, int length_word)
{
    /* We'll delete the trie if the last letter is a leaf */
    Node *temporary_node = parent;
    int i = 0;
    if (!parent)
        return NULL;
    if (!word || word[0] == '\0')
        return parent;

    /* If the node that corresponds the match is not a leaf then STOP */
    if (!is_leaf_node(parent, word, strlen(word)))
        return parent;

    /* Find the longest prefix that is not the string itself */
    char *longest = longest_prefix(parent, word, strlen(word));
    if (longest[0] == '\0') {        
        free(longest);
        return parent;
    }
    
    /* Tracking the position in the trie */
    int length_longest = strlen(longest);
    for (i = 0; i < length_longest; i++) {
        /* Get the index */
        int index = GET_INDEX(longest[i]);
        /* Check if the children exists */
        if (temporary_node->children[index]) {
            /* Move a level bellow */
            temporary_node = temporary_node->children[index];
        } else {
            /* no word */
            free(longest);
            return parent;
        }
    }
    
    /* 
     * We got a match between the 2 strings.
     * Now we are at the deepest level
     * Delete the sequence
     */

    if (i == length_word) {
        temporary_node = parent;
        for (int i = 0; i < length_word; i++) {
            int index = GET_INDEX(word[i]);
            if (temporary_node->children[index]) {
                Node *to_remove = temporary_node->children[index];
                temporary_node->children[index] = NULL;
                free_node(to_remove);
            }
        }
    } else {
        for (; i < length_word; i++) {
            int index = GET_INDEX(word[i]);
            if (temporary_node->children[index]) {
                Node *to_remove = temporary_node->children[index];
                temporary_node->children[index] = NULL;
                free_node(to_remove);
            }
        }
    }
    free(longest);
    return parent;
}

/*
 * @params
 * parent - the current node
 * @value
 * This function will print the trie
 */
void print(Node *parent)
{
    /* Prints the trie */
    if (!parent)
        return;
    Node *temporary_node = parent;

    /* Print every character */
    fprintf(out, "%c -> ", temporary_node->letter);
    for (int i = 0; i < alphabet_letters; ++i)
        print(temporary_node->children[i]);
}

/*
 * @params
 * parent - the current node
 * word - the word that will be printed
 * length_word - the length of the word
 * @value
 * This function will print the word if it exists
 */ 
int print_search(Node* parent, char* word, int length_word, Node **to_check)
{   
    if (search(parent, word, strlen(word), to_check) == 0)
        return 0;
    else
        return 1;
}