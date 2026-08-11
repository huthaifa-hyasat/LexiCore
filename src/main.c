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

    int result = dictionary_edit(
        dictionary,
        "apple",
        "A round fruit that can be red or green",
        "noun",
        "I eat an apple every day."
    );

    if (result)
    {
        printf("Edit successful!\n");
    }
    else
    {
        printf("Edit failed!\n");
    }

    DictionaryEntry *entry = dictionary_search(dictionary, "apple");

    if (entry != NULL)
    {
        printf("Word: %s\n", entry->word);
        printf("Definition: %s\n", entry->definition);
        printf("Part of speech: %s\n", entry->part_of_speech);
        printf("Example: %s\n", entry->example_sentence);
    }

    dictionary_free(dictionary);

    return 0;
}