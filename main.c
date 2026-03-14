#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define PASS_MARK 40.0f

typedef struct {
    char name[MAX_NAME_LEN];
    int rollNumber;
    float marks;
} Student;

typedef struct {
    Student *students;
    size_t count;
    size_t capacity;
} StudentDatabase;

/* Removes trailing newline from a string read by fgets. */
static void trimNewline(char *text) {
    size_t len = strlen(text);
    if (len > 0 && text[len - 1] == '\n') {
        text[len - 1] = '\0';
    }
}

/* Reads a non-empty line safely from standard input. */
static void readLine(const char *prompt, char *buffer, size_t size) {
    for (;;) {
        printf("%s", prompt);
        if (fgets(buffer, (int) size, stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        if (strchr(buffer, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
        }

        trimNewline(buffer);
        if (strlen(buffer) == 0) {
            printf("Input cannot be empty. Try again.\n");
            continue;
        }
        break;
    }
}

/* Reads and validates integer input. */
static int readInt(const char *prompt) {
    char input[128];
    char *endPtr = NULL;
    long value;

    for (;;) {
        readLine(prompt, input, sizeof(input));
        value = strtol(input, &endPtr, 10);
        if (endPtr == input || *endPtr != '\0') {
            printf("Invalid number. Please enter an integer.\n");
            continue;
        }
        return (int) value;
    }
}

/* Reads and validates floating-point input. */
static float readFloat(const char *prompt) {
    char input[128];
    char *endPtr = NULL;
    float value;

    for (;;) {
        readLine(prompt, input, sizeof(input));
        value = strtof(input, &endPtr);
        if (endPtr == input || *endPtr != '\0') {
            printf("Invalid number. Please enter a numeric value.\n");
            continue;
        }
        return value;
    }
}

/* Initializes the dynamic student database. */
static void initDatabase(StudentDatabase *db) {
    db->students = NULL;
    db->count = 0;
    db->capacity = 0;
}

/* Frees all dynamically allocated memory in the database. */
static void freeDatabase(StudentDatabase *db) {
    free(db->students);
    db->students = NULL;
    db->count = 0;
    db->capacity = 0;
}

/* Grows storage when the array is full. */
static int ensureCapacity(StudentDatabase *db) {
    if (db->count < db->capacity) {
        return 1;
    }

    size_t newCapacity = (db->capacity == 0) ? 4 : db->capacity * 2;
    Student *newBlock = realloc(db->students, newCapacity * sizeof(Student));
    if (newBlock == NULL) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    db->students = newBlock;
    db->capacity = newCapacity;
    return 1;
}

/* Returns array index for a roll number, or -1 if not found. */
static int findStudentIndexByRoll(const StudentDatabase *db, int rollNumber) {
    for (size_t i = 0; i < db->count; i++) {
        if (db->students[i].rollNumber == rollNumber) {
            return (int) i;
        }
    }
    return -1;
}

/* Converts marks to pass/fail result text. */
static const char *getResultText(float marks) {
    return (marks > PASS_MARK) ? "Pass" : "Fail";
}

/* Adds a new student record to the database. */
static void addStudent(StudentDatabase *db) {
    Student student;

    readLine("Enter student name: ", student.name, sizeof(student.name));
    student.rollNumber = readInt("Enter roll number: ");

    if (findStudentIndexByRoll(db, student.rollNumber) >= 0) {
        printf("A student with roll number %d already exists.\n", student.rollNumber);
        return;
    }

    student.marks = readFloat("Enter marks: ");

    if (!ensureCapacity(db)) {
        return;
    }

    db->students[db->count] = student;
    db->count++;

    printf("Student added successfully. Result: %s\n", getResultText(student.marks));
}

/* Updates an existing student record. */
static void modifyStudent(StudentDatabase *db) {
    int rollNumber = readInt("Enter roll number of the student to modify: ");
    int index = findStudentIndexByRoll(db, rollNumber);

    if (index < 0) {
        printf("Student with roll number %d not found.\n", rollNumber);
        return;
    }

    Student *student = &db->students[index];
    printf("Editing record for %s (Roll: %d)\n", student->name, student->rollNumber);

    readLine("Enter new name: ", student->name, sizeof(student->name));

    int newRollNumber = readInt("Enter new roll number: ");
    int duplicateIndex = findStudentIndexByRoll(db, newRollNumber);
    if (duplicateIndex >= 0 && duplicateIndex != index) {
        printf("Another student already uses roll number %d. Roll number unchanged.\n", newRollNumber);
    } else {
        student->rollNumber = newRollNumber;
    }

    student->marks = readFloat("Enter new marks: ");
    printf("Student modified successfully. Result: %s\n", getResultText(student->marks));
}

/* Deletes a student record by roll number. */
static void removeStudent(StudentDatabase *db) {
    int rollNumber = readInt("Enter roll number of the student to remove: ");
    int index = findStudentIndexByRoll(db, rollNumber);

    if (index < 0) {
        printf("Student with roll number %d not found.\n", rollNumber);
        return;
    }

    for (size_t i = (size_t) index; i + 1 < db->count; i++) {
        db->students[i] = db->students[i + 1];
    }
    db->count--;
    printf("Student removed successfully.\n");
}

/* Displays all student records in tabular format. */
static void displayStudents(const StudentDatabase *db) {
    if (db->count == 0) {
        printf("No student records available.\n");
        return;
    }

    printf("\n%-5s %-25s %-10s %-10s\n", "No.", "Name", "Roll", "Marks");
    printf("--------------------------------------------------------\n");
    for (size_t i = 0; i < db->count; i++) {
        printf("%-5zu %-25s %-10d %-10.2f (%s)\n",
               i + 1,
               db->students[i].name,
               db->students[i].rollNumber,
               db->students[i].marks,
               getResultText(db->students[i].marks));
    }
}

/* Finds and prints a student record by roll number. */
static void searchStudent(const StudentDatabase *db) {
    int rollNumber = readInt("Enter roll number to search: ");
    int index = findStudentIndexByRoll(db, rollNumber);

    if (index < 0) {
        printf("Student with roll number %d not found.\n", rollNumber);
        return;
    }

    const Student *student = &db->students[index];
    printf("Student found:\n");
    printf("Name: %s\n", student->name);
    printf("Roll Number: %d\n", student->rollNumber);
    printf("Marks: %.2f\n", student->marks);
    printf("Result: %s\n", getResultText(student->marks));
}

/* Computes and prints average marks for all students. */
static void displayAverageMarks(const StudentDatabase *db) {
    if (db->count == 0) {
        printf("No records found. Average marks cannot be calculated.\n");
        return;
    }

    float total = 0.0f;
    for (size_t i = 0; i < db->count; i++) {
        total += db->students[i].marks;
    }

    printf("Average Marks: %.2f\n", total / (float) db->count);
}

/* Comparator for sorting marks in ascending order. */
static int compareMarksAscending(const void *a, const void *b) {
    const Student *s1 = (const Student *) a;
    const Student *s2 = (const Student *) b;
    if (s1->marks < s2->marks) return -1;
    if (s1->marks > s2->marks) return 1;
    return 0;
}

/* Comparator for sorting marks in descending order. */
static int compareMarksDescending(const void *a, const void *b) {
    return compareMarksAscending(b, a);
}

/* Sorts student records by marks in chosen order. */
static void sortStudentsByMarks(StudentDatabase *db, int ascending) {
    if (db->count < 2) {
        printf("Not enough records to sort.\n");
        return;
    }

    qsort(
        db->students,
        db->count,
        sizeof(Student),
        ascending ? compareMarksAscending : compareMarksDescending
    );

    printf("Students sorted by marks in %s order.\n", ascending ? "ascending" : "descending");
}

/* Saves all student records to a text file. */
static void saveToFile(const StudentDatabase *db, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file for writing: %s\n", filename);
        return;
    }

    for (size_t i = 0; i < db->count; i++) {
        fprintf(file, "%d|%.2f|%s\n",
                db->students[i].rollNumber,
                db->students[i].marks,
                db->students[i].name);
    }

    fclose(file);
    printf("Records saved to %s successfully.\n", filename);
}

/* Loads student records from a text file into memory. */
static void loadFromFile(StudentDatabase *db, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file for reading: %s\n", filename);
        return;
    }

    db->count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
        trimNewline(line);

        char *rollToken = strtok(line, "|");
        char *marksToken = strtok(NULL, "|");
        char *nameToken = strtok(NULL, "");

        if (rollToken == NULL || marksToken == NULL || nameToken == NULL) {
            continue;
        }

        if (!ensureCapacity(db)) {
            fclose(file);
            return;
        }

        Student student;
        student.rollNumber = atoi(rollToken);
        student.marks = strtof(marksToken, NULL);
        strncpy(student.name, nameToken, sizeof(student.name) - 1);
        student.name[sizeof(student.name) - 1] = '\0';

        db->students[db->count] = student;
        db->count++;
    }

    fclose(file);
    printf("Records loaded from %s successfully.\n", filename);
}

