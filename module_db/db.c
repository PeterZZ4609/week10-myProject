/* 
 * create db file, open database, initialize database
 * 
 * Author: PeterZhang
 * History: 2020/11/12 v1
*/

#include <dlfcn.h>
#include <pthread.h>
#include "sqlite3.h"
#include "db.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static sqlite3 *sql_db = NULL;

int db_open(const char *db_path)
{
    if (NULL == sql_db)
    {
        return sqlite3_open(db_path, &sql_db);
    }
    return SQLITE_OK;
}

int db_init_table()
{
    printf("Data tables initializing...\n");
    int ret = 0;
    char sql[1024] = {0};
    sprintf(sql, "create table if not exists tb_student( id integer primary key , name text not null );");
    ret += sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
    sprintf(sql, "create table if not exists tb_course( id integer primary key, name text not null, type text not null, hours integer, credit integer, students_limit integer);");
    ret += sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
    sprintf(sql, "create table if not exists tb_selection( stu_id integer , course_id integer )");
    ret += sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
    printf("Data tables initialized. Returned %d.\n", ret);
    return ret; // If not 0, failed
}

static entry_course *new_entry_course(int id, char *name, char *type, int hours, double credit, int students_limit)
{
    entry_course *c = (entry_course *)malloc(sizeof(entry_course));
    c->next = NULL;

    c->id = id;
    c->name = name;
    char *tmp = malloc(strlen(name) + 1);
    strcpy(tmp, name);
    c->name = tmp;
    tmp = malloc(strlen(type));
    strcpy(tmp, type);
    c->type = tmp;
    tmp = NULL;
    c->hours = hours;
    c->credit = credit;
    c->students_limit = students_limit;
    return c;
}

static entry_selection *new_entry_selection(int stu_id, int course_id)
{
    entry_selection *s = (entry_selection *)malloc(sizeof(entry_selection));
    s->next = NULL;
    s->stu_id = stu_id;
    s->course_id = course_id;
    return s;
}

int db_init_mockdata()
{
    student_insert(12, "张三");
    student_insert(13, "李丢丢");
    student_insert(14, "王麻麻");
    student_insert(16, "赵子云");
    student_insert(18, "宋捶捶");
    course_insert(new_entry_course(45, "操作系统", "公选课", 32, 2, 120));
    course_insert(new_entry_course(48, "计算机网络基础", "限选课", 48, 4.5, 60));
    course_insert(new_entry_course(129, "C语言现代方法", "选修课", 32, 3.5, 120));
    selection_insert(new_entry_selection(12, 48));
    selection_insert(new_entry_selection(12, 45));
    selection_insert(new_entry_selection(13, 45));
    selection_insert(new_entry_selection(13, 129));
    selection_insert(new_entry_selection(14, 45));
    selection_insert(new_entry_selection(16, 129));
    selection_insert(new_entry_selection(18, 45));
    selection_insert(new_entry_selection(18, 48));
    selection_insert(new_entry_selection(18, 129));

    return 1;
}

/**
 * Operations API for tb_student.
 */

/**
 * Retuen 1 if success
 */
