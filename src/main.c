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
        "banana",
        "A yellow fruit",
        "noun",
        "I ate a banana."
    );

    dictionary_insert(
        dictionary,
        "book",
        "A set of written pages",
        "noun",
        "I read a book."
    );

    printf("Dictionary Statistics:\n");

    dictionary_statistics(dictionary);

    dictionary_free(dictionary);

    return 0;
}