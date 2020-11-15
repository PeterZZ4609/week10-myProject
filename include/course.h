#include "db.h"

#ifndef MYPROJECT_COURSE_H
#define MYPROJECT_COURSE_H

typedef struct course_info
{
    int id;
    char *name;
    char *type;
    int hours;
    double credit;
    int students_limit;
    entry_student *students;
    struct course_info *next;
} course_info;

course_info *get_all_courses_info();

int delete_course(int id);

int add_course(course_info *info);

int edit_course(int id, course_info *info);

#endif //MYPROJECT_COURSE_H