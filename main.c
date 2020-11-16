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
    char flag = 1;
    while (flag)
    {
        puts(SPLINE_1);
        puts("1. 添加新课程");
        puts("2. 删除一门课程");
        puts("3. 修改一门课程");
        puts("4. 查看所有课程信息");
        puts("5. 操作某课程选课信息");
        char op = wait_option();
        switch (op)
        {
        case '1':
            entry_course *course1 = (entry_course *)malloc(sizeof(entry_course));
            course1->next = NULL;

            int id1 = 0;
            while (id1 <= 0)
            {
                puts("输入课程ID：");
                scanf("%d", &id1);
                if (course_select(id1) != NULL)
                    puts("课程ID已存在！");
                else if (id1 <= 0)
                    puts("课程ID不合法！");
                else
                    course1->id = id1;
            }

            char name1[1024] = {0};
            while (strlen(name1) <= 0)
            {
                puts("输入课程名称：");
                scanf("%s", name1);
                if (strlen(name1) <= 0)
                    puts("课程名称至少包含一个字符！");
                else
                    course1->name = name1;
            }

            char type1[1024] = {0};
            while (strlen(type1) <= 0)
            {
                puts("输入课程性质：");
                scanf("%s", type1);
                if (strlen(type1) <= 0)
                    puts("课程性质至少包含一个字符！");
                else
                    course1->type = type1;
            }

            int hours1 = -1;
            while (hours1 < 0)
            {
                puts("输入课程学时：");
                scanf("%d", &hours1);
                if (hours1 < 0)
                    puts("课程学时不能为负数！");
                else
                    course1->hours = hours1;
            }

            double credit1 = -1.0;
            while (credit1 < 0)
            {
                puts("输入课程学分：");
                scanf("%f", &credit1);
                if (credit1 < 0)
                    puts("课程学分不能为负数！");
                else
                    course1->credit = credit1;
            }

            int limit1 = -1;
            while (limit1 < 0)
            {
                puts("输入课程人数限制：");
                scanf("%d", &limit1);
                if (limit1 < 0)
                    puts("课程人数限制不能为负数！");
                else
                    course1->students_limit = limit1;
            }
            puts("添加中...");
            if (course_insert(course1) == 1)
                puts("添加成功！");
            else
                puts("添加失败！");
            break;
        case '2':
            int id2;
            puts("输入将要删除的课程ID：");
            scanf("%d", &id2);
            while (course_select(id2) == NULL)
            {
                puts("课程不存在！");
                puts("输入将要删除的课程ID：");
                scanf("%d", &id2);
            }
            puts("删除中...");
            if (course_delete(id2) == 1)
                puts("删除成功！");
            else
                puts("删除失败！");
            break;
        case '3':
            int id3;
            puts("输入将要修改的课程ID：");
            scanf("%d", &id3);
            entry_course *c3;
            while ((c3 = course_select(id3)) == NULL)
            {
                puts("课程不存在！");
                puts("输入将要修改的课程ID：");
                scanf("%d", &id3);
            }
            int new_id3 = 0;
            while (1)
            {
                puts("输入课程的新ID（0表示不修改）：");
                scanf("%d", &new_id3);
                if (course_select(new_id3) != NULL)
                    puts("课程ID已存在！");
                else if (new_id3 < 0)
                    puts("课程ID不合法！");
                else if (new_id3 == 0)
                    break;
                else
                {
                    c3->id = new_id3;
                    break;
                }
            }

            char name3[1024] = {0};
            puts("输入新的课程名称（留空表示不修改）：");
            scanf("%s", name3);
            if (strlen(name3) != 0)
                c3->name = name3;

            char type3[1024] = {0};
            puts("输入课程性质（留空表示不修改）：");
            scanf("%s", type3);
            if (strlen(type3) != 0)
                c3->type = type3;

            int hours3 = -1;
            puts("输入课程学时（负数表示不修改）：");
            scanf("%d", &hours3);
            if (hours3 >= 0)
                c3->hours = hours3;

            double credit3 = -1.0;
            puts("输入课程学分（负数表示不修改）：");
            scanf("%f", &credit3);
            if (credit3 >= 0)
                c3->credit = credit3;

            puts("修改中...");
            char sql[1024] = {0};
            
            if (1 == course_edit(id3, c3))
                puts("修改成功！");
            else
                puts("修改失败！");
        case '4':
            entry_course *c4 = course_select_all();
            puts(SPLINE_2);
            while (c4)
            {
                printf("%s(%d) %s %d %.1f %d", c4->name, c4->id, c4->type, c4->hours, c4->credit, c4->students_limit);
                c4 = c4->next;
            }
            break;
        }
    }
}