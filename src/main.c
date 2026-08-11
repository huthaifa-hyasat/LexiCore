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

    dictionary_insert(
        dictionary,
        "apple",
        "A fruit",
        "noun",
        "I ate an apple."
    );

    dictionary_insert(
        dictionary,
        "application",
        "A program or software",
        "noun",
        "I installed an application."
    );

    dictionary_insert(
        dictionary,
        "banana",
        "A yellow fruit",
        "noun",
        "I ate a banana."
    );

    printf("Prefix search results for 'app':\n");

    dictionary_prefix_search(dictionary, "app");

    dictionary_free(dictionary);

    return 0;
}