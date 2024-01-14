#include <stdio.h>
#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50
#define FILE_NAME "students.txt"

struct Student {
    char name[MAX_NAME_LENGTH];
    int rollNumber;
    float marks[3];
};

void addStudent();
void viewStudents();
void searchStudent();
void updateMarks();
void deleteStudent();
void calculateAverage();

int main() {
    int choice;

    do {
        printf("\nStudent Management System\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Marks\n");
        printf("5. Delete Student\n");
        printf("6. Calculate Average Marks\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateMarks();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                calculateAverage();
                break;
            case 7:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }
    } while (choice != 7);

    return 0;
}

void addStudent() {
    FILE *file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct Student student;

    printf("Enter student name: ");
    scanf("%s", student.name);

    printf("Enter student roll number: ");
    scanf("%d", &student.rollNumber);

    printf("Enter marks for three subjects:\n");
    for (int i = 0; i < 3; i++) {
        printf("Subject %d: ", i + 1);
        scanf("%f", &student.marks[i]);
    }

    fprintf(file, "%s %d %.2f %.2f %.2f\n", student.name, student.rollNumber,
            student.marks[0], student.marks[1], student.marks[2]);

    fclose(file);

    printf("Student added successfully.\n");
}

void viewStudents() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    struct Student student;

    printf("\n%-20s%-15s%-10s%-10s%-10s\n", "Name", "Roll Number", "Subject 1", "Subject 2", "Subject 3");
    printf("----------------------------------------------------------\n");

    while (fscanf(file, "%s %d %f %f %f", student.name, &student.rollNumber,
                  &student.marks[0], &student.marks[1], &student.marks[2]) != EOF) {
        printf("%-20s%-15d%-10.2f%-10.2f%-10.2f\n", student.name, student.rollNumber,
               student.marks[0], student.marks[1], student.marks[2]);
    }

    fclose(file);
}

void searchStudent() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    int rollNumber;
    printf("Enter the roll number to search: ");
    scanf("%d", &rollNumber);

    struct Student student;
    int found = 0;

    while (fscanf(file, "%s %d %f %f %f", student.name, &student.rollNumber,
                  &student.marks[0], &student.marks[1], &student.marks[2]) != EOF) {
        if (student.rollNumber == rollNumber) {
            printf("\n%-20s%-15s%-10s%-10s%-10s\n", "Name", "Roll Number", "Subject 1", "Subject 2", "Subject 3");
            printf("----------------------------------------------------------\n");
            printf("%-20s%-15d%-10.2f%-10.2f%-10.2f\n", student.name, student.rollNumber,
                   student.marks[0], student.marks[1], student.marks[2]);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student with roll number %d not found.\n", rollNumber);
    }

    fclose(file);
}

void updateMarks() {
    FILE *file = fopen(FILE_NAME, "r+");
    if (file == NULL) {
        printf("Error opening file for reading and writing.\n");
        return;
    }

    int rollNumber;
    printf("Enter the roll number of the student whose marks you want to update: ");
    scanf("%d", &rollNumber);

    struct Student student;
    int found = 0;
    long int position;

    while (fscanf(file, "%s %d %f %f %f", student.name, &student.rollNumber,
                  &student.marks[0], &student.marks[1], &student.marks[2]) != EOF) {
        if (student.rollNumber == rollNumber) {
            position = ftell(file) - sizeof(struct Student);
            found = 1;
            break;
        }
    }

    if (found) {
        printf("Enter new marks for three subjects:\n");
        for (int i = 0; i < 3; i++) {
            printf("Subject %d: ", i + 1);
            scanf("%f", &student.marks[i]);
        }

        fseek(file, position, SEEK_SET);
        fprintf(file, "%s %d %.2f %.2f %.2f", student.name, student.rollNumber,
                student.marks[0], student.marks[1], student.marks[2]);
        printf("Marks updated successfully.\n");
    } else {
        printf("Student with roll number %d not found.\n", rollNumber);
    }

    fclose(file);
}

void deleteStudent() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    int rollNumber;
    printf("Enter the roll number of the student to be deleted: ");
    scanf("%d", &rollNumber);

    struct Student student;
    int found = 0;

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%s %d %f %f %f", student.name, &student.rollNumber,
                  &student.marks[0], &student.marks[1], &student.marks[2]) != EOF) {
        if (student.rollNumber != rollNumber) {
            fprintf(tempFile, "%s %d %.2f %.2f %.2f\n", student.name, student.rollNumber,
                    student.marks[0], student.marks[1], student.marks[2]);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(FILE_NAME);
        rename("temp.txt", FILE_NAME);
        printf("Student deleted successfully.\n");
    } else {
        printf("Student with roll number %d not found.\n", rollNumber);
        remove("temp.txt"); // Delete the temporary file if student not found
    }
}

void calculateAverage() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    struct Student student;
    float totalMarks, averageMarks;
    int count = 0;

    printf("\n%-20s%-15s%-10s\n", "Name", "Roll Number", "Average Marks");
    printf("----------------------------------\n");

    while (fscanf(file, "%s %d %f %f %f", student.name, &student.rollNumber,
                  &student.marks[0], &student.marks[1], &student.marks[2]) != EOF) {
        totalMarks = student.marks[0] + student.marks[1] + student.marks[2];
        averageMarks = totalMarks / 3.0;
        printf("%-20s%-15d%-10.2f\n", student.name, student.rollNumber, averageMarks);
        count++;
    }

    if (count == 0) {
        printf("No students found.\n");
    }

    fclose(file);
}

