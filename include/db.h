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
int db_init_table();
int db_init_table();
void db_close();

int student_insert(int id, const char *name);
entry_student *student_select();
entry_student *student_select_by_id(int id);
int student_edit(int id, entry_student *data);
int student_delete(int id);

int course_insert(entry_course *course);
int course_delete(int id);
int course_edit(int id, entry_course *course);
entry_course *course_select(int id);
entry_course *course_select_all();

int selection_insert(entry_selection *selection);
int selection_delete(int stu_id, int course_id);
entry_selection *selection_find_course(int course_id);
entry_selection *selection_find_student(int stu_id);

#endif //MYPROJECT_DB_H
