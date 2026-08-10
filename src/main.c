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
        "book",
        "A set of written pages",
        "noun",
        "I read a book."
    );

    if (dictionary_save(dictionary, "data/saved_dictionary.txt"))
    {
        printf("Dictionary saved successfully!\n");
    }
    else
    {
        printf("Failed to save dictionary!\n");
        dictionary_free(dictionary);
        return 1;
    }

    dictionary_free(dictionary);

    return 0;
}