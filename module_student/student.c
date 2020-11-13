#include <stdlib.h>

#include "student.h"
#include "db.h"

int add_student(student_info *info)
{
    return student_insert(info->id, info->name);
}
student_info *get_all_students_info()
{
    entry_student *entries = student_select();
    if (entries)
    {
        student_info *students = (student_info *)malloc(sizeof(student_info));
        students->next = NULL;
        student_info *now = students;
        while (entries)
        {
            student_info *this = (student_info *)malloc(sizeof(student_info));
            this->next = NULL;
            this->id = entries->id;
            this->name = entries->name;
            // courses
            entry_selection *selections = selection_find_student(this->id);
            if (selections)
            {
                entry_course *courses = (entry_course *)malloc(sizeof(entry_course));
                courses->next = NULL;
                entry_course *now = courses;
                while (selections)
                {
                    entry_course *this = course_select(selections->course_id);
                    now->next = this;
                    now = now->next;
                    selections = selections->next;
                }
                students->courses = courses->next; // 需要free()之前的节点
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