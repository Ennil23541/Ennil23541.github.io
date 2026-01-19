#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

  while (1) {
    char *word = NULL;
    size_t len = 0;

    printf("Please enter a line:\n");

    ssize_t number_of_char = getline(&word, &len, stdin);

    if (number_of_char != -1) {
      char *full_string = word;
      char *delimit = " \t\n\r";
      char *token = NULL;
      char *saveptr = NULL;

      while (token = strtok_r(full_string, delimit, &saveptr)) {
        printf("%s\n", token);
        full_string = NULL;
      }

      free(word);
    }
  }
}
