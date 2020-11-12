//
// Created by peter on 2020/11/12.
//

#ifndef MYPROJECT_DB_H
#define MYPROJECT_DB_H

typedef struct ret_student
{
    int id;
    char *name;
    struct ret_student *next;
} ret_student;

int open_db(const char *db_path);
void close_db();

#endif //MYPROJECT_DB_H
