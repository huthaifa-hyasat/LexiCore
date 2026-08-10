#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

size_t hash_function(const char *word, size_t bucket_count){
    unsigned long hash = 5381;
    int character;

    while ((character = *word++))
    {
        hash = ((hash << 5) + hash) + (unsigned char)character;
    }

    return hash % bucket_count;
}

Dictionary *dictionary_create(size_t bucket_count){
    Dictionary *dictionary = malloc(sizeof(Dictionary));

    if (dictionary == NULL)
    {
        return NULL;
    }

    dictionary->buckets = calloc(bucket_count, sizeof(DictionaryEntry *));

    if (dictionary->buckets == NULL)
    {
        free(dictionary);
        return NULL;
    }

    dictionary->bucket_count = bucket_count;
    dictionary->entry_count = 0;

    return dictionary;
}

int dictionary_insert(
    Dictionary *dictionary,
    const char *word,
    const char *definition,
    const char *part_of_speech,
    const char *example_sentence)
{
    if (dictionary == NULL || word == NULL || definition == NULL ||
        part_of_speech == NULL || example_sentence == NULL)
    {
        return 0;
    }

    size_t index = hash_function(word, dictionary->bucket_count);

    DictionaryEntry *current = dictionary->buckets[index];

while (current != NULL)
{
    if (strcmp(current->word, word) == 0)
    {
        return 0;
    }

    current = current->next;
}

    DictionaryEntry *entry = malloc(sizeof(DictionaryEntry));

    if (entry == NULL)
    {
        return 0;
    }

    entry->word = malloc(strlen(word) + 1);
    entry->definition = malloc(strlen(definition) + 1);
    entry->part_of_speech = malloc(strlen(part_of_speech) + 1);
    entry->example_sentence = malloc(strlen(example_sentence) + 1);

    if (entry->word == NULL ||
        entry->definition == NULL ||
        entry->part_of_speech == NULL ||
        entry->example_sentence == NULL)
    {
        free(entry->word);
        free(entry->definition);
        free(entry->part_of_speech);
        free(entry->example_sentence);
        free(entry);

        return 0;
    }

    strcpy(entry->word, word);
    strcpy(entry->definition, definition);
    strcpy(entry->part_of_speech, part_of_speech);
    strcpy(entry->example_sentence, example_sentence);

    entry->next = dictionary->buckets[index];
    dictionary->buckets[index] = entry;

    dictionary->entry_count++;

    return 1;
}

DictionaryEntry *dictionary_search(
    const Dictionary *dictionary,
    const char *word)
{
    if (dictionary == NULL || word == NULL)
    {
        return NULL;
    }

    size_t index = hash_function(word, dictionary->bucket_count);

    DictionaryEntry *current = dictionary->buckets[index];

    while (current != NULL)
    {
        if (strcmp(current->word, word) == 0)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

int dictionary_delete(Dictionary *dictionary, const char *word)
{
    if (dictionary == NULL || word == NULL)
    {
        return 0;
    }

    size_t index = hash_function(word, dictionary->bucket_count);

    DictionaryEntry *current = dictionary->buckets[index];
    DictionaryEntry *previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->word, word) == 0)
        {
            if (previous == NULL)
            {
                dictionary->buckets[index] = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            free(current->word);
            free(current->definition);
            free(current->part_of_speech);
            free(current->example_sentence);
            free(current);

            dictionary->entry_count--;

            return 1;
        }

        previous = current;
        current = current->next;
    }

    return 0;
}

void dictionary_free(Dictionary *dictionary)
{
    
    if (dictionary == NULL)
    {
        return;
    }

    for (size_t i = 0; i < dictionary->bucket_count; i++)
    {
        DictionaryEntry *current = dictionary->buckets[i];

        while (current != NULL)
        {
            DictionaryEntry *next = current->next;

            free(current->word);
            free(current->definition);
            free(current->part_of_speech);
            free(current->example_sentence);
            free(current);

            current = next;
        }
    }

    free(dictionary->buckets);
    free(dictionary);
}

int dictionary_load(Dictionary *dictionary, const char *filename)
{
    if (dictionary == NULL || filename == NULL)
    {
        return 0;
    }

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return 0;
    }

    char line[1024];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';

        char *word = strtok(line, "|");
        char *definition = strtok(NULL, "|");
        char *part_of_speech = strtok(NULL, "|");
        char *example_sentence = strtok(NULL, "|");

        if (word == NULL ||
            definition == NULL ||
            part_of_speech == NULL ||
            example_sentence == NULL)
        {
            continue;
        }

        dictionary_insert(
            dictionary,
            word,
            definition,
            part_of_speech,
            example_sentence
        );
    }

    fclose(file);

    return 1;
}

int dictionary_validate_line(const char *line)
{
    if (line == NULL || line[0] == '\0')
    {
        return 0;
    }

    char copy[1024];

    strncpy(copy, line, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';

    char *word = strtok(copy, "|");
    char *definition = strtok(NULL, "|");
    char *part_of_speech = strtok(NULL, "|");
    char *example_sentence = strtok(NULL, "|");

    if (word == NULL ||
        definition == NULL ||
        part_of_speech == NULL ||
        example_sentence == NULL)
    {
        return 0;
    }

    if (word[0] == '\0' ||
        definition[0] == '\0' ||
        part_of_speech[0] == '\0' ||
        example_sentence[0] == '\0')
    {
        return 0;
    }

    return 1;
}

int dictionary_save(const Dictionary *dictionary, const char *filename)
{
    if (dictionary == NULL || filename == NULL)
    {
        return 0;
    }

    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        return 0;
    }

    for (size_t i = 0; i < dictionary->bucket_count; i++)
    {
        DictionaryEntry *current = dictionary->buckets[i];

        while (current != NULL)
        {
            fprintf(
                file,
                "%s|%s|%s|%s\n",
                current->word,
                current->definition,
                current->part_of_speech,
                current->example_sentence
            );

            current = current->next;
        }
    }

    fclose(file);

    return 1;
}