#include "db.h"

#ifndef MYPROJECT_STUDENT_H
#define MYPROJECT_STUDENT_H

/**
 * Data structure for showing a student's detail.
 */

typedef struct student_info
{
    int id;
    char *name;
    entry_course *courses;
    struct student_info *next;
} student_info;

int add_student(student_info *info);
student_info *get_all_students_info();
int delete_student(int i);

#endif //MYPROJECT_STUDENT_H