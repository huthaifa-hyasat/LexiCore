#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

size_t hash_function(const char *word, size_t bucket_count)
{
    unsigned long hash = 5381;
    int character;

    if (word == NULL || bucket_count == 0)
    {
        return 0;
    }

    while ((character = *word++))
    {
        character = tolower((unsigned char)character);
        hash = ((hash << 5) + hash) + character;
    }

    return hash % bucket_count;
}

static int strings_equal_ignore_case(
    const char *first,
    const char *second)
{
    if (first == NULL || second == NULL)
    {
        return 0;
    }

    while (*first != '\0' && *second != '\0')
    {
        if (tolower((unsigned char)*first) !=
            tolower((unsigned char)*second))
        {
            return 0;
        }

        first++;
        second++;
    }

    return *first == '\0' && *second == '\0';
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
    if (dictionary_search(dictionary, word) != NULL)
    {
        return 0;
    }


    size_t index = hash_function(word, dictionary->bucket_count);

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

       if (strings_equal_ignore_case(current->word, word))
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

static char *trim_whitespace(char *text)
{
    if (text == NULL)
    {
        return NULL;
    }

    while (isspace((unsigned char)*text))
    {
        text++;
    }

    if (*text == '\0')
    {
        return text;
    }

    char *end = text + strlen(text) - 1;

    while (end > text &&
           isspace((unsigned char)*end))
    {
        end--;
    }

    *(end + 1) = '\0';

    return text;
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
        fprintf(stderr, "Error: Cannot open file '%s'.\n", filename);
        return 0;
    }

    char line[4096];

    size_t line_number = 0;
    size_t valid_entries = 0;
    size_t invalid_lines = 0;
    size_t duplicate_words = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_number++;

        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '\0')
        {
            continue;
        }

        char *word = strtok(line, "|");
        char *part_of_speech = strtok(NULL, "|");
        char *definition = strtok(NULL, "|");
        char *example_sentence = strtok(NULL, "|");

        word = trim_whitespace(word);
        part_of_speech = trim_whitespace(part_of_speech);
        definition = trim_whitespace(definition);
        example_sentence = trim_whitespace(example_sentence);

        if (word == NULL ||
            part_of_speech == NULL ||
            definition == NULL ||
            example_sentence == NULL ||
            word[0] == '\0' ||
            part_of_speech[0] == '\0' ||
            definition[0] == '\0' ||
            example_sentence[0] == '\0')
        {
            fprintf(
                stderr,
                "Warning: Invalid entry ignored at line %zu.\n",
                line_number
            );

            invalid_lines++;
            continue;
        }

        if (dictionary_search(dictionary, word) != NULL)
        {
            fprintf(
                stderr,
                "Warning: Duplicate word ignored at line %zu: %s\n",
                line_number,
                word
            );

            duplicate_words++;
            continue;
        }

        if (dictionary_insert(
                dictionary,
                word,
                definition,
                part_of_speech,
                example_sentence))
        {
            valid_entries++;
        }
        else
        {
            fprintf(
                stderr,
                "Warning: Failed to insert entry at line %zu.\n",
                line_number
            );

            invalid_lines++;
        }
    }

    fclose(file);

    printf("\nDictionary loaded successfully.\n");
    printf("Valid entries loaded: %zu\n", valid_entries);
    printf("Invalid lines ignored: %zu\n", invalid_lines);
    printf("Duplicate words ignored: %zu\n", duplicate_words);

    return 1;
}

int dictionary_save(const Dictionary *dictionary, const char *filename)
{
    if (dictionary == NULL || filename == NULL)
    {
        return 0;
    }

    FILE *existing_file = fopen(filename, "r");

    if (existing_file != NULL)
    {
        fclose(existing_file);

        printf("Warning: File '%s' already exists.\n", filename);
        printf("Do you want to overwrite it? (y/n): ");

        int answer = getchar();

        int character;
        while ((character = getchar()) != '\n' &&
               character != EOF)
        {
        }

        if (answer != 'y' && answer != 'Y')
        {
            printf("Save cancelled.\n");
            return 0;
        }
    }

    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        fprintf(
            stderr,
            "Error: Cannot open file '%s' for writing.\n",
            filename
        );

        return 0;
    }

    for (size_t i = 0;
         i < dictionary->bucket_count;
         i++)
    {
        DictionaryEntry *current =
            dictionary->buckets[i];

        while (current != NULL)
        {
            if (fprintf(
                    file,
                    "%s|%s|%s|%s\n",
                    current->word,
                    current->part_of_speech,
                    current->definition,
                    current->example_sentence
                ) < 0)
            {
                fprintf(
                    stderr,
                    "Error: Failed while writing to '%s'.\n",
                    filename
                );

                fclose(file);
                return 0;
            }

            current = current->next;
        }
    }

    if (fclose(file) != 0)
    {
        fprintf(
            stderr,
            "Error: Failed to close file '%s'.\n",
            filename
        );

        return 0;
    }

    return 1;
}

