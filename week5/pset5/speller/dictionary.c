// Implements a dictionary's functionality

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Global variable that counts how many words were read
int words_count = 0;

// Choose number of buckets in hash table
const unsigned int N = 26 * 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get the index in our hash table
    int index = hash(word);
    // Goes at every node in our linked list, searching for the word
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        // If found (ignoring case), return true
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }
    // If the went on every node and nothing was found, return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // This is a two character hash function, i.e., it returns a value according to the first two characters in a word

    // Convert the first and the character to a lower case one and then convert it to an int (e.g., an 'a' would be 97)
    int first_letter_number = 0, second_letter_number = 0;
    first_letter_number = (int) tolower(word[0]);
    // Make the letter's number a number within the range of 0 to 25, inclusive (i.e., from 'a' to 'z'), so we return a zero-indexed number
    first_letter_number -= 97;
    if (strlen(word) > 1)
    {
        second_letter_number = (int) tolower(word[1]);
        second_letter_number -= 97;
    }
    /*
        To understand the bellow formula let's look at an example for the first two characters:
        (Beeing x the 'first_letter_number' and y the 'second_letter_number', we have the following)
        aa --> 00               x(25 + 1) + y = 0(25 + 1) + 1 = 0 + 0 = 0
        ab --> 01               x(25 + 1) + y = 0(25 + 1) + 1 = 0 + 1 = 1
        ac --> 02
        ...
        az --> 025 = 25         x(25 + 1) + y = 0(25 + 1) + 25 = 0 + 25 = 25
        ba --> 26               x(25 + 1) + y = 1(25 + 1) + 0 = 26 + 0 = 26
        bb --> 27
        ...
        bz --> 51               x(25 + 1) + y = 1(25 + 1) + 25 = 26 + 25 = 51
        ca --> 52               x(25 + 1) + y = 2(25 + 1) + 0 = 2*26 + 0 = 52
    */
    int index_number = first_letter_number * (25 + 1) + second_letter_number;
    return index_number;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    unsigned int index;
    // Open the dictionary file
    FILE *dict = fopen(dictionary, "r");
    // Check if we are out of memory. If so, return false
    if (dict == NULL)
    {
        return false;
    }
    // Create our word as a buffer for every word read in the dictionary
    char *word_buffer = malloc(LENGTH);
    // Keep reading the dictionary until it gets to it's end (EOF - End Of File)
    while (fscanf(dict, "%s", word_buffer) != EOF)
    {
        // Create a node for each word
        node *new_word = malloc(sizeof(node));
        // Check if we are out of memory to do so
        if (new_word == NULL)
        {
            return false;
        }
        // Copy the read word to the word field in our brand new node
        strcpy(new_word->word, word_buffer);
        // Update the number of read words
        words_count ++;
        //Hash word
        index = hash(new_word->word);
        // As described in the walkthrough video, first, point the brand new word to the beginning of the hash table
        new_word->next = table[index];
        // Then, point the table itself to where 'new_word' is pointing to, so we don't lose the breadcrumbs
        // That way, every new word is inserted in the beginning of our linked list
        table[index] = new_word;
    }
    free(word_buffer);
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Return the number of words we are counting in the 'load' function
    return words_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor, *tmp;
    // Itarates through every node in our hash table
    for (int i = 0; i < N; i ++)
    {
        // If the 'head' of the node is itself NULL, continue the iteration
        if (table[i] == NULL)
        {
            continue;
        }
        // Else, the cursor points to the heads points and the tmp points to the same thing as cursor does
        cursor = table[i];
        tmp = cursor;
        // Stop the loop if tmp points to a NULL value
        while (tmp != NULL)
        {
            // The cursor goes to its next node
            cursor = cursor->next;
            // We free tmp (the node where cursor was pointing to)
            free(tmp);
            // Update the tmp to where cursor currently points to
            tmp = cursor;
        }
    }
    return true;
}