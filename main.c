// salwa
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>     
#include <conio.h>      

#define MAX_SEMESTERS 20
#define MAX_COURSES   10
#define NAME_LEN      50
#define DATA_FILE     "students_data.dat"
typedef struct{
    char name[NAME_LEN];
    int credits;
    char grade[4];
} Course;
typedef struct{
    int number;
    int courseCount;
    Course courses[MAX_COURSES];
} Semester;

Semester semesters[MAX_SEMESTERS];
int semesterCount = 0;
int degreeCredits = 0;
void readLine(const char *prompt, char *buffer, int size){
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL){
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}
int readInt(const char *prompt, int min, int max){
    char line[100];
    int value;
    while (1){
        readLine(prompt, line, sizeof(line));
        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max){
            return value;
        }
        printf("  Invalid input. Enter a whole number from %d to %d.\n", min, max);
    }
}
double readDouble(const char *prompt, double min, double max){
    char line[100];
    double value;
    while (1){
        readLine(prompt, line, sizeof(line));
        if (sscanf(line, "%lf", &value) == 1 && value >= min && value <= max){
            return value;
        }
        printf("  Invalid input. Enter a number from %.2f to %.2f.\n", min, max);
    }
}
double gradeToPoints(const char *grade){
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
    return -1.0;   
}
void readGrade(char *grade){
    char line[100];
    while (1){
        readLine("Grade (A, A-, B+, B, B-, C+, C, C-, D+, D, F): ", line, sizeof(line));
        for (int i = 0; line[i] != '\0'; i++){
            line[i] = toupper((unsigned char)line[i]);
        }
        if (strlen(line) <= 2 && gradeToPoints(line) >= 0){
            strcpy(grade, line);
            return;
        }
        printf("Invalid grade. Try again.\n");
    }
}
int semesterCredits(const Semester *s){
    int total = 0;
    for (int i = 0; i < s->courseCount; i++){
        total += s->courses[i].credits;
    }
    return total;
}
double semesterPoints(const Semester *s){
    double total = 0;
    for (int i = 0; i < s->courseCount; i++) {
        total += gradeToPoints(s->courses[i].grade) * s->courses[i].credits;
    }
    return total;
}
double calculateGPA(const Semester *s){
    int credits = semesterCredits(s);
    if (credits == 0) return 0.0;         
    return semesterPoints(s) / credits;
}
int totalCreditsEarned(void){
    int total = 0;
    for (int i = 0; i < semesterCount; i++){
        total += semesterCredits(&semesters[i]);
    }
    return total;
}
double totalPointsEarned(void){
    double total = 0;
    for (int i = 0; i < semesterCount; i++){
        total += semesterPoints(&semesters[i]);
    }
    return total;
}
double calculateCGPA(void){
    int credits = totalCreditsEarned();
    if (credits == 0) return 0.0;
    return totalPointsEarned() / credits;
}
void saveData(void){
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf("Warning: could not save data!\n");
        return;
    }
    fprintf(fp, "%d\n%d\n", degreeCredits, semesterCount);
    for (int i = 0; i < semesterCount; i++) {
        fprintf(fp, "%d %d\n", semesters[i].number, semesters[i].courseCount);
        for (int j = 0; j < semesters[i].courseCount; j++) {
            fprintf(fp, "%d %s %s\n", semesters[i].courses[j].credits,
                                      semesters[i].courses[j].grade,
                                      semesters[i].courses[j].name);
        }
    }
    fclose(fp);
}
void loadData(void){
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL){
        return;                             
    }
    if (fscanf(fp, "%d %d", &degreeCredits, &semesterCount) != 2 ||semesterCount < 0 || semesterCount > MAX_SEMESTERS){
        degreeCredits = 0;
        semesterCount = 0;
        fclose(fp);
        return;
    }
    for (int i = 0; i < semesterCount; i++){
        fscanf(fp, "%d %d", &semesters[i].number, &semesters[i].courseCount);
        if (semesters[i].courseCount < 1 || semesters[i].courseCount > MAX_COURSES) {
            degreeCredits = 0;
            semesterCount = 0;
            fclose(fp);
            return;
        }
        for (int j = 0; j < semesters[i].courseCount; j++){
            fscanf(fp, "%d %3s %49[^\n]", &semesters[i].courses[j].credits,semesters[i].courses[j].grade,semesters[i].courses[j].name);
        }
    }
    fclose(fp);
}
void viewReport(void){
    if (semesterCount == 0){
        printf("No semesters entered yet.\n");
        return;
    }
    for (int i = 0; i < semesterCount; i++){
        Semester *s = &semesters[i];
        printf("\n=== Semester %d ===\n", s->number);
        printf("%-25s %-8s %-6s\n", "Course", "Credits", "Grade");
        printf("---------------------------------------\n");
        for (int j = 0; j < s->courseCount; j++) {
            printf("%-25s %-8d %-6s\n", s->courses[j].name,s->courses[j].credits,s->courses[j].grade);
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

void degreeProgress(void){
    if (degreeCredits == 0){
        degreeCredits = readInt("Total credit hours needed for your degree: ", 1, 500);
        saveData();
    }
    int done = totalCreditsEarned();
    double percent = (double)done / degreeCredits * 100.0;
    printf("\nCredits completed: %d of %d\n", done, degreeCredits);
    printf("You have completed %.1f%% of your degree requirements.\n", percent);
    printf("Remaining credits: %d\n", degreeCredits - done);
}

void targetSimulator(void){
    if (degreeCredits == 0){
        degreeCredits = readInt("Total credit hours needed for your degree: ", 1, 500);
        saveData();
    }
    double target = readDouble("Target CGPA (0.0 - 4.0): ", 0.0, 4.0);
    int done = totalCreditsEarned();
    int remaining = degreeCredits - done;
    if (remaining <= 0){
        printf("\nYou have already completed all credits. Final CGPA: %.2f\n",
               calculateCGPA());
        return;
    }
    double pointsNeeded = target * degreeCredits - totalPointsEarned();
    double requiredGPA  = pointsNeeded / remaining;
    printf("\nCurrent CGPA: %.2f | Remaining credits: %d\n", calculateCGPA(), remaining);
    if (requiredGPA > 4.0){
        printf("A target of %.2f is no longer mathematically achievable.\n", target);
        printf("(You would need %.2f, but the maximum GPA is 4.00.)\n", requiredGPA);
    } else if (requiredGPA <= 0){
        printf("You have already secured this target, whatever your future grades are!\n");
    } else{
        printf("To reach a CGPA of %.2f you must average %.2f GPA in your remaining semesters.\n",target, requiredGPA);
    }
}

// arib
void clearScreen(void) {
    system("cls");
}

void pressKey(void) {
    printf("\nPress any key to continue...");
    _getch();
}
int menu(const char *title, const char *options[], int count) {
    int selected = 0;

    while (1) {
        clearScreen();
        printf("========================================\n");
        printf("%s\n\n", title);
        for (int i = 0; i < count; i++) {
            if (i == selected) {
                printf(">> %s\n", options[i]);
            } else {
                printf("   %s\n", options[i]);
            }
        }

        int key = _getch();

        if (key == 13) {                            
            clearScreen();
            return selected;
        } else if (key == 27) {                   
            clearScreen();
            return -1;
        } else if (key == 224 || key == 0) {        // arrow key: 72 = up, 80 = down
            key = _getch();
            if (key == 72)      selected = (selected - 1 + count) % count;
            else if (key == 80) selected = (selected + 1) % count;
        }
    }
}

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
    printf("Semester GPA: %.2f\n", calculateGPA(&s));
    printf("Your CGPA is now: %.2f\n", calculateCGPA());
    saveData();
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
        saveData();                          
        printf("Changes saved.\n");
        printf("Semester %d GPA: %.2f | CGPA: %.2f\n",
               temp.number, calculateGPA(&temp), calculateCGPA());
    } else {
        printf("Edit cancelled. Nothing was changed.\n");
    }
}


// marium

void deleteSemester(void) {
    int number;       
    int index = -1;      
    char answer[10];

    if (semesterCount == 0) {
        printf("There are no semesters to delete.\n");
        return;                                    
    }
    printf("-------- DELETE SEMESTER --------\n");
    number = readInt("Semester no. to be deleted: ", 1, MAX_SEMESTERS);

    for (int i = 0; i < semesterCount; i++) {
        if (semesters[i].number == number) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Semester %d NOT FOUND!\n", number);
        return;
    }
    printf("Semester %d has %d courses (%d credits)\n", semesters[index].number,
           semesters[index].courseCount, semesterCredits(&semesters[index]));

    readLine("Are you sure you want to delete it? (y/n): ", answer, sizeof(answer));

   
    if (answer[0] != 'y' && answer[0] != 'Y') {
        printf("Deletion of semester CANCELLED.\n");
        return;
    }

    for (int i = index; i < semesterCount - 1; i++) {
        semesters[i] = semesters[i + 1];
    }
    semesterCount--;
    saveData();

    printf("Semester %d has been deleted. CGPA is now %.2f\n", number, calculateCGPA());
}

int main(void){
    const char *options[] = {"Add Semester", "View Report", "Edit Semester","Delete Semester", "Degree Progress","Target CGPA Simulator", "Exit"};
    int choice;
    loadData();
    do {
        choice = menu("SMART CGPA TRACKER", options, 7);
        switch (choice){
            case 0: addSemester();     
            break;
            case 1: viewReport();      
            break;
            case 2: editSemester();   
            break;
            case 3: deleteSemester();  
            break;
            case 4: degreeProgress();  
            break;
            case 5: targetSimulator(); 
            break;
            case 6: saveData(); 
            printf("Data saved. Goodbye!\n"); 
            break;
        }
        if (choice >= 0 && choice != 6){
            pressKey();                
        }
    }while (choice != 6);
    return 0;
}
