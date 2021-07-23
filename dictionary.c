// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536; //Got this hashtable size from https://cs50.stackexchange.com/questions/37209/pset5-speller-hash-function

// Hash table
node *table[N];

// Counter for the number of words in dictionary, making it a global variable so all functions have access.
int dic_words = 0;
// Global variable that is true if dictionary is loaded, and false if dictionary is not loaded. Initially will be false.
bool loaded = false;


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hashindex = hash(word);
    
    for (node *tmp = table[hashindex]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word) //Got this hash function online from https://cs50.stackexchange.com/questions/37209/pset5-speller-hash-function
{
    // TODO
    unsigned int hash_value = 0;
        for (int i = 0, n = strlen(word); i < n; i++)
        {
            hash_value = (hash_value << 2) ^ (tolower(word[i]));
        }
        return hash_value % N; //N is size of hashtable
    return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    for (int i = 0; i < N; i++)             // Makes every linkedlist in the hashtable point to NULL initially.
    {
        table[i] = NULL;
    }
    
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    char buffer[LENGTH]; // Temporary buffer to store the words from dictionary in each while loop iteration.
    
    while (fscanf(file, "%s", buffer) != EOF) // This while loop will make a new node for each new word in each iteration, and will append the node
    {                                         // to the start of the corresponding linkedlist in the hashtable according to the hashindex the "hash" function returns.
        node *n = malloc(sizeof(node));
         if (n == NULL)
        {   
            return NULL;
        }
        strcpy(n->word, buffer);
        int hashindex = hash(n->word);
        n->next = table[hashindex]; // Makes the next pointer of "n" point to the start of the linkedlist at the hashindex so we dont lose the rest of the linkedlist.
        table[hashindex] = n;       // Makes the start of the linkedlist at the hashindex point to the new node "n".
        dic_words++;                // A counter variable that keeps track of the number of words being loaded in from dictionary.
    }
    
    if (fscanf(file, "%s", buffer) == EOF)
    {
        fclose(file);
        loaded = true;
        return true;
    }
    else
    {
        return false;
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (loaded == true)
    {
        return dic_words;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
