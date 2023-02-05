//
// Created by Behnam Dezfouli
// CSEN, Santa Clara University
//
//
// This program offers two options to the user:
//
// -- Option 1: enables the user to copy a file to another file. 
// The user needs to enter the name of the source and destination files.
// -- Option 2: enables the user to measure the performance of file copy. 
// In addition to the name of source and destination files, the user needs to enter the maximum source file size as well as the step size to increment the source file size.
//
//
// double copier(FILE *s_file, FILE *d_file)
// Precondition: s_file is a pointer to a file opened in read mode, d_file is a file pointer opened in write mode
// Postcondition: Copies the contents of the file pointed to by s_file to the file pointed to by d_file
//


#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#define SIZE 1
#define DUM_CHAR "A"
  
double copier(FILE *s_file, FILE *d_file) {
    char buffer[SIZE];                                                  // This is a buffer to hold the data read from the source file
    int count = 0;                                                      // Counter to count the number of bytes copied from "[source file]" to "[destination file]"
    clock_t start, end;
    double cpu_time_used;

    start = clock();                                                    // Start the time measurement

    while (fread(buffer, SIZE, 1, s_file) == 1) {                       // Read from source file and write to destination file
        fwrite(buffer, SIZE, 1, d_file);                                // write to the destination file
        count++;                                                        // Increment the counter
    }

    end = clock();                                                      // End the time measurement
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;          // Calculate the time used to copy the file

    return cpu_time_used;
}

int main() {
    int option;
    
    printf("Please select an option: \n\n1. File copy\n2. File copy with performance measurement \n\nEnter your option: ");

    scanf("%d", &option);

    if (option == 1) {                                                  // File copy

        FILE *s_file, *d_file;
        char s_file_name[100], d_file_name[100];

        printf("Enter the name of the source file: ");
        scanf("%s", s_file_name);

        printf("Enter the name of the destination file: ");
        scanf("%s", d_file_name);

        s_file = fopen(s_file_name, "r");                               // Open the source file in read mode

        if (s_file == NULL) {                                           // Check if the source file exists 
            printf("Error opening the source file!");
            exit(1);
        }

        d_file = fopen(d_file_name, "w");                               // Open the destination file in write mode

        if (d_file == NULL) {                                           // Check if the destination file exists 
            printf("Error opening the destination file!");
            exit(1);
        }

        double cpu_time_used = copier(s_file, d_file);                  // Print the time (in seconds) used to copy the file

        printf("The file was copied successfully in %f seconds", cpu_time_used);

        fclose(s_file);
        fclose(d_file);

        return 0;

    } else if (option == 2) {                                           // File copy with performance measurement
        FILE *fpFile1, *fpFile2;                                        // File pointers
        char filename1[100], filename2[100];                            // File names
        int file_size_bytes, step_size_bytes;                           // Maximum size and step size

        double cpu_time_used = 0;                                       // Time used to copy the file
 
        printf("Enter the name of the source file: ");                  // Scan the first source file name
        scanf("%s", filename1);

        printf("Enter the name of the destination file: ");             // Scan the second destination file name
        scanf("%s", filename2);                     

        printf("Enter the maximum size of the source file: ");          // Scan the maximum size of the source file
        scanf("%d", &file_size_bytes);

        printf("Enter the step size: ");                                // Scan step size bytes
        scanf("%d", &step_size_bytes);

        int j;

        for (j = 0; j <= file_size_bytes; j += step_size_bytes) {       // Loop through the file size
            fpFile1 = fopen(filename1, "w");                            // Open the file in write mode

            int i;

            for (i = 1; i <= j; ++i) {                                  // Write j bytes to the file
                putc(*DUM_CHAR, fpFile1);                               // Write the character to the first file
            }

            fclose(fpFile1);

            fpFile1 = fopen(filename1, "r");
            fpFile2 = fopen(filename2, "w");

            cpu_time_used += copier(fpFile1, fpFile2);                  // Copy the file

            printf("The file was copied successfully in %f seconds!\n", cpu_time_used);

            fclose(fpFile1);
            fclose(fpFile2);
        }
 
        return 0;

    } else {
        printf("Invalid option!");
        return 0;
    }
    
    return 0;
}