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

int semesterCredits(const Semester *s) {
    int total = 0;
    for (int i = 0; i < s->courseCount; i++) {
        total += s->courses[i].credits;
    }
    return total;
}
double semesterPoints(const Semester *s) {
    double total = 0;
    for (int i = 0; i < s->courseCount; i++) {
        total += gradeToPoints(s->courses[i].grade) * s->courses[i].credits;
    }
    return total;
}
double calculateGPA(const Semester *s) {
    int credits = semesterCredits(s);
    if (credits == 0) return 0.0;          // avoid dividing by zero
    return semesterPoints(s) / credits;
}
int totalCreditsEarned(void) {
    int total = 0;
    for (int i = 0; i < semesterCount; i++) {
        total += semesterCredits(&semesters[i]);
    }
    return total;
}
double totalPointsEarned(void) {
    double total = 0;
    for (int i = 0; i < semesterCount; i++) {
        total += semesterPoints(&semesters[i]);
    }
    return total;
}
double calculateCGPA(void) {
    int credits = totalCreditsEarned();
    if (credits == 0) return 0.0;
    return totalPointsEarned() / credits;
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
void addSemester(void) {
    Semester s;
    
    if (semesterCount >= MAX_SEMESTERS) {
        printf("Semester limit reached.\n");
        return;
    }

    printf("--- Add Semester ---\n");

    s.number = readInt("Semester number: ", 1, MAX_SEMESTERS);

    for (int i = 0; i < semesterCount; i++) {
        if (semesters[i].number == s.number) {
            printf("Semester %d already exists.\n", s.number);
            return;
        }
    }

    s.courseCount = readInt("Number of courses: ", 1, MAX_COURSES);

    for (int i = 0; i < s.courseCount; i++) {
        printf("\nCourse %d\n", i + 1);

        readLine("Course name: ", s.courses[i].name, NAME_LEN);
        while (strlen(s.courses[i].name) == 0) {
            printf("Name cannot be empty.\n");
            readLine("Course name: ", s.courses[i].name, NAME_LEN);
        }

        s.courses[i].credits = readInt("Credit hours: ", 1, 6);
        readGrade(s.courses[i].grade);
    }

    semesters[semesterCount] = s;
    semesterCount++;
    printf("\nSemester %d saved.\n", s.number);
}
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
