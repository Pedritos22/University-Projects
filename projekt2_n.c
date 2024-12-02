#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_LINE 1024  

int dychotomic_search(char **arr, int size, const char *word) {
    int left = 0, right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(arr[mid], word);
        if (cmp == 0) {
            return mid; 
        } else if (cmp < 0) {
            left = mid + 1; 
        } else {
            right = mid - 1; 
        }
    }
    return -1; 
}

char **read_words(const char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Nie można otworzyć pliku");
        exit(1);
    }

    char **words = NULL;
    char buffer[MAX_LINE];
    *size = 0;

    while (fgets(buffer, MAX_LINE, file) != NULL) {
        int length = strlen(buffer); 
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';  
        }

        words = realloc(words, (*size + 1) * sizeof(char *));
        if (!words) {
            perror("Błąd alokacji pamięci dla tablicy słów");
            fclose(file);
            exit(1);
        }

        words[*size] = strdup(buffer);
        if (!words[*size]) {
            perror("Błąd alokacji pamięci dla słowa");
            fclose(file);
            exit(1);
        }

        (*size)++;
    }

    fclose(file);
    return words;
}

void result(const char *filename, const char *word, int position) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Nie można otworzyć pliku do zapisu");
        exit(1);
    }

    if (position != -1) {
        fprintf(file, "Słowo '%s' znajduje się na pozycji: %d\n", word, position + 1);
        printf("Słowo '%s' znajduje się na pozycji: %d\n", word, position + 1);
    } else {
        fprintf(file, "Słowo '%s' nie zostało znalezione w ciągu.\n", word);
        printf("Słowo '%s' nie zostało znalezione w ciągu.\n", word);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");  

    if (argc != 4) {
        fprintf(stderr, "Użycie: %s <plik_wejściowy> <szukane_słowo> <plik_wyjściowy>\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];
    const char *search_word = argv[2];
    const char *output_file = argv[3];

    int size;
    char **words = read_words(input_file, &size);  

    int position = dychotomic_search(words, size, search_word);

    result(output_file, search_word, position);

    for (int i = 0; i < size; i++) {
        free(words[i]);
    }
    free(words);

    return 0;
}
