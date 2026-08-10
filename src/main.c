#include <stdio.h>
#include "hash_table.h"

int main(void)
{
    const char *valid_line =
        "apple|A fruit|noun|I ate an apple.";

    const char *invalid_line =
        "apple|A fruit|noun";

    printf("Valid line: %s\n",
           dictionary_validate_line(valid_line) ? "PASS" : "FAIL");

    printf("Invalid line: %s\n",
           dictionary_validate_line(invalid_line) ? "FAIL" : "PASS");

    return 0;
}