#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char *nextWord(FILE *file)
{
    int maxLength = 16;
    int length = 0;
    char *word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Validates user input. Converts string to lowercase.
 */
char *validateInput(char *input)
{
    for (int i = 0; i < strlen(input); i++)
    {
        if (!(input[i] >= 65 && input[i] <= 90) && !(input[i] >= 97 && input[i] <= 122))
        {
            return NULL;
        }
        else
        {
            if (input[i] >= 65 && input[i] <= 90)
            {
                input[i] += 32;
            }
        }
    }
    return input;
}

/**
 * Calculates Levenshtein distance.
 * Source: https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C
 */
int levenshteinDistance(char *s1, char *s2)
{
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len + 1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++)
    {
        column[0] = x;
        for (y = 1, lastdiag = x - 1; y <= s1len; y++)
        {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y - 1] == s2[x - 1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return (column[s1len]);
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE *file, HashMap *map)
{
    assert(file != NULL);
    assert(map != NULL);

    char *word = nextWord(file);

    while (word != NULL)
    {
        hashMapPut(map, word, -1);
        free(word);
        word = nextWord(file);
    }
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char **argv)
{
    HashMap *map = hashMapNew(1000);

    FILE *file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);
    int num_suggestions = 5;
    int first_five = 0;
    int distance;
    char suggestions[5][50] = {"", "", "", "", ""};
    int smallestDistance = 1000;
    int leastValuableIdx = -1;
    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("\nEnter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);

        // Implement the spell checker code here..
        char *word = validateInput(inputBuffer);

        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        }
        // If input invalid
        while (word == NULL)
        {
            printf("Invalid input. Enter one word, lowercase and uppercase letters only.\n");
            printf("Or type 'quit' to quit the program.\n");
            printf("Enter a word: ");
            scanf("%s", inputBuffer);

            if (strcmp(inputBuffer, "quit") == 0)
            {
                quit = 1;
            }

            word = validateInput(inputBuffer);
        }
        if (!quit)
        {
            // If word not in dictionary
            if (hashMapGet(map, word) == NULL)
            {
                printf("The inputted word %s is spelled incorrectly.\n", word);
                // Loop through map
                for (int i = 0; i < hashMapCapacity(map); i++)
                {
                    // Pointer for current bucket
                    struct HashLink *current = map->table[i];
                    if (current != NULL)
                    {
                        while (current != NULL)
                        {
                            distance = levenshteinDistance(word, current->key);
                            hashMapPut(map, current->key, distance);
                            current = current->next;
                        }
                    }
                }
                // Loop through map
                for (int i = 0; i < hashMapCapacity(map); i++)
                {
                    struct HashLink *current = map->table[i];
                    if (current != NULL)
                    {
                        while (current != NULL)
                        {
                            // Make first 5 inputs original suggestions
                            if (first_five < 5)
                            {
                                strcpy(suggestions[first_five], current->key);
                                if (current->value < smallestDistance)
                                {
                                    smallestDistance = current->value;
                                }
                                // Get least valuable index
                                if (current->value > levenshteinDistance(word, suggestions[leastValuableIdx]))
                                {
                                    leastValuableIdx = first_five;
                                }
                                first_five++;
                            }
                            else
                            {
                                // Replace suggestion if current value in loop is smaller
                                for (int j = 0; j < num_suggestions; j++)
                                {
                                    if (current->value < levenshteinDistance(word, suggestions[j]))
                                    {
                                        strcpy(suggestions[j], current->key);
                                        break;
                                    }
                                }
                                // Update smallest distance as needed
                                if (current->value < smallestDistance)
                                {
                                    smallestDistance = current->value;
                                }
                            }
                            current = current->next;
                        }
                    }
                }
                // Print suggestions
                printf("Did you mean...?\n");
                for (int i = 0; i < num_suggestions; i++)
                {
                    printf("%s\n", suggestions[i]);
                }
            }
            else
            {
                // Word spelled correctly
                printf("The inputted word %s is spelled correctly.\n", word);
            }
        }
    }

    hashMapDelete(map);
    return 0;
}