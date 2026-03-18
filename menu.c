#include "menu.h"

#include <stdio.h>

void displayMenu(void) {
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

