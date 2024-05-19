/*
Logen Autry
Fianl project
4/18/24
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

// creating structures for each file/data type
typedef struct dob{
  int day;
  int month;
  int year;
}dob;

typedef struct student{
  int id;
  char *fName;
  char *lName;
  dob dob;
  
}Student;

typedef struct course{
  char id[51];
  char cName[51];
  float totCredit;
}Course;

typedef struct enrollment{
  char cid[51];
  int sid;
  char semester[51];
  float score;
}Enrollment;

// initializing functions
Student* loadStudents(int *numStudents);
Enrollment* loadEnrollment(int *numEnrollments);
Course*  loadCourses(int *numCourses, char *fn);
void menu(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, char *fn);
void studSearch(char lname[], int id, Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls);
char letterGrade(float grade);
void courSearch(char *cid, char *s, Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls);
void addCourse(char *cid, char *name, int totCredit, Course *ArrayOfCourses, int *amountCourses, char *fn);
Course*  loadCourses2(int *numCourses, char *fn);
void freeA(Student *s, Course *c, Enrollment *e, int sCount);

int main(void) {

  // checks if there is a memory leak
  atexit(report_mem_leak);

  // initializes variables
  int sCount, eCount, cCount;
  Student *studArr;
  Course *couArr, *couArr2;
  Enrollment *enArr;
  char courseFileName[51];

  // loads arrays
  studArr = loadStudents(&sCount);
  couArr = loadCourses(&cCount, courseFileName);
  enArr = loadEnrollment(&eCount);

  // creates a second course array for editting in menu so no double freeing will occur
  couArr2 = couArr;
  menu(studArr, sCount, couArr2, cCount, enArr, eCount, courseFileName);
  
  freeA(studArr, couArr, enArr, sCount);
  
  return 0;
}

Student* loadStudents(int *numStudents){
  // initalizing variables
  int nameLen;
  char sFilename[51];
  FILE *sfp;
  char currFirName[51];
  char currLaName[51];
  
  // gets file name and opens
  scanf("%s", sFilename);
  sfp = fopen(sFilename, "r");

  // gets and updates number of students
  fscanf(sfp, "%d", numStudents);

  // sets up student array to the proper size
  Student *sArray = (Student*)malloc(*numStudents * sizeof(Student));

  // fills student array
  for(int i = 0; i < *numStudents; i++){

    // gets data from file
    fscanf(sfp, "%d %s %s %d %d %d", &sArray[i].id, currFirName, currLaName, &sArray[i].dob.day, &sArray[i].dob.month, &sArray[i].dob.year);

    // fills first name
    nameLen = strlen(currFirName);
    sArray[i].fName = (char*)malloc((nameLen + 1)*sizeof(char));
    strcpy(sArray[i].fName, currFirName);

    // fills last name
    nameLen = strlen(currLaName);
    sArray[i].lName = (char*)malloc((nameLen + 1)*sizeof(char));
    strcpy(sArray[i].lName, currLaName);
    
  }

  // closing file
  fclose(sfp);
  return sArray;
}

Course*  loadCourses(int *numCourses, char *fn){
  // initializing variables
  char cFileName[51];
  FILE *cfp;

  // getting filename and opening it
  scanf("%s", cFileName);
  strcpy(fn, cFileName);
  cfp = fopen(cFileName, "r");

  // getting course number
  fscanf(cfp, "%d", numCourses);

  Course *cArray = (Course*)malloc(*numCourses * sizeof(Course));
  for(int i = 0; i < *numCourses; i++){
    fscanf(cfp, "%s %s %f", cArray[i].id, cArray[i].cName, &cArray[i].totCredit);
  }

  // closing file
  fclose(cfp);
  return cArray;
  
}

Course*  loadCourses2(int *numCourses, char *fn){
  FILE *cfp;

  // getting filename and opening it
  cfp = fopen(fn, "r");

  // getting course number
  fscanf(cfp, "%d", numCourses);

  Course *cArray = (Course*)malloc(*numCourses * sizeof(Course));
  for(int i = 0; i < *numCourses; i++){
    fscanf(cfp, "%s %s %f", cArray[i].id, cArray[i].cName, &cArray[i].totCredit);
  }

  // closing file
  fclose(cfp);
  return cArray;
}

Enrollment* loadEnrollment(int *numEnrollments){
  // initializing variables
  char eFileName[51];
  FILE *efp;
  scanf("%s", eFileName);

  // opening file and getting enrollment number
  efp = fopen(eFileName, "r");
  fscanf(efp, "%d", numEnrollments);

  // filling array
  Enrollment *eArray = (Enrollment*)malloc(*numEnrollments * sizeof(Enrollment));

  for(int i = 0; i < *numEnrollments; i++){
    fscanf(efp, "%s %d %s %f", eArray[i].cid, &eArray[i].sid, eArray[i].semester, &eArray[i].score);
  }
 
  // closing file
  fclose(efp);
  return eArray;
}

void menu(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls, char *fn){

  // initializing variables
  char choice[51], secChoice[51], lastChoice[51], cid[51], semester[51];
  int id = 0, totCredit, courseAdded = 0;
  int courseCount, semesterCount, min_idx;
  char curString[51] = "", tempString[51]="";
  Student temp;

  printf("==================\n");
  // menu loop
  while(1){
    id = 0;

    // scans for first choice
    scanf("%s", choice);
    // checks if exit
    if(strcmp(choice, "exit")==0){
      printf("------------------\nThank you!");
      break;
    }
    
    // check if its search_students 
    else if(strcmp(choice, "search_students")==0){
      // scans for other choices
      scanf(" %s %s", secChoice, lastChoice);
      // checks if lname

      
      if(strcmp(secChoice, "lname")==0){

        studSearch(lastChoice, -1, ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
      }

      if(strcmp(secChoice, "id")==0){
        // makes last choice string into int
        for (int j = 0; lastChoice[j] != '\0'; j++)
          id = id * 10 + (lastChoice[j] - 48); 

        // calls student search
        studSearch("none", id, ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
        
      }
    }

    // search course command
    else if(strcmp(choice, "search_course")==0){
      scanf("%s", secChoice);

      // cid command
      if(strcmp(secChoice, "cid")==0){
        scanf("%s", lastChoice);
        printf("------------------\n");
        courSearch(lastChoice, "none", ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
      }

      // cid semester
      else if(strcmp(secChoice, "cid_semester")==0){
        scanf("%s %s", cid, semester);
        printf("------------------\n");
        courSearch(cid, semester, ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
      }
    }
    
    // add course command
    else if(strcmp(choice, "add_course")==0){
      scanf("%s %s %d",secChoice, lastChoice, &totCredit);
      printf("------------------\n");

      // checks if this command has already been run and frees previous array that is being pointed to to prevent memory leak
      if(courseAdded){
        free(ArrayOfCourses);
      }
      // adds new course to the file
      addCourse(secChoice, lastChoice, totCredit, ArrayOfCourses, &amountCourses, fn);
      courseAdded = 1;

      // loads new course into array of courses
      ArrayOfCourses = loadCourses2(&amountCourses, fn);

    }

    // count command
    else if(strcmp(choice, "count")==0){
      scanf("%s", secChoice);
      if(strcmp(secChoice, "students")==0){
        // prints amount of students
        printf("------------------\ntotal students %d\n------------------", amountStudents);
      }
      // prints students in course
      else if(strcmp(secChoice, "students_course")==0){
        printf("------------------");
        // for amount of courses
        for(int i = 0; i < amountCourses; i++){
          courseCount = 0;

          // if it is the current course the course count is added to
          for(int j = 0; j < amountEnrolls; j++){
            if(strcmp(ArrayOfCourses[i].id,ArrayOfEnrollments[j].cid)==0){
              courseCount++;
            }
          }
          // prints course count
          printf("\n%s %d", ArrayOfCourses[i].id, courseCount);
        }
        printf("\n------------------");
      }
      // prints students in semester
      else if(strcmp(secChoice, "students_semester")==0){
        printf("------------------\n");

        // for amount of enrolls
        for(int i = 0; i < amountEnrolls; i++){
          // sets semester count to zero each iteration
          semesterCount = 0;

          // makes temp string current semester
          strcpy(tempString, ArrayOfEnrollments[i].semester);

          // if the temp string is already counted for it skips to the next semester
          if(strcmp(curString,tempString)==0){
            continue;
          }

          // if it isnt it counts and prints
          else{
            strcpy(curString, ArrayOfEnrollments[i].semester);
          }
          for(int j = 0; j < amountEnrolls; j++){
            if(strcmp(curString,ArrayOfEnrollments[j].semester)==0){
              semesterCount++;
            }
          }
          printf("%s %d\n", ArrayOfEnrollments[i].semester, semesterCount);
        }
        printf("------------------");
      }
    }

    // sort command
    else if(strcmp(choice, "sort")==0){
      printf("------------------\n");

      // for amount of students
      for(int i = 0; i < amountStudents; i++){

        // sets min index to current iteration
        min_idx = i;

        // if min idx id is bigger the smaller one becomes new min idx
        for(int j = i+1; j < amountStudents; j++){
          if(ArrayOfStudents[min_idx].id > ArrayOfStudents[j].id){
            min_idx = j;
          }
        }

        // swaps
        temp = ArrayOfStudents[i];
        ArrayOfStudents[i] = ArrayOfStudents[min_idx];
        ArrayOfStudents[min_idx] = temp;
      }

      // prints
      for(int i = 0; i < amountStudents; i++){
        printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[i].id, ArrayOfStudents[i].fName, ArrayOfStudents[i].lName, ArrayOfStudents[i].dob.month, ArrayOfStudents[i].dob.day, ArrayOfStudents[i].dob.year);
      }
      printf("------------------");
    }

    printf("\n==================\n");
  }
  // frees array of course to prevent memory leak and checks if a memory leak would even be created by adding a course
  if(courseAdded){
    free(ArrayOfCourses);
  }
}

void studSearch(char lname[], int id, Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls){
  int found = 0;
  int i = 0;
  char grade;
  printf("------------------");
  // searching for by id
  if(strcmp(lname, "none")==0){

    // searching for student
    for(i = 0; i < amountStudents; i++){
      if(ArrayOfStudents[i].id == id){
        found = 1;
        // prints student once found
        printf("\n%d %s %s %d/%d/%d", ArrayOfStudents[i].id, ArrayOfStudents[i].fName, ArrayOfStudents[i].lName, ArrayOfStudents[i].dob.month, ArrayOfStudents[i].dob.day, ArrayOfStudents[i].dob.year);

        // searches for courses enrolled in
        for(int j = 0; j < amountEnrolls; j++){
          if(ArrayOfEnrollments[j].sid == id){

            // searches for name of courses
            for(int k = 0; k < amountCourses; k++){

              // prints once found
              if(strcmp(ArrayOfEnrollments[j].cid, ArrayOfCourses[k].id)==0){
                // gets letter grade
                grade = letterGrade(ArrayOfEnrollments[j].score);
                
                printf("\n%s %s %s %.0f %c", ArrayOfEnrollments[j].cid, ArrayOfCourses[k].cName, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, grade);
              }
            }
            
          }
        }
        // breaks once the student is found
        printf("\n------------------");
        break;
      }
    }
    if(!found){
      printf("\nnot found\n------------------");
    }
  }
  // searching by last name
  else if(id == -1){
    for(i = 0; i < amountStudents; i++){
      if(strcmp(lname, ArrayOfStudents[i].lName)==0){
        found = 1;
        // prints student once found
        printf("\n%d %s %s %d/%d/%d", ArrayOfStudents[i].id, ArrayOfStudents[i].fName, ArrayOfStudents[i].lName, ArrayOfStudents[i].dob.month, ArrayOfStudents[i].dob.day, ArrayOfStudents[i].dob.year);

        // searches for courses enrolled in
        for(int j = 0; j < amountEnrolls; j++){
          if(ArrayOfEnrollments[j].sid == ArrayOfStudents[i].id){

            // searches for name of courses
            for(int k = 0; k < amountCourses; k++){

              // prints once found
              if(strcmp(ArrayOfEnrollments[j].cid, ArrayOfCourses[k].id)==0){
                // gets letter grade
                grade = letterGrade(ArrayOfEnrollments[j].score);

                printf("\n%s %s %s %.0f %c", ArrayOfEnrollments[j].cid, ArrayOfCourses[k].cName, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, grade);
              }
            }

          }
        }
        printf("\n------------------");
      }
    }
    // if not found
    if(!found){
      printf("\nnot found\n------------------");
    }
  }
  
}

char letterGrade(float grade){
  // calculates letter grade from float
  if(grade >= 90){
    return 'A';
  }
  else if(grade < 90 && grade >= 80){
    return 'B';
  }
  else if(grade < 80 && grade >= 70){
    return 'C';
  }
  else if(grade < 70 && grade >= 60){
    return 'D';
  }
  else{
    return 'F';
  }
}

void courSearch(char *cid, char *s, Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls){
  char grade;
  int found = 0;
  // search course cid
  if(strcmp(s, "none")==0){

    // searches for course
    for(int i = 0; i < amountCourses; i++){
      if(strcmp(cid, ArrayOfCourses[i].id)==0){
        found = 1;
        // prints course
        printf("%s %.0f", ArrayOfCourses[i].cName, ArrayOfCourses[i].totCredit);

        // searches for enrollment
        for(int j = 0; j < amountEnrolls; j++){
          if(strcmp(cid, ArrayOfEnrollments[j].cid)==0){

            // searches for student by found sid
            for(int k = 0; k < amountStudents; k++){

              // prints student in course
              if(ArrayOfEnrollments[j].sid == ArrayOfStudents[k].id){
                grade = letterGrade(ArrayOfEnrollments[j].score);
                printf("\n%s %s %s %.0f %c", ArrayOfStudents[k].fName, ArrayOfStudents[k].lName, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, grade);
              }
            }
          }
        }
      }
    }
    if(!found){
      printf("not found");
    }
    printf("\n------------------");
  }
  else{
    // searches for course
    for(int i = 0; i < amountCourses; i++){
      if(strcmp(cid, ArrayOfCourses[i].id)==0){
        found = 1;
        // prints course
        printf("%s %.0f", ArrayOfCourses[i].cName, ArrayOfCourses[i].totCredit);

        // searches for enrollment
        for(int j = 0; j < amountEnrolls; j++){
          if(strcmp(cid, ArrayOfEnrollments[j].cid)==0){

            // searches for student by found sid
            for(int k = 0; k < amountStudents; k++){

              // prints student in course
              if(ArrayOfEnrollments[j].sid == ArrayOfStudents[k].id){

                // checks if right semester
                if(strcmp(ArrayOfEnrollments[j].semester, s)==0){
                  grade = letterGrade(ArrayOfEnrollments[j].score);
                  printf("\n%s %s %s %.0f %c", ArrayOfStudents[k].fName, ArrayOfStudents[k].lName, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, grade);
                }
              }
            }
          }
        }
      }
    }
    if(!found){
      printf("not found");
    }
    printf("\n------------------");
    
  }
  
}

void addCourse(char *cid, char *name, int totCredit, Course *ArrayOfCourses, int *amountCourses, char *fn){
  int found = 0;
  FILE *fp;

  // opening file to be editted
  fp = fopen(fn, "r+");
  
  // checks if unique
  for(int i = 0; i < *amountCourses; i++){
    if(strcmp(ArrayOfCourses[i].id, cid)==0){
      printf("\"course id needs to be unique. This course cannot be added.\"");
      found = 1;
      break;
    }
  }
  // if unique
  if(!found){
    *amountCourses += 1;
    fprintf(fp, "%d", *amountCourses);
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "\n%s %s %d", cid, name, totCredit);
    printf("record added");
  }
  fclose(fp);
  printf("\n------------------");
}

void freeA(Student *s, Course *c, Enrollment *e, int sCount){
  // freeing studArr 
  for(int i = 0; i < sCount; i++){
    free(s[i].fName);
    free(s[i].lName);
  }
  free(s);

  // freeing couArr
  free(c);

  // freeing enArr
  free(e);
}