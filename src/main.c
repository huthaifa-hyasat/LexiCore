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

    printf("Entries before free: %zu\n", dictionary->entry_count);

    dictionary_free(dictionary);

    printf("Dictionary memory freed successfully!\n");

    return 0;
}