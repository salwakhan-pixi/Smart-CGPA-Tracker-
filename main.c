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
void saveData(void) {
    FILE *fp = fopen(DATA_FILE, "w");       
    if (fp == NULL) {                        
        printf("Warning: could not save data!\n");
        return;
    }
    fprintf(fp, "%d\n%d\n", degreeCredits, semesterCount);
    for (int i = 0; i < semesterCount; i++) {
        fprintf(fp, "%d %d\n", semesters[i].number, semesters[i].courseCount);
        for (int j = 0; j < semesters[i].courseCount; j++) {
            fprintf(fp, "%d %s %s\n", semesters[i].courses[j].credits,semesters[i].courses[j].grade,semesters[i].courses[j].name);
        }
    }

    fclose(fp);                             
}
void loadData(void) {
    FILE *fp = fopen(DATA_FILE, "r");     
    if (fp == NULL) {
        return;                              
    }
    if (fscanf(fp, "%d %d", &degreeCredits, &semesterCount) != 2 ||
        semesterCount < 0 || semesterCount > MAX_SEMESTERS) {
        degreeCredits = 0;
        semesterCount = 0;
        fclose(fp);
        return;
    }
    for (int i = 0; i < semesterCount; i++) {
        fscanf(fp, "%d %d", &semesters[i].number, &semesters[i].courseCount);
        for (int j = 0; j < semesters[i].courseCount; j++) {
            fscanf(fp, "%d %3s %49[^\n]", &semesters[i].courses[j].credits,semesters[i].courses[j].grade,semesters[i].courses[j].name);
        }
    }

    fclose(fp);
}
void viewReport(void) {
    if (semesterCount == 0) {
        printf("No semesters entered yet.\n");
        return;
    }
    for (int i = 0; i < semesterCount; i++) {
        Semester *s = &semesters[i];

        printf("\n=== Semester %d ===\n", s->number);
        printf("%-25s %-8s %-6s\n", "Course", "Credits", "Grade");
        printf("---------------------------------------\n");
        for (int j = 0; j < s->courseCount; j++) {
            printf("%-25s %-8d %-6s\n", s->courses[j].name,
                                        s->courses[j].credits,
                                        s->courses[j].grade);
        }
        printf("---------------------------------------\n");
        printf("Semester GPA: %.2f  (%d credits)\n",
               calculateGPA(s), semesterCredits(s));
    }
    printf("\n=======================================\n");
    printf("OVERALL CGPA: %.2f  (%d credits)\n",
           calculateCGPA(), totalCreditsEarned());
    printf("=======================================\n");
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
void viewSemesters(void) {
    if (semesterCount == 0) {
        printf("No semesters added yet.\n");
        return;
    }

    printf("--- All Semesters ---\n");

    for (int i = 0; i < semesterCount; i++) {
        int totalCredits = 0;
        double totalPoints = 0;

        printf("\nSemester %d\n", semesters[i].number);
        printf("%-30s %-8s %-6s\n", "Course", "Credits", "Grade");
        printf("----------------------------------------------\n");

        for (int j = 0; j < semesters[i].courseCount; j++) {
            printf("%-30s %-8d %-6s\n",
                   semesters[i].courses[j].name,
                   semesters[i].courses[j].credits,
                   semesters[i].courses[j].grade);

            totalCredits += semesters[i].courses[j].credits;
            totalPoints += gradeToPoints(semesters[i].courses[j].grade)
                           * semesters[i].courses[j].credits;
        }
        
        printf("----------------------------------------------\n");
        printf("Credits: %d    GPA: %.2f\n", totalCredits, totalPoints / totalCredits);
    }
}
void editSemester(void) {
    Semester temp;
    char title[1500];
    char line[100];
    char courseText[MAX_COURSES][100];
    const char *courseOptions[MAX_COURSES];
    const char *mainOptions[] = {"Edit a course", "Add a course", "Remove a course",
                                 "Save changes", "Cancel (discard changes)"};
    const char *fieldOptions[] = {"Name", "Credit hours", "Grade"};
    int number, idx = -1, choice, c, field, i;

    if (semesterCount == 0) {
        printf("No semesters to edit.\n");
        return;
    }

    printf("--- Edit Semester ---\n");
    number = readInt("Semester number to edit: ", 1, MAX_SEMESTERS);

    for (i = 0; i < semesterCount && idx == -1; i++) {
        if (semesters[i].number == number) {
            idx = i;
        }
    }

    if (idx == -1) {
        printf("Semester %d not found.\n", number);
        return;
    }

    temp = semesters[idx];

    do {
        sprintf(title, "Editing Semester %d\n\n%-4s %-30s %-8s %-6s",
                temp.number, "No.", "Course", "Credits", "Grade");

        for (i = 0; i < temp.courseCount; i++) {
            sprintf(line, "\n%-4d %-30s %-8d %-6s", i + 1, temp.courses[i].name,
                    temp.courses[i].credits, temp.courses[i].grade);
            strcat(title, line);

            sprintf(courseText[i], "%d. %s (%d credits, %s)", i + 1,
                    temp.courses[i].name, temp.courses[i].credits,
                    temp.courses[i].grade);
            courseOptions[i] = courseText[i];
        }

        choice = menu(title, mainOptions, 5);

        if (choice == 0) {
            c = menu("Which course do you want to edit?", courseOptions, temp.courseCount);
            if (c != -1) {
                field = menu("What do you want to change?", fieldOptions, 3);
                clearScreen();

                if (field == 0) {
                    do {
                        readLine("New course name: ", temp.courses[c].name, NAME_LEN);
                        if (strlen(temp.courses[c].name) == 0) {
                            printf("Name cannot be empty.\n");
                        }
                    } while (strlen(temp.courses[c].name) == 0);
                } else if (field == 1) {
                    temp.courses[c].credits = readInt("New credit hours: ", 1, 6);
                } else if (field == 2) {
                    readGrade(temp.courses[c].grade);
                }
            }
        } else if (choice == 1) {
            clearScreen();
            if (temp.courseCount >= MAX_COURSES) {
                printf("This semester already has %d courses.\n", MAX_COURSES);
                pressKey();
            } else {
                c = temp.courseCount++;

                do {
                    readLine("Course name: ", temp.courses[c].name, NAME_LEN);
                    if (strlen(temp.courses[c].name) == 0) {
                        printf("Name cannot be empty.\n");
                    }
                } while (strlen(temp.courses[c].name) == 0);
                temp.courses[c].credits = readInt("Credit hours: ", 1, 6);
                readGrade(temp.courses[c].grade);
            }
        } else if (choice == 2) {
            if (temp.courseCount <= 1) {
                clearScreen();
                printf("A semester needs at least one course.\n");
                pressKey();
            } else {
                c = menu("Which course do you want to remove?", courseOptions, temp.courseCount);
                if (c != -1) {
                    for (i = c; i < temp.courseCount - 1; i++) {
                        temp.courses[i] = temp.courses[i + 1];
                    }
                    temp.courseCount--;
                }
            }
        }
    } while (choice >= 0 && choice <= 2);

    clearScreen();
    if (choice == 3) {
        semesters[idx] = temp;
        printf("Changes saved.\n");
    } else {
        printf("Edit cancelled. Nothing was changed.\n");
    }
}
int main(void){
    char *sample[] = {"option 1", "option 2", "option 3"};
    int count = sizeof(sample)/sizeof(sample[0]);
    int selected = mainMenu("Main Menu ", sample, count);
    
}
