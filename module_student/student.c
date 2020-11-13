#include <stdlib.h>

#include "student.h"
#include "db.h"

int add_student(student_info *info)
{
    return student_insert(info->id, info->name);
}
student_info *get_all_students_info()
{
    entry_student* entries = student_select();
    if(entries){
        student_info* students = (student_info*)malloc(sizeof(student_info));
        students->next = NULL;
        while(entries){
            student_info* this = (student_info*)malloc(sizeof(student_info));
            this->id = entries->id;
            this->name = entries->name;
            // courses
            selection_find_course()
        }
    }
}
int delete_student(int i);