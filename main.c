#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sqlite3.h>
#include "strings_.h"
#include "$.h"
#include "modele_records.h"
#include "module_admin.h"
#include "module_user.h"
void logIn(sqlite3 *);
void main(){
    while(1){
        title_common;
        system("cls");
        puts("Hey there!");
        puts("Press 1 to login | ESC - Exit");
        sqlite3 *db;
        switch (_getch()){

        case $key_1:{
            logIn(db);
            break;
        }
        
        case $key_esc:{
            sqlite3_close_v2(db);
            return;
        }
        default:
            break;
        }
    }
}   
void logIn(sqlite3 *db){
    switch(sqlite3_open_v2(DATABASE_FILE_NAME, &db, SQLITE_OPEN_READWRITE, NULL)){
        case SQLITE_OK:{
            break;
        }
        case SQLITE_CANTOPEN:{
            puts(NO_DATABASE_FILE);
            puts(EXIT_MSG);
            _getch();
            return;
        }
    }
    foreignKeysOn(db);
    bool log_status = false;
    char tmp_login[20];
    char tmp_passwd[20];
    sqlite3_stmt *res;
    char *sql_logging = "SELECT user_id, role FROM table_list_users WHERE login = ?  AND passwd = ?";
    puts("Enter login:");
    scanf("%20s[^\n]", tmp_login);
    puts("Enter password:");
    scanf("%20s[^\n]", tmp_passwd);
    if (sqlite3_prepare_v2(db, sql_logging, -1, &res, 0) == SQLITE_OK){
        sqlite3_bind_text(res, 1, tmp_login, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, tmp_passwd, -1, SQLITE_STATIC);
        if(sqlite3_step(res) == SQLITE_ROW){
            if(strcmp("Admin", sqlite3_column_text(res, 1))==0){
                log_status=true;
                adminMenu(db);
            }
            if(strcmp("User", sqlite3_column_text(res, 1))==0){
                uint64_t user_id = sqlite3_column_int64(res, 0);
                log_status=true;
                userMenu(user_id, db);
            }
        }
        if(log_status==false){
            puts("Log issue! Try again\n");
            getch();
        }
    } else  fprintf(ERROR_FROM_DB);
    sqlite3_finalize(res);
}

// main(): приветствие, указание номера клавиши и что она делает( т.к все действия в программк реализованы посредством конструкции switch(getch())). После нажатия клавиши вызывается функция login.
// login(): открывается файл с sql бд, и пользователь вводит данные. Если данные коректны, то на основе роли пользователя вызывается соот меню(админ/юзер).
// Далее все вызовы функций работы с данными реализованы посредством ранее названной конструкции.
// module_admin.h содержит логику функции работы с учетными записями пользователей (стандартый CRUD) и вызовы подменю работы с учетками юзеров и их записями о выходных или больниных.
// module_user.h содержит просмотра учетной пользователя.
// sick_and_off.h - логика работы с данными о выходных/больничных.
// $.h 