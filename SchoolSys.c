#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define NUM_GRADES 10
#define NUM_LEVELS 12
#define NUM_CLASSES 10

struct Student {
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    char phone_number[11];
    int grades[NUM_GRADES];
    struct Student* next;
};

// Function to read student data from the file
int readStudentData(FILE* file, struct Student* new_student, int* level, int* class) {
    if (fscanf(file, "%s %s %s %d %d", new_student->first_name, new_student->last_name, new_student->phone_number, level, class) != 5) {
        return 0; // Failed to read all required fields
    }
    for (int i = 0; i < NUM_GRADES; i++) {
        if (fscanf(file, "%d", &new_student->grades[i]) != 1) {
            return 0; // Failed to read grade
        }
    }
    return 1; // Successfully read student data
}

// Function to add a new student to the linked list
void addStudent(struct Student* School[NUM_LEVELS][NUM_CLASSES], struct Student* new_student, int level, int class) {
    // Check if the level and class number are within valid ranges
    if (level < 1 || level > NUM_LEVELS || class < 1 || class > NUM_CLASSES) {
        printf("Invalid level or class number for student %s %s. Skipping...\n", new_student->first_name, new_student->last_name);
        free(new_student); // Free the memory allocated for the student
        return;
    }

    // Truncate the first name, last name, and phone number if they exceed the maximum allowed length
    new_student->first_name[MAX_NAME_LEN - 1] = '\0'; // Ensure null-terminated string
    new_student->last_name[MAX_NAME_LEN - 1] = '\0'; // Ensure null-terminated string
    new_student->phone_number[10] = '\0'; // Ensure null-terminated string

    // Add the student to the linked list for the corresponding level and class
    new_student->next = School[level - 1][class - 1];
    School[level - 1][class - 1] = new_student;
}

// Function to delete a student from the linked list
void deleteStudent(struct Student* School[NUM_LEVELS][NUM_CLASSES], int level, int class, const char* first_name, const char* last_name) {
    // Find the student in the linked list and delete it
    struct Student* current_student = School[level - 1][class - 1];
    struct Student* prev_student = NULL;
    while (current_student != NULL) {
        if (strcmp(current_student->first_name, first_name) == 0 && strcmp(current_student->last_name, last_name) == 0) {
            if (prev_student == NULL) {
                School[level - 1][class - 1] = current_student->next;
            } else {
                prev_student->next = current_student->next;
            }
            free(current_student);
            return;
        }
        prev_student = current_student;
        current_student = current_student->next;
    }
}

// Function to print student information
void printStudentInfo(struct Student* School[NUM_LEVELS][NUM_CLASSES]) {
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class = 0; class < NUM_CLASSES; class++) {
            struct Student* current_student = School[level][class];
            while (current_student != NULL) {
                printf("First Name: %s\n", current_student->first_name);
                printf("Last Name: %s\n", current_student->last_name);
                printf("Grades: ");
                for (int i = 0; i < NUM_GRADES; i++) {
                    printf("%d ", current_student->grades[i]);
                }
                printf("\n");
                printf("Phone Number: %s\n", current_student->phone_number);
                printf("\n");

                current_student = current_student->next;
            }
        }
    }
}

int main() {
    // Create a 2D array of pointers to Student for each level and class
    struct Student* School[NUM_LEVELS][NUM_CLASSES] = { NULL };

    FILE* file = fopen("/home/avigayil/CheckPointEx/students.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char line[100];
    // Reads a single line from the file into the character array line
    while (fgets(line, sizeof(line), file) != NULL) {
        // Allocate memory for a new student
        struct Student* new_student = (struct Student*)malloc(sizeof(struct Student));
        // Check if memory allocation was successful
        if (new_student == NULL) {
            printf("Memory allocation error\n");
            return 1;
        }

        // Parse the line to extract student information and add the student to the data structure
        int level, class;
        if (readStudentData(file, new_student, &level, &class)) {
            addStudent(School, new_student, level, class);
        } else {
            printf("Error parsing student data. Skipping...\n");
            free(new_student); // Free the memory allocated for the student
        }
    }

    fclose(file);

    // Print student information
    printStudentInfo(School);

    // Free allocated memory for students
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class = 0; class < NUM_CLASSES; class++) {
            struct Student* current_student = School[level][class];
            while (current_student != NULL) {
                struct Student* next_student = current_student->next;
                free(current_student);
                current_student = next_student;
            }
        }
    }

    return 0;
}

