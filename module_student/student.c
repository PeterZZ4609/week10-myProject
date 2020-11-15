#include <stdlib.h>

#include "student.h"
#include "db.h"

int add_student(student_info *info)
{
    return student_insert(info->id, info->name);
}
student_info *get_all_students_info()
{
    // puts("s1");
    entry_student *entries = student_select();
    // puts("s1.end");
    if (entries)
    {
        student_info *students = (student_info *)malloc(sizeof(student_info));
        students->next = NULL;
        student_info *now = students;
        // puts("---");
        while (entries)
        {
            // puts("start");
            student_info *this = (student_info *)malloc(sizeof(student_info));
            this->next = NULL;
            this->id = entries->id;
            // char *tmp = malloc(strlen(entries->name) + 1);
            // strcpy(tmp, entries->name);
            this->name = entries->name;
            // courses
            entry_selection *selections = selection_find_student(this->id);
            // puts("end");
            if (selections)
            {
                entry_course *courses = (entry_course *)malloc(sizeof(entry_course));
                courses->next = NULL;
                entry_course *now2 = courses;
                while (selections)
                {
                    entry_course *this2 = course_select(selections->course_id);
                    now2->next = this2;
                    now2 = now2->next;
                    selections = selections->next;
                    // puts(now2->name);
                }
                this->courses = courses->next; // 需要free()之前的节点
            }
            else
            {
                this->courses = NULL;
            }
            //courses ok
            now->next = this;
            now = now->next;
            entries = entries->next;
        }
        return students->next; // need free()
    }
    else
    {
        return NULL;
    }
}
int delete_student(int id)
{
    return student_delete(id);
}