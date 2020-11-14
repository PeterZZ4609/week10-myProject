#include <stdio.h>
#include <stdlib.h>

#include "student.h"
#include "course.h"

#define INDENT "    "
#define SPLINE_1 "============================================"
#define SPLINE_2 "--------------------------------------------"

char wait_option();
void manage_student();
void manage_course();

int main(void)
{
    // 初始化数据库
    db_open("./test.db");
    db_init_table();
    // 主循环
    while (1)
    {
        // 主菜单
        puts("----------------");
        puts("1. 管理学生信息");
        puts("2. 管理课程信息");
        puts("0. 退出");
        char op = wait_option();
        switch (op)
        {
        case '1':
            manage_student();
            break;
        case '2':
            manage_course();
            break;
        case '0':
            exit(0);
        default:
            break;
        }
    }
    db_close();
    exit(0);
}

char wait_option()
{
    printf("> ");
    char c = getchar();
    while ('\n' != getchar())
        ;
    fflush(stdin);
    return c;
}

void manage_student()
{
    // puts("1");
    puts(SPLINE_1);
    student_info *all = get_all_students_info();
    student_info *iter = all;
    // puts("1.end");
    while (iter)
    {
        // puts("2");
        printf("%s ID:%d\n", iter->name, iter->id);
        entry_course *iter2 = iter->courses;
        if (!iter2)
        {
            printf("%s空\n", INDENT);
        }
        else
        {
            // puts("3");
            while (iter2)
            {
                printf("%s(%d)%s %s %d学时 %.1f学分\n", INDENT, iter2->id, iter2->name, iter2->type, iter2->hours, iter2->credit);
                iter2 = iter2->next;
            }
        }
        puts(SPLINE_2);
        iter = iter->next;
    }
}

void manage_course()
{
}