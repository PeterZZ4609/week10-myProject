#include <stdio.h>
#include <stdlib.h>

#include "course.h"

int delete_course(int id);

int add_course(course_info *info);

int edit_course(int id, course_info *info);

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

            // students
            entry_student *students = (entry_student *)malloc(sizeof(entry_student));
            entry_selection *selections = selection_find_course(this->id);
            if (selections)
            {
                // entry_student *this = 
            }
            else
            {
                this->students = NULL;
            }
            now->next = this;
            now = now->next;
            entries = entries->next;
        }
    }
    else
    {
        return NULL;
    }
}