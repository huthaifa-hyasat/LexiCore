#include <stdio.h>
#include <string.h>
#include "hash_table.h"

#define MAX_INPUT 1024

void read_line(char *buffer, size_t size)
{
    if (fgets(buffer, size, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void print_menu(void)
{
    printf("\n========================================\n");
    printf("        LEXICORE ENGLISH DICTIONARY\n");
    printf("========================================\n");
    printf("1. Add a new word\n");
    printf("2. Search for an exact word\n");
    printf("3. Delete a word\n");
    printf("4. Edit an existing word\n");
    printf("5. Search by prefix\n");
    printf("6. Display words alphabetically\n");
    printf("7. Display dictionary statistics\n");
    printf("8. Load dictionary from file\n");
    printf("9. Save dictionary to file\n");
    printf("0. Exit\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

void add_word(Dictionary *dictionary)
{
    char word[MAX_INPUT];
    char definition[MAX_INPUT];
    char part_of_speech[MAX_INPUT];
    char example_sentence[MAX_INPUT];

    printf("Enter word: ");
    read_line(word, sizeof(word));

    printf("Enter definition: ");
    read_line(definition, sizeof(definition));

    printf("Enter part of speech: ");
    read_line(part_of_speech, sizeof(part_of_speech));

    printf("Enter example sentence: ");
    read_line(example_sentence, sizeof(example_sentence));

    if (word[0] == '\0' ||
        definition[0] == '\0' ||
        part_of_speech[0] == '\0' ||
        example_sentence[0] == '\0')
    {
        printf("Invalid input. Fields cannot be empty.\n");
        return;
    }

    if (dictionary_insert(
            dictionary,
            word,
            definition,
            part_of_speech,
            example_sentence))
    {
        printf("Word added successfully!\n");
    }
    else
    {
        printf("Failed to add word. It may already exist.\n");
    }
}

void search_word(Dictionary *dictionary)
{
    char word[MAX_INPUT];

    printf("Enter word to search: ");
    read_line(word, sizeof(word));

    if (word[0] == '\0')
    {
        printf("Word cannot be empty.\n");
        return;
    }

    DictionaryEntry *entry =
        dictionary_search(dictionary, word);

    if (entry == NULL)
    {
        printf("Word not found.\n");
        return;
    }

    printf("\nWord: %s\n", entry->word);
    printf("Definition: %s\n", entry->definition);
    printf("Part of speech: %s\n", entry->part_of_speech);
    printf("Example: %s\n", entry->example_sentence);
}

void delete_word(Dictionary *dictionary)
{
    char word[MAX_INPUT];
    char confirmation[MAX_INPUT];

    printf("Enter word to delete: ");
    read_line(word, sizeof(word));

    if (word[0] == '\0')
    {
        printf("Word cannot be empty.\n");
        return;
    }

    DictionaryEntry *entry =
        dictionary_search(dictionary, word);

    if (entry == NULL)
    {
        printf("Word not found.\n");
        return;
    }

    printf("Delete the word \"%s\"? (y/n): ", entry->word);
    read_line(confirmation, sizeof(confirmation));

    if (confirmation[0] != 'y' &&
        confirmation[0] != 'Y')
    {
        printf("Deletion cancelled.\n");
        return;
    }

    if (dictionary_delete(dictionary, word))
    {
        printf("Word deleted successfully!\n");
    }
    else
    {
        printf("Failed to delete word.\n");
    }
}

void edit_word(Dictionary *dictionary)
{
    char word[MAX_INPUT];
    char definition[MAX_INPUT];
    char part_of_speech[MAX_INPUT];
    char example_sentence[MAX_INPUT];

    printf("Enter word to edit: ");
    read_line(word, sizeof(word));

    if (word[0] == '\0')
    {
        printf("Word cannot be empty.\n");
        return;
    }

    if (dictionary_search(dictionary, word) == NULL)
    {
        printf("Word not found.\n");
        return;
    }

    printf("Enter new definition: ");
    read_line(definition, sizeof(definition));

    printf("Enter new part of speech: ");
    read_line(part_of_speech, sizeof(part_of_speech));

    printf("Enter new example sentence: ");
    read_line(example_sentence, sizeof(example_sentence));

    if (definition[0] == '\0' ||
        part_of_speech[0] == '\0' ||
        example_sentence[0] == '\0')
    {
        printf("Invalid input. Fields cannot be empty.\n");
        return;
    }

    if (dictionary_edit(
            dictionary,
            word,
            definition,
            part_of_speech,
            example_sentence))
    {
        printf("Edit successful!\n");
    }
    else
    {
        printf("Edit failed.\n");
    }
}

void prefix_search(Dictionary *dictionary)
{
    char prefix[MAX_INPUT];

    printf("Enter prefix: ");
    read_line(prefix, sizeof(prefix));

    if (prefix[0] == '\0')
    {
        printf("Prefix cannot be empty.\n");
        return;
    }

    printf("\nPrefix search results for '%s':\n", prefix);

    dictionary_prefix_search(dictionary, prefix);
}

int main(void)
{
    Dictionary *dictionary =
        dictionary_create(DEFAULT_BUCKET_COUNT);

    if (dictionary == NULL)
    {
        printf("Failed to create dictionary.\n");
        return 1;
    }

    int choice;
    char filename[MAX_INPUT];

    do
    {
        print_menu();

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");

            while (getchar() != '\n')
            {
            }

            continue;
        }

        while (getchar() != '\n')
        {
        }

        switch (choice)
        {
            case 1:
                add_word(dictionary);
                break;

            case 2:
                search_word(dictionary);
                break;

            case 3:
                delete_word(dictionary);
                break;

            case 4:
                edit_word(dictionary);
                break;

            case 5:
                prefix_search(dictionary);
                break;

            case 6:
                printf("\n--- Alphabetical Dictionary ---\n");
                dictionary_display_alphabetical(dictionary);
                break;

            case 7:
                printf("\n--- Dictionary Statistics ---\n");
                dictionary_statistics(dictionary);
                break;

            case 8:
                printf("Enter filename: ");
                read_line(filename, sizeof(filename));

                if (dictionary_load(dictionary, filename))
                {
                    printf("Dictionary loaded successfully!\n");
                }
                else
                {
                    printf("Failed to load dictionary.\n");
                }

                break;

            case 9:
                printf("Enter filename: ");
                read_line(filename, sizeof(filename));

                if (dictionary_save(dictionary, filename))
                {
                    printf("Dictionary saved successfully!\n");
                }
                else
                {
                    printf("Failed to save dictionary.\n");
                }

                break;

            case 0:
            {
                char save_choice;

                printf("Save changes before exiting? (y/n): ");
                scanf(" %c", &save_choice);

                if (save_choice == 'y' || save_choice == 'Y')
               {
                 printf("Enter filename: ");
                 read_line(filename, sizeof(filename));

                 if (dictionary_save(dictionary, filename))
                 {
                  printf("Dictionary saved successfully!\n");
                 } 
                 else
                 {
                   printf("Failed to save dictionary.\n");
                 }
                 }
                 else if (save_choice == 'n' || save_choice == 'N')
                 {
                  printf("Exiting without saving.\n");
                 }
                 else
                 {
                  printf("Invalid choice. Exiting without saving.\n");
                 }

                 printf("Exiting LexiCore...\n");
                 break;
            }

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 0);

    dictionary_free(dictionary);

    return 0;
}