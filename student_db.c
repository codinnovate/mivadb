#include "student_db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

/* Ensure there is enough space to store another student. */
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

/* Find an index by roll number, or -1 if not present. */
static int findStudentIndexByRoll(const StudentDatabase *db, int rollNumber) {
    for (size_t i = 0; i < db->count; i++) {
        if (db->students[i].rollNumber == rollNumber) {
            return (int) i;
        }
    }
    return -1;
}

/* Turn marks into pass/fail text. */
static const char *getResultText(float marks) {
    return (marks > PASS_MARK) ? "Pass" : "Fail";
}

void initDatabase(StudentDatabase *db) {
    db->students = NULL;
    db->count = 0;
    db->capacity = 0;
}

void freeDatabase(StudentDatabase *db) {
    free(db->students);
    db->students = NULL;
    db->count = 0;
    db->capacity = 0;
}

void addStudent(StudentDatabase *db) {
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

void modifyStudent(StudentDatabase *db) {
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

void removeStudent(StudentDatabase *db) {
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

void displayStudents(const StudentDatabase *db) {
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

void searchStudent(const StudentDatabase *db) {
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

void displayAverageMarks(const StudentDatabase *db) {
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

static int compareMarksAscending(const void *a, const void *b) {
    const Student *s1 = (const Student *) a;
    const Student *s2 = (const Student *) b;
    if (s1->marks < s2->marks) return -1;
    if (s1->marks > s2->marks) return 1;
    return 0;
}

static int compareMarksDescending(const void *a, const void *b) {
    return compareMarksAscending(b, a);
}

void sortStudentsByMarks(StudentDatabase *db, int ascending) {
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

void saveToFile(const StudentDatabase *db, const char *filename) {
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

void loadFromFile(StudentDatabase *db, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file for reading: %s\n", filename);
        return;
    }

    db->count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
        char *newline = strchr(line, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }

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

