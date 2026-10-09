#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_SEMESTERS 20
#define MAX_COURSES   10
#define NAME_LEN      50
#define DATA_FILE     "students_data.dat"
typedef struct {
    char name[NAME_LEN];
    int  credits;
    char grade[4];
} Course;
typedef struct {
    int    number;
    int    courseCount;
    Course courses[MAX_COURSES];
} Semester;
Semester semesters[MAX_SEMESTERS];
int semesterCount = 0;
int degreeCredits = 0;

void readLine(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}
int readInt(const char *prompt, int min, int max) {
    char line[100];
    int value;
    while (1) {
        readLine(prompt, line, sizeof(line));
        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("  Invalid input. ...\n");
    }
}

int main()
{
    printf("Smart CGPA Tracker & Degree Progress Planner\n");
    return 0;
}