/* Prints available menu options. */
static void displayMenu(void) {
    printf("\n===== Miva Student Record System =====\n");
    printf("1. Add student\n");
    printf("2. Modify student\n");
    printf("3. Remove student\n");
    printf("4. Display all students\n");
    printf("5. Search student by roll number\n");
    printf("6. Display average marks\n");
    printf("7. Sort students by marks (ascending)\n");
    printf("8. Sort students by marks (descending)\n");
    printf("9. Save records to file\n");
    printf("10. Load records from file\n");
    printf("0. Exit\n");
}

int main(void) {
    /* Program setup and greeting. */
    StudentDatabase db;
    initDatabase(&db);

    char userName[MAX_NAME_LEN];
    printf("Welcome to the Miva Student Record System.\n");
    readLine("Please enter your name: ", userName, sizeof(userName));
    printf("Hello, %s!\n", userName);

    const char *defaultFile = "students.txt";
    int choice;

    do {
        displayMenu();
        choice = readInt("Choose an option: ");

        switch (choice) {
            case 1:
                addStudent(&db);
                break;
            case 2:
                modifyStudent(&db);
                break;
            case 3:
                removeStudent(&db);
                break;
            case 4:
                displayStudents(&db);
                break;
            case 5:
                searchStudent(&db);
                break;
            case 6:
                displayAverageMarks(&db);
                break;
            case 7:
                sortStudentsByMarks(&db, 1);
                break;
            case 8:
                sortStudentsByMarks(&db, 0);
                break;
            case 9:
                saveToFile(&db, defaultFile);
                break;
            case 10:
                loadFromFile(&db, defaultFile);
                break;
            case 0:
                printf("Exiting the program. Goodbye, %s!\n", userName);
                break;
            default:
                printf("Invalid option. Please choose a valid menu item.\n");
        }
    } while (choice != 0);

    freeDatabase(&db);
    return 0;
}
