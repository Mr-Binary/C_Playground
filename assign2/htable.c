/**
 * HASH TABLE ADT
 * Stores and manages a collection of strings using either a linear probing or double hash function.
 * Uses htable: definitions of htable data structure
 * Uses mylib: memory allocation and getword functions.
 * @Author foxre274 gaoha773 scrca599
 * @Date 10/Sep/2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

#define DEFAULT_CAPACITY 113

struct htablerec {
    unsigned int capacity; /* Maximum number of keys. */
    int num_keys; /* Current number of keys stored.  */
    int *freq; /* array of integers to store number of occurrences of each word (not case sensitive)  */
    int *stats; /*  number of collisions, filled sequentially and indexed by num_keys  */
    char **items; /* array of keys (strings)  */
    hashing_t method; /* the type of the hash table  */
};


/**
 * Creates a new instance of a Hash table with a default size of 113 if 
 * no capacity passed (which means the capacity = 0).
 *
 * @param capacity as maximum hash table size. 
 * @param type the type of the hash function used to store keys.

 * @return an empty hash table of size capacity.
 */

htable htable_new(int capacity, hashing_t type){
    unsigned int i;
    htable result = emalloc(sizeof * result);
    
    if (capacity == 0){
        result-> capacity = DEFAULT_CAPACITY;
    } else {
        result-> capacity = capacity;    
    }
    
    result -> num_keys = 0;
    result -> method = type;

    result -> freq = emalloc(result->capacity * sizeof result->freq[0]);
    result -> items = emalloc(result->capacity * sizeof (char*));
    result -> stats = emalloc(result->capacity * sizeof result->stats[0]);

    for (i = 0; i < result->capacity; i++){
        result -> freq[i] = 0;
        result -> stats[i] = 0;
        result->items[i] = NULL;
    }
    return result;
}

/**
 * Print the items in the hash table.
 * @param h the hash table to be printed.
 * @param f the function we need to call when iterating the hash table.
 */ 
void htable_print(htable h, void f()){
    unsigned int i;
    for (i = 0; i < h->capacity; i++){
        if (h->items[i] != NULL){
            f(h->freq[i], h->items[i]);
        }
    }
}

/**
 * Creates an integer key from a string.
 * Used for linear and double hashing.
 *
 * @param word a word used for key calculations.
 * @return unsigned integer representing the word.
 */
static unsigned int word_to_int(char *word){
    unsigned int result = 0;
    while (*word != '\0') {
        result = (*word++ +31 *result);
    }
    return result;
}

/**
 * Creates a step size used for hashing.
 *
 * @param h the hash table to use capacity value.
 * @param i_key the current integer key value used to calculate the step size for double hashing.
 *
 * @return step size, which is 1 if hashing method is linear probing, otherwise
 * a calculated step size using i_key is returned.
 */
static unsigned int htable_step(htable h, unsigned int i_key){

    if ( h->method == LINEAR_P ){
        return 1;
    } else {
        return 1 + (i_key % ( h->capacity -1 ));
    }
}

/**
 * Print the Hash table Contents in the form of a 
 * table with the following columns:
 * Pos  Freq  Stats  Word
 * 
 * @li Pos - hash table position.
 * @li Freq - how many occurrences of the word in the text at the given position.
 * @li Stats - number of collisions that occurred, in sequential order.
 * @li Word - Word stored in hash table at the given position.
 *
 * @param h hash table.
 * @param FILE stream to output information to.
 */
void htable_print_entire_table(htable h, FILE *stream){
    unsigned int i;
    char *word;

    fprintf(stream, "  Pos  Freq  Stats  Word\n");
    fprintf(stream,"________________________________________\n");
    for(i = 0; i < h->capacity; i++){
        if ( h->items[i] == NULL ) {
            word = "\0";
        }
        else {
            word = h->items[i];
        }
        fprintf(stream,"%5d %5d %5d   %s\n",i, h->freq[i], h->stats[i], word);
    }
}

/**
 * Deallocates memory which was initially allocated for the object's
 * construction.
 *
 * @param h table to free.
 */
void htable_free(htable h){

    unsigned int i;

    for (i = 0; i < h->capacity; i++){
        if ( h->items[i] != NULL ) {
            free(h->items[i]);
        }
    }

    free (h->freq);
    free (h->items);
    free (h->stats);

    free(h);
}

/**
 * Inserts a word into a hash table.
 * Uses word_to_int and h_table_step to work the key and step size.
 * The word is inserted into the first available position.
 * 
 * First we use a while loop to find the proper position for the new word.
 * If the position is already occupied, we will increase the collisions and
 * increase the index.
 * 
 * Then we will insert word into this index, otherwise increase the frequency.
 *
 * @param h hash table to insert word into.
 * @param str word to insert into hash table.
 *
 * @return 0 if table has no available space for a new word (full), otherwise
 * frequency of word occurrence if word is already in the table, or 1 if a new
 * word is placed into an empty cell.
 */
int htable_insert(htable h, char *str){

    unsigned int key = word_to_int(str)%h->capacity;
    int steps = htable_step(h, word_to_int(str));
    unsigned int collisions = 0;

    while ( h->items[key] != NULL &&
            strcmp(str, h->items[key]) != 0 &&
            collisions < h->capacity){
        key  = key + steps;
        if (key >= h->capacity){
            key = key - h->capacity;
        }
        collisions++;
    }

    if (h->items[key] == NULL){
        h->items[key] = emalloc((strlen(str)+1)*sizeof h->items[0][0]);
        strcpy(h->items[key], str);
        h->stats[h->num_keys] = collisions;
        h->num_keys++;
        h->freq[key]++;
        return 1;
    } else {
        if (strcmp(str,h->items[key]) == 0 ){
            h->freq[key]++;
            return h->freq[key];
        } else {
            return 0;
        }
    }
}

/**
 *  Search for a word in the hash table.
 *  Uses htable_step and word_to_int to find step size and key for searching.
 *
 * @param h the hash table to be searched.
 * @param str the word to be searched for.
 *
 * @return 0 if the number of collisions is greater than the size of the table,
 * showing the word doesn't exist. (Prime sized table should ensure all cells in
 * table visited).
 * If the word is found, the number of occurrences is returned.
 */
int htable_search(htable h, char *str){
    int step = htable_step(h, word_to_int(str));
    unsigned int check = word_to_int(str)%h->capacity;
    unsigned int collisions = 0;

    while ( h->items[check] != NULL &&
            strcmp(str, h->items[check]) !=0 &&
            collisions < h->capacity){

        check  = check + step;
        if (check >= h->capacity){
            check = check - h->capacity;
        }
        collisions++;
    }

    if (collisions == h->capacity){
        return 0;
    }else {
        return h->freq[check];
    }
}

/**
 * Prints out a line of data from the hash table to reflect the state the table
 * was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h the hash table.
 * @param stream a stream to print the data to.
 * @param percent_full the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            }
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }

        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full,
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {

    int i;

    fprintf(stream, "\n%s\n\n",
            h->method == LINEAR_P ? "Linear Probing" : "Double Hashing");
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "-----------------------------------------------------\n\n");
}
