void printAllIdUsers(sqlite3 *db){
    system("cls");
    sqlite3_stmt *print_call;
    char *sql_print_id_all_users = "SELECT user_id, login FROM table_list_users"; 
    if (sqlite3_prepare_v2(db, sql_print_id_all_users, -1, &print_call, 0) == SQLITE_OK) 
    {
        while (sqlite3_step(print_call) == SQLITE_ROW) 
        {
            printf("User ID: %-20ju | User Login: %-20s\n", sqlite3_column_int64(print_call, 0), sqlite3_column_text(print_call, 1));
        } 
    }
    sqlite3_finalize(print_call);
}
void readUser(sqlite3 *db, uint64_t id){
    sqlite3_stmt *res;
    char *sql_admin_print_user;
    sql_admin_print_user = "SELECT * FROM table_list_users WHERE user_id = ?";
    if (sqlite3_prepare_v2(db, sql_admin_print_user, -1, &res, 0) == SQLITE_OK) 
    {
        sqlite3_bind_int64(res, 1, id);
        if (sqlite3_step(res) == SQLITE_ROW) 
        {
            printf("\nUser Id: %ju\n", sqlite3_column_int64(res, 0));
            printf("Login: %s\n", sqlite3_column_text(res, 1));
            printf("Password: %s\n", sqlite3_column_text(res, 2));
            printf("Role: %s\n", sqlite3_column_text(res, 3));
            printf("Name: %s\n", sqlite3_column_text(res, 4));
            printf("Surname: %s\n", sqlite3_column_text(res, 5));
            printf("Second name: %s\n", sqlite3_column_text(res, 6));
            printf("Hours: %d\n", sqlite3_column_int(res, 7));
            printf("Lateness: %d\n", sqlite3_column_int(res, 8));
        }
    }
    sqlite3_finalize(res);
}
void createUser(sqlite3 *db){
    system("cls");
    sqlite3_stmt *res;
    RECORD_USER p;
    unsigned short w=1, c=1;
    while(w){
        puts("Enter id");
        while(!scanf("%ju", &p.id))scanf("%*[^\n]");
        puts("Enter login");
        scanf("%20s[^\n]", p.login);
        puts("Enter password");
        scanf("%20s[^\n]", p.passwd);
        p.role = "User";
        puts("Enter name");
        scanf("%20s[^\n]", p.name);
        puts("Enter surname");
        scanf("%20s[^\n]", p.surname);
        puts("Enter second name");
        scanf("%20s[^\n]", p.second_name);
        puts("Enter hours");
        while(!scanf("%d", &p.hours))scanf("%*[^\n]");
        puts("Enter number of lateness");
        while(!scanf("%d", &p.lateness))scanf("%*[^\n]");
        w=1;
        system("cls");
        printf("ID: %ju\nLogin: %s\nPassword: %s\nRole: %s\nName: %s\nSurname: %s\nSecond Name: %s\nHours: %d\nLateness: %d", p.id, p.login, p.passwd, p.role, p.name, p.surname, p.second_name, p.hours, p.lateness);
        puts("\nIs that correct data?\n1 - Yes\n2 - No\n3 - Exit");
        switch(getch()){
            case $key_1:{
                w=0;
                break;
            }
            case $key_2:{
                system("cls");
                break;
            }
            case $key_3:{
                return;
            }
            default:{
                return;
            }
        }
    }
    if(c){
        char *sql_create_user = "INSERT INTO table_list_users  (user_id, login, passwd, role, name, surname, second_name, hours, lateness) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        if (sqlite3_prepare_v2(db, sql_create_user, -1, &res, 0) == SQLITE_OK){
            sqlite3_bind_int64(res, 1, p.id);
            sqlite3_bind_text(res, 2, p.login, -1, SQLITE_STATIC);
            sqlite3_bind_text(res, 3, p.passwd, -1, SQLITE_STATIC);
            sqlite3_bind_text(res, 4, p.role, -1, SQLITE_STATIC);
            sqlite3_bind_text(res, 5, p.name, -1, SQLITE_STATIC);
            sqlite3_bind_text(res, 6, p.surname, -1, SQLITE_STATIC);
            sqlite3_bind_text(res, 7, p.second_name, -1, SQLITE_STATIC);
            sqlite3_bind_int(res, 8, p.hours);
            sqlite3_bind_int(res, 9, p.lateness);
            if (sqlite3_step(res) == SQLITE_DONE) 
            {
                printf("Profile created successfully.\n");
            } else fprintf(stderr, "Error! Something gone wrong\n : %s", sqlite3_errmsg(db));
        } else {
            fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        }
    }
    sqlite3_finalize(res);
}
void printAllUsers(sqlite3 *db){
    system("cls");
    sqlite3_stmt *res;
    char *sql_print_all_users = "SELECT user_id, name, surname, second_name, hours, lateness FROM table_list_users WHERE role = 'User'"; 
    if (sqlite3_prepare_v2(db, sql_print_all_users, -1, &res, 0) == SQLITE_OK) 
    {
        printf("%-20s | %-20s | %-20s | %-20s | %-20s | %-20s\n", "User ID", "Name", "Surname", "Second name", "Hours", "Lateness");
        while (sqlite3_step(res) == SQLITE_ROW) 
        {
            printf("\n%-20ju | ", sqlite3_column_int64(res, 0));
            printf("%-20s | ", sqlite3_column_text(res, 1));
            printf("%-20s | ", sqlite3_column_text(res, 2));
            printf("%-20s | ", sqlite3_column_text(res, 3));
            printf("%-20d | ", sqlite3_column_int(res, 4));
            printf("%-20d", sqlite3_column_int(res, 5));
        }
    } else {
        // выводим сообщение об ошибке
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(res);
}
void adminUpdate(sqlite3 *db){
    char *sql_update_user;
    char tmp_string[20]; 
    char *role;
    int tmp_int;
    uint64_t tmp_update_id, tmp_int64;;
    printAllIdUsers(db);
    puts("Enter user id:");
    while(!scanf("%ju", &tmp_update_id))scanf("%*[^\n]");
    if(isUserExists(tmp_update_id, db)){
        readUser(db, tmp_update_id);
        puts("\n1 - User Id | 2 - Login | 3 - Password | 4 - Role | 5 - Name |  6 - Surname | 7 - Surname | 8 - Hours | 9 - Lateness");
        switch(_getch()){
            case $key_1:{
                puts("Enter new id:");
                while(!scanf("%ju", &tmp_int64))scanf("%*[^\n]");
                sql_update_user = "UPDATE table_list_users SET user_id = ? WHERE user_id = ?;";
                updateUserByParamInt64(db, sql_update_user, tmp_int64, tmp_update_id);
                break;
            }
            case $key_2:{
                puts("Enter new login:");
                scanf("%20s[^\n]", tmp_string);
                sql_update_user = "UPDATE table_list_users SET login = ? WHERE user_id = ?";
                updateUserByParamText(db, sql_update_user, tmp_string,tmp_update_id);
                break;
            }
            case $key_3:{
                puts("Enter new password:");
                scanf("%20s[^\n]", tmp_string);
                sql_update_user = "UPDATE table_list_users SET password = ? WHERE user_id = ?";
                updateUserByParamText(db, sql_update_user, tmp_string,tmp_update_id);
            }
            case $key_4:{
                bool is_valid_command;
                puts("Select user role:\n1 - Admin | 2 - User");
                switch (getch()){
                    case $key_1:{
                        role = "Admin";
                        is_valid_command=true;
                        break;
                    }
                    case $key_2:{
                        role = "User";
                        is_valid_command=true;
                        break;
                    }
                    default:{
                        is_valid_command = false;
                        break;
                    }
                }
                if(is_valid_command){
                    puts(role);
                    sql_update_user = "UPDATE table_list_users SET role = ? WHERE user_id = ?";
                    updateUserByParamText(db, sql_update_user, role, tmp_update_id);
                } else {
                    puts(UNKNOWN_COMMAND);
                } 
                break;
            }
            case $key_5:{
                puts("Enter new name:");
                scanf("%20s[^\n]", tmp_string);
                sql_update_user = "UPDATE table_list_users SET name = ? WHERE user_id = ?";
                updateUserByParamText(db, sql_update_user, tmp_string, tmp_update_id);
                break;
            }
            case $key_6:{
                puts("Enter new surname:");
                scanf("%20s[^\n]", tmp_string);
                sql_update_user = "UPDATE table_list_users SET surname = ? WHERE user_id = ?";
                updateUserByParamText(db, sql_update_user, tmp_string, tmp_update_id);
                break;
            }
            case $key_7:{
                puts("Enter new second name:");
                scanf("%20s[^\n]", tmp_string);
                sql_update_user = "UPDATE table_list_users SET second_name = ? WHERE user_id = ?";
                updateUserByParamText(db, sql_update_user, tmp_string, tmp_update_id);
                break;
            }
            case $key_8:{
                puts("Enter new hours:");
                while(!scanf("%d", &tmp_int))scanf("%^[\n]");
                sql_update_user = "UPDATE table_list_users SET hours = ? WHERE user_id = ?";
                updateUserByParamInt(db, sql_update_user, tmp_int, tmp_update_id);
                break;
            }
            case $key_9:{
                puts("Enter new number of lateness:");
                while(!scanf("%d", &tmp_int))scanf("%^[\n]");
                sql_update_user = "UPDATE table_list_users SET lateness = ? WHERE user_id = ?";
                updateUserByParamInt(db, sql_update_user, tmp_int, tmp_update_id);
                break;
            }
            default:{
                puts(UNKNOWN_COMMAND);
                break;
            }
        }
    } else puts(NO_ID);
}
void deleteUser(sqlite3 *db){
    system("cls");
    uint64_t tmp_id;
    printAllIdUsers(db);
    puts("Enter user's id:");
    scanf("%ju", &tmp_id);
    if(isUserExists(tmp_id, db)){
        puts("Are you sure ? 1 - Yes");
        if(getch()!=$key_1){
            return;
        }
        sqlite3_stmt *res;
        char *sql_delete_user = "DELETE FROM table_list_users WHERE user_id = ?"; 
        if(sqlite3_prepare_v2(db, sql_delete_user, -1, &res, 0) == SQLITE_OK){
            sqlite3_bind_int64(res, 1, tmp_id);
            if(sqlite3_step(res) == SQLITE_DONE){
                puts(DELETE_SUCCESS);
            } else fprintf(ERROR_FROM_DB);
        }
        sqlite3_finalize(res);    
    } else puts(NO_ID);
}
void sortUsersCommand(sqlite3 *db, char *sql_command){
    sqlite3_stmt *res;
    if(sqlite3_prepare_v2(db, sql_command, -1, &res, 0) == SQLITE_OK){
        while(sqlite3_step(res) == SQLITE_ROW)
        {
            printf("\nUser Id: %-20ju | ", sqlite3_column_int64(res, 0));
            printf("Login: %-20s | ", sqlite3_column_text(res, 1));
            printf("Password: %-20s | ", sqlite3_column_text(res, 2));
            printf("Role: %-5s | ", sqlite3_column_text(res, 3));
            printf("Name: %-20s | ", sqlite3_column_text(res, 4));
            printf("Surname: %-20s | ", sqlite3_column_text(res, 5));
            printf("Second name: %-20s ", sqlite3_column_text(res, 6));
            printf("Hours: %-10d | ", sqlite3_column_int(res, 7));
            printf("Lateness: %-10d\n", sqlite3_column_int(res, 8));
        }
    } else {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(res);
}
void sortUsersASC(sqlite3 *db){
    puts("1 - User id | 2 - Hours | 3 - Lateness");
    switch(getch()){
        case $key_1:{
            puts(SORT_USER_ID);
            sortUsersCommand(db, "SELECT * FROM table_list_users ORDER BY user_id ASC");
            break;
        }
        case $key_2:{
            puts(SORT_HOURS);
            sortUsersCommand(db ,"SELECT * FROM table_list_users ORDER BY hours ASC");
            break;
        }
        case $key_3:{
            puts(SORT_LATENESS);
            sortUsersCommand(db, "SELECT * FROM table_list_users ORDER BY lateness ASC");
            break;
        }
        default:{
            puts(UNKNOWN_COMMAND);
            break;
        }
    }
}
void sortUsersDESC(sqlite3 *db){
    puts("1 - User id | 2 - Hours | 3 - Lateness");
    switch(getch()){
        case $key_1:{
            puts(SORT_USER_ID);
            sortUsersCommand(db, "SELECT * FROM table_list_users ORDER BY user_id DESC");
            break;
        }
        case $key_2:{
            puts(SORT_HOURS);
            sortUsersCommand(db ,"SELECT * FROM table_list_users ORDER BY hours DESC");
            break;
        }
        case $key_3:{
            puts(SORT_LATENESS);
            sortUsersCommand(db, "SELECT * FROM table_list_users ORDER BY lateness DESC");
            break;
        }
        default:{
            puts(UNKNOWN_COMMAND);
            break;
        }
    }
}
void sortUsersMenu(sqlite3 *db){
    system("cls");
    puts("Sort records by:\n1 - Ascending | 2 - Descending");
    switch(getch()){
        case $key_1:{
            puts(SORT_ASC);
            sortUsersASC(db);
            break;
        }
        case $key_2:{
            puts(SORT_DESC);
            sortUsersDESC(db);
            break;
        }
        default:{
            puts(UNKNOWN_COMMAND);
            break;
        }
    }
}
void findByFIO(sqlite3 *db){
    system("cls");
    char name[20], second_name[20], surname[20];
    char *sql_find_by_fio = "SELECT user_id, name, surname, second_name FROM table_list_users WHERE name = ? AND second_name = ? AND surname = ?";
    puts("Enter name to find:");
    scanf("%20s", name);
    puts("Enter surname to find:");
    scanf("%20s", surname);
    puts("Enter second name to find:");
    scanf("%20s", second_name);
    printf("\nAll Record with %s %s %s\n", name, surname, second_name);
    sqlite3_stmt *res;
    if(sqlite3_prepare_v2(db, sql_find_by_fio, -1, &res, 0) == SQLITE_OK){
        sqlite3_bind_text(res, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, second_name, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 3, surname, -1, SQLITE_STATIC);
        while (sqlite3_step(res) == SQLITE_ROW) 
            {
                printf("\nUser Id: %-20ju | ", sqlite3_column_int64(res, 0));
                printf("Name: %-20s | ", sqlite3_column_text(res, 1));
                printf("Surname: %-20s | ", sqlite3_column_text(res, 2));
                printf("Second name: %-20s | ", sqlite3_column_text(res, 3));
            }
        
    } else fprintf(ERROR_FROM_DB);
    sqlite3_finalize(res);
}
void adminProfileMenu(sqlite3 *db){
    while(1){
        title_admin_profile; 
        system("cls");
        puts("1 - Create user profile\n2 - Read user profile\n3 - Update user profile\n4 - Delete user profile\n5 - Print all users\n6 - Find By FIO\n7 - Sort Menu\nEsc - Return");
        switch(getch()){
            case $key_1:{
                title_admin_create;
                createUser(db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_2:{
                title_admin_read;
                system("cls");
                uint64_t read_id;
                puts("Enter user's id:");
                while(!scanf("%ju[^\n]", &read_id))scanf("%*[^\n]");
                if(isUserExists(read_id, db)){
                    readUser(db, read_id);
                } else puts(NO_ID);
                puts(EXIT_MSG);
                getch();
                break;
            }
            case $key_3:{
                title_admin_update;
                adminUpdate(db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_4:{
                title_admin_delete;
                deleteUser(db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_5:{
                title_admin_read;
                printAllUsers(db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_6:{
                title_admin_find;
                findByFIO(db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_7:{
                title_admin_sort_users;
                sortUsersMenu(db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_esc:{
                return;
            }
            default:{
                break;
            }
        }
    }
}
void adminMenu(sqlite3 *db){
    while(1){
        title_admin; 
        system("cls");
        puts("1 - Work with user's profiles\n2 - Work with user's records\nEsc - Log out");
        switch(_getch()){
            case $key_1:{
                title_admin_create;
                adminProfileMenu(db);
                break;
            }
            case $key_2:{
                adminRecordMenu(db);
                break;
            }
            case $key_esc:{
                return;
                break;
            }
            default:{
                break;
            }
        }    
    }
}