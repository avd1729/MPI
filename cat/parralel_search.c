#include <stdio.h>
#include <string.h>
#include <omp.h>

#define MAX_TEXT_LENGTH 1000
#define MAX_WORD_LENGTH 100

int parallel_word_search(char *text, char *word)
{
    int text_length = strlen(text);
    int word_length = strlen(word);
    int found = 0;

#pragma omp parallel for
    for (int i = 0; i <= text_length - word_length; i++)
    {
        int j;
        for (j = 0; j < word_length; j++)
        {
            if (text[i + j] != word[j])
            {
                break;
            }
        }

        if (j == word_length)
        {
#pragma omp critical
            found = 1;
        }
    }

    return found;
}

int main()
{
    char word[MAX_WORD_LENGTH];
    char *file_names[] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt"}; // List of file names

    printf("Enter the word to search: ");
    scanf("%s", word);

    printf("Select a file to search from:\n");
    for (int i = 0; i < sizeof(file_names) / sizeof(file_names[0]); i++)
    {
        printf("%d. %s\n", i + 1, file_names[i]);
    }

    int selected_file;
    printf("Enter the number of the file you want to search in: ");
    scanf("%d", &selected_file);

    if (selected_file < 1 || selected_file > sizeof(file_names) / sizeof(file_names[0]))
    {
        printf("Invalid file number.\n");
        return 1;
    }

    FILE *file = fopen(file_names[selected_file - 1], "r");
    if (file == NULL)
    {
        printf("Failed to open the selected text file.\n");
        return 1;
    }

    char text[MAX_TEXT_LENGTH];
    int found = 0;

    double start_time = omp_get_wtime();

    while (fgets(text, sizeof(text), file) != NULL)
    {
        text[strlen(text) - 1] = '\0';
        found = parallel_word_search(text, word);
        if (found)
        {
            break;
        }
    }

    double end_time = omp_get_wtime();

    fclose(file);

    if (found)
    {
        printf("Word found in the text.\n");
    }
    else
    {
        printf("Word not found in the text.\n");
    }

    printf("Execution time: %f seconds\n", end_time - start_time);

    return 0;
}
