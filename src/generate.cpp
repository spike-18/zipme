#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_FILES 30
#define CHARS_PER_FILE 100000
#define MIN_ASCII 32
#define MAX_ASCII 127

// Function to generate a random printable ASCII character
char generateRandomChar() {
    return (char)(rand() % (MAX_ASCII - MIN_ASCII + 1) + MIN_ASCII);
}

int main() {
    // Seed for random number generation
    srand(time(NULL));

    // Create 100 files
    for (int fileNum = 1; fileNum <= NUM_FILES; ++fileNum) {
        char filename[20];
        char path[40];
        sprintf(filename, "file%d.txt", fileNum);
        sprintf(path, "../data/train/%s", filename);
        FILE *file = fopen(path, "w+");

        // Check if file opened successfully
        if (file == NULL) {
            printf("Error creating file %s\n", filename);
            return 1;
        }

        // Write 100,000 random printable ASCII characters to the file
        for (int i = 0; i < CHARS_PER_FILE; ++i) {
            char randomChar = generateRandomChar();
            fprintf(file, "%c", randomChar);
        }

        // Close the file
        fclose(file);
        printf("File %s created successfully\n", filename);
    }

    for (int testFileNum = 1; testFileNum <= 3; ++testFileNum) {
        char filename[20];
        char path[40];
        sprintf(filename, "test%d.txt", testFileNum);
        sprintf(path, "../data/test/%s", filename);
        FILE *file = fopen(path, "w+");

        // Check if file opened successfully
        if (file == NULL) {
            printf("Error creating test file %s\n", filename);
            return 1;
        }

        // Write 100,000 random printable ASCII characters to the test file
        for (int i = 0; i < CHARS_PER_FILE; ++i) {
            char randomChar = generateRandomChar();
            fprintf(file, "%c", randomChar);
        }

        // Close the file
        fclose(file);
        printf("Test file %s created successfully\n", filename);
    }


    return 0;
}