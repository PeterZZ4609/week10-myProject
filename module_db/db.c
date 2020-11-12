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

int open_db(const char *db_path)
{
    if (NULL == sql_db)
    {
        return sqlite3_open(db_path, &sql_db);
    }
    return SQLITE_OK;
}

int init_table()
{
    printf("Data tables initializing...\n");
    int ret = 0;
    char sql[1024] = {0};
    strcpy(sql, "create table tb_student( id integer primary key , name text not null );");
    ret += sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
    strcpy(sql, "create table tb_course( id integer primary key, name text not null, type text not null, hours integer, credit integer, students_limit integer);");
    ret += sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
    strcpy(sql, "create table tb_option( stu_id integer , course_id integer )");
    ret += sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
    printf("Data tables initialized. Returned %d.\n", ret);
    return ret; // If not 0, failed
}

int insert_student(int id, const char *name)
{
    // well, I dont know what is sql injection.
    char sql[1024] = {0};
    char tmp[20] = {0};
    // itoa(id, tmp, 10); // itoa is not in C89
    sprintf(tmp, "%d", id);
    strcat(sql, "insert into tb_student values(");
    strcat(sql, tmp);
    strcat(sql, ", '");
    strcat(sql, name);
    strcat(sql, "');");
    return sqlite3_exec(sql_db, sql, NULL, NULL, NULL);
}

// s=column count,
int student_callback(void *ret, int s, char **data, char **column_name)
{
    // append this result to the ret
    ret_student *now = (ret_student *)ret;
    while (now->next != NULL)
    {
        now = now->next;
    }
    ret_student *res = (ret_student *)malloc(sizeof(ret_student));
    res->next = NULL;
    res->id = atoi(data[0]);
    res->name = data[1]; // TODO May need strcpy()
    now->next = res;
    return 0; // continue; if non-zero, sqlite3_exec() will not go on
}

static ret_student *select_student()
{
    ret_student *ret = (ret_student *)malloc(sizeof(ret_student)); // It is linking table's head without data
    ret->next = NULL;
    char sql[1024] = "select * from tb_student;";
    char **errmsg;
    if (sqlite3_exec(sql_db, sql, student_callback, ret, NULL) != 0)
    {
        // fprintf(stderr, errmsg);
        return NULL;
    }
    return ret->next;
}

void close_db()
{
    if (NULL != sql_db)
        sqlite3_close(sql_db);
}

int main(void)
{
    open_db("./test.db");

    init_table();

    insert_student(12, "Peter");
    insert_student(13, "Annie");
    insert_student(14, "Johnson");
    insert_student(15, "Linus");
    insert_student(12, "PeterZhang"); // Must fail

    ret_student *ret = select_student();
    printf("select_student():\n");
    while (ret != NULL)
    {
        printf("%d: %s\n", ret->id, ret->name);
        ret = ret->next;
    }
    return 0;
}