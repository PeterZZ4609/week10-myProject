#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    db_init_mockdata();
    // 主循环
    while (1)
    {
        // 主菜单
        puts(SPLINE_1);
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
    char c;
    while ('\n' == (c = getchar()))
        ;
    while ('\n' != getchar())
        ;
    fflush(stdin);
    return c;
}

void manage_student()
{
    char flag = 1;
    while (flag)
    {

        puts(SPLINE_2);
        puts("1. 添加学生信息");
        puts("2. 删除学生信息");
        puts("3. 修改学生信息");
        puts("4. 查看所有学生信息");
        puts("0. 返回上一级");
        char op = wait_option();
        switch (op)
        {
        case '1':
            puts(SPLINE_2);
            puts("学生ID：");
            int id;
            scanf("%d", &id);
            puts("学生姓名：");
            char name[1024];
            scanf("%s", name);
            puts("添加中...");
            if (student_insert(id, name) == 1)
                puts("添加成功！");
            else
                puts("添加失败！");
            break;
        case '2':
            puts(SPLINE_2);
            puts("请输入将要删除的学生ID：");
            int id2;
            scanf("%d", &id2);
            entry_student *stu2 = student_select_by_id(id2);
            if (stu2 == NULL)
            {
                puts("该学生不存在！");
            }
            else
            {
                puts("删除中...");
                student_delete(id2);
                puts("删除成功！");
            }
            break;
        case '3':
            puts("请输入将要修改的学生的ID：");
            int id3;
            scanf("%d", &id3);
            entry_student *stu3 = student_select_by_id(id3);
            if (stu3 == NULL)
            {
                puts("该学生不存在！");
            }
            else
            {
                puts("输入新ID（0表示不修改）：");
                int new_id;
                scanf("%d", &new_id);
                if (new_id > 0)
                {
                    stu3->id = new_id;
                }
                puts("输入新的姓名：");
                char name[1024];
                scanf("%s", name);
                if (strlen(name) > 0)
                {
                    stu3->name = name;
                }
                puts("修改中：");
                if (student_edit(id3, stu3))
                    puts("\033[01;32m修改成功！\033[0m");
                else
                    puts("修改失败！");
            }
            break;
        case '4':
            puts(SPLINE_2);
            student_info *all = get_all_students_info();
            student_info *iter = all;
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
            break;
        case '0':
            flag = 0;
            break;
        default:
            break;
        }
    }
}

void manage_course()
{
}