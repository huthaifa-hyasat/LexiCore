#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include "dictionary.h"

#define DEFAULT_BUCKET_COUNT 101

size_t hash_function(const char *word, size_t bucketCount);

Dictionary *dictionary_create(size_t bucketCount);

int dictionary_insert(
    Dictionary *dictionary,
    const char *word,
    const char *definition,
    const char *part_of_speech,
    const char *example_sentence
);

DictionaryEntry *dictionary_search(
    const Dictionary *dictionary,
    const char *word
);

int dictionary_delete(Dictionary *dictionary, const char *word);

void dictionary_free(Dictionary *dictionary);

int dictionary_load(Dictionary *dictionary, const char *filename);

int dictionary_validate_line(const char *line);

int dictionary_save(const Dictionary *dictionary, const char *filename);

#endif