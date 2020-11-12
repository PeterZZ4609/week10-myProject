//
// Created by peter on 2020/11/12.
//

#ifndef MYPROJECT_DB_H
#define MYPROJECT_DB_H

typedef struct entry_student
{
    int id;
    char *name;
    struct entry_student *next;
} entry_student;

typedef struct entry_course
{
    int id;
    char *name;
    char *type;
    int hours;
    double credit;
    int students_limit;
    struct entry_course *next;
} entry_course;

typedef struct entry_selection
{
    int stu_id;
    int course_id;
    struct entry_selection *next;
} entry_selection;

int db_open(const char *db_path);
void db_close();

#endif //MYPROJECT_DB_H
