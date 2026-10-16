// salwa
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

double gradeToPoints(const char *grade) {
    if (strcmp(grade, "A")  == 0) return 4.0;
    if (strcmp(grade, "A-") == 0) return 3.7;
    if (strcmp(grade, "B+") == 0) return 3.3;
    if (strcmp(grade, "B")  == 0) return 3.0;
    if (strcmp(grade, "B-") == 0) return 2.7;
    if (strcmp(grade, "C+") == 0) return 2.3;
    if (strcmp(grade, "C")  == 0) return 2.0;
    if (strcmp(grade, "C-") == 0) return 1.7;
    if (strcmp(grade, "D+") == 0) return 1.3;
    if (strcmp(grade, "D")  == 0) return 1.0;
    if (strcmp(grade, "F")  == 0) return 0.0;
    return -1.0;   // nothing so invalid
}
void readGrade(char *grade) {
    char line[100];
    while (1) {
        readLine("Grade (A, A-, B+, B, B-, C+, C, C-, D+, D, F): ", line, sizeof(line));
        for (int i = 0; line[i] != '\0'; i++) {
            line[i] = toupper((unsigned char)line[i]);
        }
        if (strlen(line) <= 2 && gradeToPoints(line) >= 0) {
            strcpy(grade, line);   
            return;
        }
        printf("Invalid grade. Try again.\n");
    }
}

// test
int main(void) {
    char grade[4];                 

    readGrade(grade);              
    printf("You entered: %s\n", grade);
    printf("Grade points: %.1f\n", gradeToPoints(grade));

    return 0;
}
// arib
int mainMenu(const char *title,const char *options[], int count){
    int selected = 0;
    
    

    while (1) {
        system("cls");
        printf("===%s===\n\n", title);
        for (int i = 0; i < count; i++) {
            if (i == selected) {
                printf(">> %s\n", options[i]);
            }
            else {
                printf("   %s\n", options[i]);
            }
        }

        int key = _getch();

        if (key == 13) {                            // Enter
            system("cls");
            return selected;
        }
        else if (key == 224 || key == 0) {          // arrow key ; 72=up, 80=down;
            key = _getch();

            if (key == 72)      selected = (selected - 1 + count) % count;
            else if (key == 80) selected = (selected + 1) % count;
        }
    }
    return selected;
}

int main(void){
    char *sample[] = {"option 1", "option 2", "option 3"};
    int count = sizeof(sample)/sizeof(sample[0]);
    int selected = mainMenu("Main Menu ", sample, count);
    
}