size_t dictionary_prefix_search(
    const Dictionary *dictionary,
    const char *prefix)
{
    if (dictionary == NULL || prefix == NULL || prefix[0] == '\0')
    {
        return 0;
    }

    size_t prefix_length = strlen(prefix);
    size_t match_count = 0;

    for (size_t i = 0; i < dictionary->bucket_count; i++)
    {
        DictionaryEntry *current = dictionary->buckets[i];

        while (current != NULL)
        {
            size_t j = 0;

            while (j < prefix_length &&
                   current->word[j] != '\0' &&
                   tolower((unsigned char)current->word[j]) ==
                   tolower((unsigned char)prefix[j]))
            {
                j++;
            }

            if (j == prefix_length)
            {
                match_count++;

                printf(
                    "%zu. %s | %s | %s | %s\n",
                    match_count,
                    current->word,
                    current->definition,
                    current->part_of_speech,
                    current->example_sentence
                );
            }

            current = current->next;
        }
    }

    printf("\n%zu matching words found.\n", match_count);

    return match_count;
}
static int compare_entries(
    const void *a,
    const void *b)
{
    const DictionaryEntry *first =
        *(const DictionaryEntry **)a;

    const DictionaryEntry *second =
        *(const DictionaryEntry **)b;

    return strcmp(first->word, second->word);
}
void dictionary_display_alphabetical(
    const Dictionary *dictionary)
{
    if (dictionary == NULL || dictionary->entry_count == 0)
    {
        return;
    }

    DictionaryEntry **entries =
        malloc(dictionary->entry_count *
               sizeof(DictionaryEntry *));

    if (entries == NULL)
    {
        return;
    }

    size_t count = 0;

    for (size_t i = 0;
         i < dictionary->bucket_count;
         i++)
    {
        DictionaryEntry *current =
            dictionary->buckets[i];

        while (current != NULL)
        {
            entries[count++] = current;
            current = current->next;
        }
    }

    qsort(
        entries,
        count,
        sizeof(DictionaryEntry *),
        compare_entries
    );

    for (size_t i = 0; i < count; i++)
    {
        printf(
            "%s | %s | %s | %s\n",
            entries[i]->word,
            entries[i]->definition,
            entries[i]->part_of_speech,
            entries[i]->example_sentence
        );
    }

    free(entries);
}
void dictionary_statistics(const Dictionary *dictionary)
{
    if (dictionary == NULL)
    {
        return;
    }

    size_t used_buckets = 0;
    size_t empty_buckets = 0;
    size_t longest_chain = 0;
    size_t total_used_entries = 0;

    size_t nouns = 0;
    size_t verbs = 0;
    size_t adjectives = 0;
    size_t adverbs = 0;
    size_t other = 0;

    for (size_t i = 0;
         i < dictionary->bucket_count;
         i++)
    {
        size_t bucket_size = 0;

        DictionaryEntry *current =
            dictionary->buckets[i];

        while (current != NULL)
        {
            bucket_size++;

            if (current->part_of_speech != NULL)
            {
                if (strcmp(current->part_of_speech, "noun") == 0)
                {
                    nouns++;
                }
                else if (strcmp(current->part_of_speech, "verb") == 0)
                {
                    verbs++;
                }
                else if (strcmp(current->part_of_speech, "adjective") == 0)
                {
                    adjectives++;
                }
                else if (strcmp(current->part_of_speech, "adverb") == 0)
                {
                    adverbs++;
                }
                else
                {
                    other++;
                }
            }
            else
            {
                other++;
            }

            current = current->next;
        }

        if (bucket_size > 0)
        {
            used_buckets++;
            total_used_entries += bucket_size;

            if (bucket_size > longest_chain)
            {
                longest_chain = bucket_size;
            }
        }
        else
        {
            empty_buckets++;
        }
    }

    double load_factor = 0.0;

    if (dictionary->bucket_count > 0)
    {
        load_factor =
            (double)dictionary->entry_count /
            dictionary->bucket_count;
    }

    double average_used_chain = 0.0;

    if (used_buckets > 0)
    {
        average_used_chain =
            (double)total_used_entries /
            used_buckets;
    }

    printf("\n************* Dictionary Statistics *************\n");

    printf("Total entries:                 %zu\n",
           dictionary->entry_count);

    printf("Total buckets:                %zu\n",
           dictionary->bucket_count);

    printf("Used buckets:                 %zu\n",
           used_buckets);

    printf("Empty buckets:               %zu\n",
           empty_buckets);

    printf("Load factor:                 %.2f\n",
           load_factor);

    printf("Longest chain:                %zu\n",
           longest_chain);

    printf("Average used-chain length:   %.2f\n",
           average_used_chain);

    printf("\n--- Parts of Speech ---\n");

    printf("Nouns:                       %zu\n", nouns);
    printf("Verbs:                       %zu\n", verbs);
    printf("Adjectives:                  %zu\n", adjectives);
    printf("Adverbs:                     %zu\n", adverbs);
    printf("Other:                       %zu\n", other);

    printf("*******************************************\n");
}

int dictionary_edit(
    Dictionary *dictionary,
    const char *word,
    const char *definition,
    const char *part_of_speech,
    const char *example_sentence)
{
    if (dictionary == NULL ||
        word == NULL ||
        definition == NULL ||
        part_of_speech == NULL ||
        example_sentence == NULL)
    {
        return 0;
    }

    DictionaryEntry *entry =
        dictionary_search(dictionary, word);

    if (entry == NULL)
    {
        return 0;
    }

    char *new_definition =
        malloc(strlen(definition) + 1);

    char *new_part_of_speech =
        malloc(strlen(part_of_speech) + 1);

    char *new_example_sentence =
        malloc(strlen(example_sentence) + 1);

    if (new_definition == NULL ||
        new_part_of_speech == NULL ||
        new_example_sentence == NULL)
    {
        free(new_definition);
        free(new_part_of_speech);
        free(new_example_sentence);

        return 0;
    }

    strcpy(new_definition, definition);
    strcpy(new_part_of_speech, part_of_speech);
    strcpy(new_example_sentence, example_sentence);

    free(entry->definition);
    free(entry->part_of_speech);
    free(entry->example_sentence);

    entry->definition = new_definition;
    entry->part_of_speech = new_part_of_speech;
    entry->example_sentence = new_example_sentence;

    return 1;
}