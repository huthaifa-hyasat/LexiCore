# LexiCore

LexiCore is a dynamic English dictionary implemented in C using a Hash Table.

## Features

- Add a new word
- Search for a word
- Case-insensitive word search
- Delete a word
- Edit an existing word
- Prefix search
- Display words alphabetically
- Dictionary statistics
- Load dictionary from a file
- Save dictionary to a file
- Validation of loaded data
- Duplicate-word detection
- Memory cleanup

## Data Structure

LexiCore uses a Hash Table with separate chaining.

Each dictionary entry contains:

- Word
- Definition
- Part of speech
- Example sentence

## Project Structure

```text
LexiCore/
├── data/
│   ├── dictionary.txt
│   ├── saved_dictionary.txt
│   ├── test_dictionary.txt
│   └── test_save.txt
├── include/
│   └── hash_table.h
├── src/
│   ├── main.c
│   ├── hash_table.c
│   ├── dictionary.c
│   └── utils.c
├── tests/
│   └── test_cases.md
├── .gitignore
├── CONTRIBUTIONS.md
├── Makefile
└── README.md
Conflict test - master branch