int student_insert(int id, const char *name)
{
    // well, I dont know what is sql injection.
    char sql[1024] = {0};
    sprintf(sql, "insert into tb_student values(%d,'%s');", id, name);
    // itoa(id, tmp, 10); // itoa is not in C89
    return SQLITE_OK == sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

// s=column count,
static int student_query_callback(void *ret, int s, char **data, char **column_name)
{
    // append this result to the ret
    entry_student *now = (entry_student *)ret;
    while (now->next != NULL)
    {
        now = now->next;
    }
    entry_student *res = (entry_student *)malloc(sizeof(entry_student));
    res->next = NULL;
    res->id = atoi(data[0]);
    res->name = data[1]; // TODO May need strcpy()
    char *name = (char *)malloc(sizeof(char) * strlen(data[1]) + 1);
    strcpy(name, data[1]);
    res->name = name;
    now->next = res;
    return 0; // continue; if non-zero, sqlite3_exec() will not go on
}

entry_student *student_select()
{
    entry_student *ret = (entry_student *)malloc(sizeof(entry_student)); // It is linking table's head without data
    ret->next = NULL;
    char sql[1024] = "select * from tb_student;";
    char **errmsg;
    if (SQLITE_OK != sqlite3_exec(sql_db, sql, student_query_callback, ret, NULL))
    {
        fprintf(stderr, "%s\n", *errmsg);
        return NULL;
    }
    return ret->next; // need free()
}

entry_student *student_select_by_id(int id)
{
    entry_student *ret = (entry_student *)malloc(sizeof(entry_student)); // It is linking table's head without data
    ret->next = NULL;
    char sql[1024];
    sprintf(sql, "select * from tb_student where id=%d;", id);
    char **errmsg;
    if (SQLITE_OK != sqlite3_exec(sql_db, sql, student_query_callback, ret, NULL))
    {
        fprintf(stderr, "%s\n", *errmsg);
        return NULL;
    }
    return ret->next; // need free()
}

/**
 * Return 1 if success
 */
int student_edit(int id, entry_student *data)
{
    char sql[1024] = "update tb_student set id=";
    sprintf(sql, "update tb_student set id=%d, name='%s' where id=%d;", data->id, data->name, id);
    return SQLITE_OK == sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

/**
 * Return 1 if success
 */
int student_delete(int id)
{
    char sql[1024] = {0};
    sprintf(sql, "delete from tb_student where id=%d;", id);
    return SQLITE_OK == sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

/**
 * Operations API for tb_course.
 */

/**
 * Return 1 if success
 */
int course_insert(entry_course *course)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into tb_course values(%d, '%s', '%s', %d, %.1f, %d);",
            course->id, course->name, course->type, course->hours, course->credit, course->students_limit);
    return SQLITE_OK == sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

int course_delete(int id)
{
    char sql[1024] = {0};
    sprintf(sql, "delete from tb_course where id=%d;", id);
    return SQLITE_OK == sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

int course_edit(int id, entry_course *course)
{
    char sql[1024] = {0};
    // id may not exists
    sprintf(sql, "update tb_course set id=%d, name='%s', type='%s', hours=%d, credit=%.1f, students_limit=%d where id=%d;",
            course->id, course->name, course->type, course->hours, course->credit, course->students_limit, id);
    return SQLITE_OK == sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

static int course_query_callback(void *ret, int s, char **data, char **column_names)
{
    entry_course *now = (entry_course *)ret;
    while (now->next != NULL)
        now = now->next;
    entry_course *res = (entry_course *)malloc(sizeof(entry_course));
    res->next = NULL;
    // columns may not stay the right order
    int i = 0;
    while (i < s)
    {
        if (strcmp(column_names[i], "id") == 0)
            res->id = atoi(data[i]);
        else if (strcmp(column_names[i], "name") == 0)
        {
            char *name = malloc(1024);
            strcpy(name, data[i]);
            res->name = name;
        }
        else if (strcmp(column_names[i], "type") == 0)
        {
            char *type = malloc(1024);
            strcpy(type, data[i]);
            res->type = type;
        }
        else if (strcmp(column_names[i], "hours") == 0)
            res->hours = atoi(data[i]);
        else if (strcmp(column_names[i], "credit") == 0)
            res->credit = atof(data[i]);
        else if (strcmp(column_names[i], "students_limit") == 0)
            res->students_limit = atoi(data[i]);
        ++i;
    }
    now->next = res;
    return 0;
}

entry_course *course_select(int id)
{
    entry_course *ret = (entry_course *)malloc(sizeof(entry_course));
    ret->next = NULL;
    char sql[1024] = {0};
    sprintf(sql, "select * from tb_course where id=%d;", id);
    // char **errmsg;
    if (SQLITE_OK != sqlite3_exec(sql_db, sql, course_query_callback, ret, NULL))
    {
        // fprintf(stderr, "%s\n", *errmsg);
        return NULL;
    }
    return ret->next; // TODO need free()
}

entry_course *course_select_all()
{
    entry_course *res = (entry_course *)malloc(sizeof(entry_course));
    res->next = NULL;
    char sql[1024] = "select * from tb_course;";
    // char *errmsg = (char *)malloc(sizeof(char) * 1024);
    if (SQLITE_OK != sqlite3_exec(sql_db, sql, course_query_callback, res, NULL))
    {
        // fprintf(stderr, "Error: %s\n", errmsg);
        return NULL;
    }
    return res->next; // TODO need free()
}

/**
 * Operations API for courses selections.
 */

int selection_insert(entry_selection *selection)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into tb_selection values(%d, %d);", selection->stu_id, selection->course_id);
    return SQLITE_OK == sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

int selection_delete(int stu_id, int course_id)
{
    char sql[1024] = {0};
    sprintf(sql, "delete from tb_selection where stu_id=%d and course_id=%d;", stu_id, course_id);
    return SQLITE_OK == sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

static int selection_query_callback(void *ret, int s, char **data, char **column_names)
{
    entry_selection *res = (entry_selection *)malloc(sizeof(entry_selection));
    res->next = NULL;
    res->stu_id = atoi(data[0]);
    res->course_id = atoi(data[1]);
    entry_selection *now = (entry_selection *)ret;
    while (now->next)
        now = now->next;
    now->next = res;
    return 0;
}

static entry_selection *selection_find(int id, const char *type)
{
    char sql[1024] = {0};
    entry_selection *res = (entry_selection *)malloc(sizeof(entry_selection));
    res->next = NULL;
    sprintf(sql, "select * from tb_selection where %s_id=%d;", type, id);
    // char **errmsg;
    if (SQLITE_OK != sqlite3_exec(sql_db, sql, selection_query_callback, res, NULL))
    {
        // fprintf(stderr, "%s\n", *errmsg);
        return NULL;
    }
    return res->next; // TODO need free()
}

entry_selection *selection_find_course(int course_id)
{
    return selection_find(course_id, "course");
}

entry_selection *selection_find_student(int stu_id)
{
    return selection_find(stu_id, "stu");
}

void db_close()
{
    if (NULL != sql_db)
        sqlite3_close(sql_db);
}

int main2(void)
{
    db_open("./test.db");

    db_init_table();

    db_init_mockdata();

    db_close();

    return 0;
}