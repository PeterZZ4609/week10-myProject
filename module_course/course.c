#include <stdio.h>
#include <stdlib.h>

#include "course.h"

int delete_course(int id)
{
    return course_delete(id);
}

int add_course(course_info *info)
{
    entry_course c;
    c.id = info->id;
    c.name = info->name;
    c.type = info->type;
    c.hours = info->hours;
    c.credit = info->credit;
    c.students_limit = info->students_limit;
    return course_insert(&c);
}

int edit_course(int id, course_info *info)
{
    entry_course c;
    c.id = info->id;
    c.name = info->name;
    c.type = info->type;
    c.hours = info->hours;
    c.credit = info->credit;
    c.students_limit = info->students_limit;
    return course_edit(id, &c);
}

course_info *get_all_courses_info()
{
    entry_course *entries = course_select_all();
    if (entries)
    {
        course_info *courses = (course_info *)malloc(sizeof(course_info));
        courses->next = NULL;
        course_info *now = courses;
        while (entries)
        {
            course_info *this = (course_info *)malloc(sizeof(course_info));
            this->next = NULL;
            this->id = entries->id;
            this->name = entries->name;
            this->type = entries->type;
            this->hours = entries->hours;
            this->credit = entries->credit;
            this->students_limit = entries->students_limit;

            this->students = NULL;

            // students
            entry_selection *selections = selection_find_course(this->id);
            if (selections)
            {
                entry_student *students = (entry_student *)malloc(sizeof(entry_student));
                students->next = NULL;
                entry_student *now = students;
                while (selections)
                {
                    entry_student *this = student_select_by_id(selections->stu_id);
                    now->next = this;
                    now = now->next;
                    selections = selections->next;
                }
                this->students = students->next; // need free()
            }
            now->next = this;
            now = now->next;
            entries = entries->next;
        }

        return courses->next;
    }
    else
    {
        return NULL;
    }
}