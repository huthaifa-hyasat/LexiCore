#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stddef.h>

typedef struct DictionaryEntry
{
    char *word;
    char *definition;
    char *part_of_speech;
    char *example_sentence;

    struct DictionaryEntry *next;
} DictionaryEntry;

typedef struct Dictionary
{
    DictionaryEntry **buckets;
    size_t bucket_count;
    size_t entry_count;
} Dictionary;

#endif