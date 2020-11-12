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
    if (sqlite3_exec(sql_db, sql, student_query_callback, ret, NULL) != 0)
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
    char sql[2014] = {0};
    sprintf(sql, "insert into tb_course values(%d, '%s', '%s', %d, %d, %d);",
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
    sprintf(sql, "update tb_course set id=%d, name='%s', type='%s', hours=%d, credit=%d, students_limit=%d;",
            course->id, course->name, course->type, course->hours, course->credit, course->students_limit);
    return SQLITE_OK == sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

static int course_query_callback(void *ret, int s, char **data, char **column_names)
{
    entry_course *res = (entry_course *)malloc(sizeof(entry_course));
    res->next = NULL;
    res->id = atoi(data[0]);
    // columns may not stay the right order
    int i = 0;
    while (i < s)
    {
        if (strcmp(column_names[i], "id"))
            res->id = atoi(data[i]);
        else if (strcmp(column_names[i], "name"))
            res->name = data[i];
        else if (strcmp(column_names[i], "type"))
            res->type = data[i];
        else if (strcmp(column_names[i], "hours"))
            res->hours = atoi(data[i]);
        else if (strcmp(column_names[i], "credit"))
            res->credit = atoi(data[i]);
        else if (strcmp(column_names[i], "students_limit"))
            res->students_limit = atoi(data[i]);
        ++i;
    }
    ((entry_course *)ret)->next = res;
    return 0;
}

entry_course *course_select(int id)
{
    entry_course *ret = (entry_course *)malloc(sizeof(entry_course));
    ret->next = NULL;
    char sql[1024] = {0};
    sprintf(sql, "select * from tb_course where id=%d;", id);
    char **errmsg;
    if (SQLITE_OK != sqlite3_exec(sql_db, sql, course_query_callback, ret, errmsg))
    {
        fprintf(stderr, "%s\n", *errmsg);
        return NULL;
    }
    return ret->next; // TODO need free()
}

entry_course *course_select_all()
{
    entry_course *ret = (entry_course *)malloc(sizeof(entry_course));
    ret->next = NULL;
    char sql[1024] = {0};
    sprintf(sql, "select * from tb_course;");
    char **errmsg;
    if (SQLITE_OK != sqlite3_exec(sql_db, sql, course_query_callback, ret, errmsg))
    {
        fprintf(stderr, "%s\n", *errmsg);
        return NULL;
    }
    return ret->next; // TODO need free()
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

int selection_query_callback(void *ret, int s, char **data, char **column_names)
{
    entry_selection *res = (entry_selection *)malloc(sizeof(entry_selection));
    res->next = NULL;
    res->stu_id = atoi(data[0]);
    res->course_id = atoi(data[1]);
    ((entry_selection *)ret)->next = res;
    return 0;
}

entry_selection *selection_find_course(int course_id)
{
    char sql[1024] = {0};
    entry_selection *res = (entry_selection *)malloc(sizeof(entry_selection));
    res->next = NULL;
    sprintf(sql, "select * from tb_selection where course_id=%d;", course_id);
    char **errmsg;
    if (SQLITE_OK != sqlite3_exec(sql_db, sql, selection_query_callback, res, errmsg))
    {
        fprintf(stderr, "%s\n", *errmsg);
        return NULL;
    }
    return res->next; // TODO need free()
}

void db_close()
{
    if (NULL != sql_db)
        sqlite3_close(sql_db);
}

int main(void)
{
    db_open("./test.db");

    db_init_table();
    puts("");

    puts("Insert students...");
    student_insert(12, "Peter");
    student_insert(13, "Annie");
    student_insert(14, "Johnson");
    student_insert(15, "Linus");
    student_insert(12, "PeterZhang"); // Must fail
    puts("");

    entry_student *ret = student_select();
    printf("select_student():\n");
    while (ret != NULL)
    {
        printf("%d: %s (%lu)\n", ret->id, ret->name, strlen(ret->name));
        ret = ret->next;
    }
    puts("");

    entry_student a;
    a.id = 88;
    a.name = "PeterZhang";
    a.next = NULL;

    printf("%d\n", student_edit(12, &a));
    ret = student_select();
    while (ret != NULL)
    {
        printf("%d: %s (%lu)\n", ret->id, ret->name, strlen(ret->name));
        ret = ret->next;
    }
    puts("");

    printf("%d\n", student_delete(88));
    ret = student_select();
    while (ret != NULL)
    {
        printf("%d: %s (%lu)\n", ret->id, ret->name, strlen(ret->name));
        ret = ret->next;
    }

    return 0;
}