# LexiCore Test Cases

| # | Test | Input / Action | Expected Result | Actual Result | Status |
|---|------|----------------|-----------------|---------------|--------|
| 1 | Load valid file | Load dictionary.txt | Valid entries are loaded | Passed | PASS |
| 2 | Load empty file | Load an empty file | No entries loaded, program does not crash | Passed | PASS |
| 3 | Missing file | Load a file that does not exist | Error message displayed | Passed | PASS |
| 4 | Invalid line | Load a line with missing fields | Invalid line ignored with warning | Passed | PASS |
| 5 | Duplicate word | Load an existing word twice | Duplicate is ignored | Passed | PASS |
| 6 | Search existing word | Search for apple | Full entry is displayed | Passed | PASS |
| 7 | Search missing word | Search for xyzabc | Word not found message | Passed | PASS |
| 8 | Case-insensitive search | Search for APPLE | apple is found | Passed | PASS |
| 9 | Add new word | Add a new dictionary entry | Entry is added successfully | Passed | PASS |
| 10 | Duplicate insertion | Add an existing word | Duplicate is rejected | Passed | PASS |
| 11 | Edit word | Edit an existing word | Entry information is updated | Passed | PASS |
| 12 | Delete word | Delete an existing word | Entry is removed | Passed | PASS |
| 13 | Delete missing word | Delete xyzabc | Delete operation is rejected | Passed | PASS |
| 14 | Prefix search | Search prefix app | All matching words are displayed | Passed | PASS |
| 15 | Alphabetical display | Display dictionary alphabetically | Entries appear in alphabetical order | Passed | PASS |
| 16 | Statistics | Display dictionary statistics | Correct table and word statistics displayed | Passed | PASS |
| 17 | Save dictionary | Save to test_save.txt | Dictionary is written to file | Passed | PASS |
| 18 | Reload dictionary | Load saved dictionary | Saved entries are restored | Passed | PASS |
| 19 | Invalid menu input | Enter a non-numeric menu choice | Program continues without crashing | Passed | PASS |
| 20 | Memory cleanup | Exit the program | All allocated memory is released | Passed | PASS |