#ifndef STUDENT_DB_H
#define STUDENT_DB_H

#include <stddef.h>

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

void initDatabase(StudentDatabase *db);
void freeDatabase(StudentDatabase *db);

void addStudent(StudentDatabase *db);
void modifyStudent(StudentDatabase *db);
void removeStudent(StudentDatabase *db);
void displayStudents(const StudentDatabase *db);
void searchStudent(const StudentDatabase *db);
void displayAverageMarks(const StudentDatabase *db);
void sortStudentsByMarks(StudentDatabase *db, int ascending);
void saveToFile(const StudentDatabase *db, const char *filename);
void loadFromFile(StudentDatabase *db, const char *filename);

#endif /* STUDENT_DB_H */
