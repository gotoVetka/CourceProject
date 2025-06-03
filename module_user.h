void userPrint(sqlite3 *db, uint64_t user_id){
    sqlite3_stmt *res;
    char *sql_user_print_user= "SELECT user_id, name, surname, second_name, hours, lateness FROM table_list_users WHERE user_id = ?";
    if (sqlite3_prepare_v2(db, sql_user_print_user, -1, &res, 0) == SQLITE_OK) {
        sqlite3_bind_int64(res, 1, user_id);
        if (sqlite3_step(res) == SQLITE_ROW) 
        {
            printf("\nId: %ju\n", sqlite3_column_int64(res, 0));
            printf("\nName: %s\n", sqlite3_column_text(res, 1));
            printf("\nSurname: %s\n ", sqlite3_column_text(res, 2));
            printf("\nSecond name: %s\n", sqlite3_column_text(res, 3));
            printf("\nHours: %d\n", sqlite3_column_int(res, 4));
            printf("\nLateness: %d\n", sqlite3_column_int(res, 5));
        }
    }
    sqlite3_finalize(res);
}
void sortUserRecordsCommand(uint64_t user_id, char *sql_command, sqlite3 *db){
    sqlite3_stmt *res;
    if(sqlite3_prepare_v2(db, sql_command, -1, &res, 0) == SQLITE_OK){
        sqlite3_bind_int64(res, 1, user_id);
        printf("%-20s | %-20s | %-20s | %-20s | %-20s", "Record id", "User id", "Type", "Start date", "End date");
        while(sqlite3_step(res) == SQLITE_ROW)
        {
            printf("\n%-20ju | ", sqlite3_column_int64(res, 0));
            printf("%-20ju | ", sqlite3_column_int64(res, 1));
            printf("%-20s | ", sqlite3_column_text(res, 2));
            printf("%-20s | ", sqlite3_column_text(res, 3));
            printf("%-20s", sqlite3_column_text(res, 4));
            
        }
    } else {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(res);
}
void sortUserRecordsMenu(uint64_t current_user_id, sqlite3 *db){
    puts("1 - Start | 2 - End | 3 - Type");
    switch(getch()){
            case $key_1:{
                puts(SORT_START);
                sortUserRecordsCommand(current_user_id, "SELECT * FROM table_users_sick_and_off  WHERE user_id = ? ORDER BY start DESC", db);
                break;
            }
            case $key_2:{
                puts(SORT_END);
                sortUserRecordsCommand(current_user_id, "SELECT * FROM table_users_sick_and_off  WHERE user_id = ? ORDER BY end DESC", db);
                break;
            }
            case $key_3:{
                puts(SORT_TYPE);
                sortUserRecordsCommand(current_user_id, "SELECT * FROM table_users_sick_and_off  WHERE user_id = ? ORDER BY type DESC", db);
                break;
            default:{
                puts(UNKNOWN_COMMAND);
                break;
            }
        }
    }
}
void userUpdate(uint64_t current_id, sqlite3 *db){
    char *sql_update_user = 0;
    int tmp_int;
    char tmp_string[20];
    system("cls");
    userPrint(db, current_id);
    puts("1 - Name | 2 - Surname | 3 - Second name | ESC - Exit");
    switch(getch()){
        case $key_1:{
            puts("Enter new name:");
            scanf("%20s[^\n]", tmp_string);
            updateUserByParamText(db, "UPDATE table_list_users SET name = ? WHERE user_id = ?", tmp_string, current_id);
            break;
        }
        case $key_2:{
            puts("Enter new surname:");
            scanf("%20s[^\n]", tmp_string);
            updateUserByParamText(db, "UPDATE table_list_users SET surname = ? WHERE user_id = ?", tmp_string, current_id);
            break;
        }
        case $key_3:{
            puts("Enter new second name:");
            scanf("%20s[^\n]", tmp_string);
            updateUserByParamText(db, "UPDATE table_list_users SET second_name = ? WHERE user_id = ?", tmp_string, current_id);
            break;
        }
        case $key_esc:{
            break;
        }
        default:{
            puts("Invalid operation");
            break;
        }
    }
}

void userMenu(uint64_t current_user_id, sqlite3 *db){
    bool continue_menu_flag=true;
    while(continue_menu_flag){
        title_user;
        system("cls");
        printf("Hello, %ju!\n", current_user_id);
        puts("1 - Update my profile\n2 - My profile\n3 - Print all my records\n4 - Sort my records\nEsc - Log out");
        switch(getch()){
            case $key_1:{
                title_user_update;
                userUpdate(current_user_id, db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_2:{
                title_user_read;
                system("cls");
                readUser(db, current_user_id);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_3:{
                title_user_read;
                system("cls");
                printAllRecordsByUserId(db, current_user_id);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_4:{
                title_user_sort;
                system("cls");
                sortUserRecordsMenu(current_user_id, db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_esc:{
                sqlite3_close(db);
                continue_menu_flag = false;
                break;
            }
            default:{
                break;
            }
            
        }
    }
}