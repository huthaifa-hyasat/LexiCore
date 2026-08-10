#include <stdio.h>
#include "hash_table.h"

int main(void)
{
    Dictionary *dictionary = dictionary_create(101);

    if (dictionary == NULL)
    {
        printf("Dictionary creation failed!\n");
        return 1;
    }

    int first_insert = dictionary_insert(
        dictionary,
        "apple",
        "A fruit",
        "noun",
        "I ate an apple."
    );

    int duplicate_insert = dictionary_insert(
        dictionary,
        "apple",
        "Another definition",
        "noun",
        "I like apples."
    );

    printf("First insert: %s\n",
           first_insert ? "PASS" : "FAIL");

    printf("Duplicate insert: %s\n",
           duplicate_insert == 0 ? "PASS" : "FAIL");

    printf("Total entries: %zu\n", dictionary->entry_count);

    dictionary_free(dictionary);

    return 0;
}