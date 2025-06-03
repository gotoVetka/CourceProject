void addRecord(sqlite3 *db){
    system("cls");
    RECORD_DAY p;
    bool continueFill=true, isDateRight=false;
    while(continueFill){
        puts("Enter user id: ");
        while(!scanf("%ju", &p.user_id))scanf("%*[^\n]");
        if(!isUserExists(p.user_id, db)){
            puts(NO_ID);
            puts(EXIT_MSG);
            _getch();
            return;
        }
        puts("Select user role:\n1 - Vacation | 2 - Sick");
         while(continueFill){
            switch (getch()){
                case $key_1:{
                    p.type = "Vacation";
                    continueFill=false;
                    break;
                }
                case $key_2:{
                    p.type = "Sick";
                    continueFill=false;
                    break;
                }
                default:{
                    scanf("%*[^\n]");
                    break;
                }
            }
        }
        puts(p.type);
        puts("Enter start date(DD.MM.YYYY):");
        scanf("%i.%i.%i", &p.start.day, &p.start.month, &p.start.year);
        if(!isValidDate(p.start)){
            puts(INVALID_DATE_FORMAT);
            return;
        }
        puts("Enter end date(DD.MM.YYYY):");
        scanf("%i.%i.%i", &p.end.day, &p.end.month, &p.end.year);
        if(!isValidDate(p.end)){
            puts(INVALID_DATE_FORMAT);
            return;
        }
        if(!isValidInterval(p)){
            puts(INVALID_TIME_INTERVAL);
            return;
        }
        sprintf(p.start_string, "%i.%i.%i", p.start.day, p.start.month, p.start.year);
        sprintf(p.end_string, "%i.%i.%i", p.end.day, p.end.month, p.end.year);
        system("cls");
        printf("User id: %ju\nType: %s\n", p.user_id, p.type);
        printf("Start: %s\n", p.start_string);
        printf("End: %s", p.end_string);
        puts("\nIs that correct data?\n1 - Yes\n2 - No\n3 - Exit");
        switch(getch()){
            case $key_1:{
                isDateRight=true;
                break;
            }
            case $key_2:{
                system("cls");
                continueFill = true;
                break;
            }
            case $key_3:{
                continueFill=false;
                isDateRight= false;
                break;
            }
            default:{
                scanf("%*[^\n]");
                break;
            }
        }
    }
    if(isDateRight){
        char *sql_add_record = "INSERT INTO table_users_sick_and_off  (user_id, type, start, end) VALUES (?, ?, ?, ?)";
        sqlite3_stmt *res;
        if (sqlite3_prepare_v2(db, sql_add_record, -1, &res, 0) == SQLITE_OK){
            sqlite3_bind_int64(res, 1, p.user_id);
            sqlite3_bind_text(res, 2, p.type, -1, SQLITE_STATIC);
            sqlite3_bind_text(res, 3, p.start_string, -1, SQLITE_STATIC);
            sqlite3_bind_text(res, 4, p.end_string, -1, SQLITE_STATIC);
            if (sqlite3_step(res) == SQLITE_DONE) 
            {
                printf(ADD_SUCCESS);
            } else fprintf(stderr, "Error! Something gone wrong : %s\n", sqlite3_errmsg(db));
        } else {
            fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_finalize(res);
    }
}
void updateRecordByParam(sqlite3 *db, char *sql_command, char *to_update, uint64_t user_id, uint64_t record_id){
    sqlite3_stmt *res;
    if(sqlite3_prepare_v2(db, sql_command, -1, &res, 0) == SQLITE_OK){
        sqlite3_bind_text(res, 1, to_update, -1, SQLITE_STATIC);
        sqlite3_bind_int64(res, 2, user_id);
        sqlite3_bind_int64(res, 3, record_id);
        if(sqlite3_step(res) != SQLITE_DONE){
            fprintf(stderr, "Error : %s\n", sqlite3_errmsg(db));
        } else puts(UPDATE_SUCCESS);
    } else {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
}
void printAllRecordsByUserId(sqlite3 *db, uint64_t user_id){
    sqlite3_stmt *res;
    char *sql_print_all = "SELECT * FROM table_users_sick_and_off WHERE user_id = ?";
    if (sqlite3_prepare_v2(db, sql_print_all, -1, &res, 0) == SQLITE_OK) 
    {
        sqlite3_bind_int64(res, 1, user_id);
        printf("%-20s | %-20s | %-20s | %-20s | %-20s", "Record id", "User id", "Type", "Start date", "End date");
        while (sqlite3_step(res) == SQLITE_ROW) 
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
    puts("\n");
    sqlite3_finalize(res);
}
void printRecord(sqlite3 *db, uint64_t user_id, uint64_t record_id){
    sqlite3_stmt *res;
    char *sql_print_record = "SELECT * FROM table_users_sick_and_off WHERE user_id = ? AND id = ?";
    if (sqlite3_prepare_v2(db, sql_print_record, -1, &res, 0) == SQLITE_OK) 
    {
        sqlite3_bind_int64(res, 1, user_id);
        sqlite3_bind_int64(res, 2, record_id);
        printf("%-20s | %-20s | %-20s | %-20s | %-20s", "Record id", "User id", "Type", "Start date", "End date");
        while (sqlite3_step(res) == SQLITE_ROW) 
        {
            printf("\n%-20ju | ", sqlite3_column_int64(res, 0));
            printf("%-20ju | ", sqlite3_column_int64(res, 1));
            printf("%-20s | ", sqlite3_column_text(res, 2));
            printf("%-20s | ", sqlite3_column_text(res, 3));
            printf("%-20s\n", sqlite3_column_text(res, 4));
        }
    } else {

        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(res);
}
void printAllRecords(sqlite3 *db){
    sqlite3_stmt *res;
    char *sql_print_record = "SELECT * FROM table_users_sick_and_off";
    if (sqlite3_prepare_v2(db, sql_print_record, -1, &res, 0) == SQLITE_OK) 
    {
        printf("%-20s | %-20s | %-20s | %-20s | %-20s\n", "Record id", "User id", "Type", "Start date", "End date");
        while (sqlite3_step(res) == SQLITE_ROW) 
        {
            printf("\n%-20ju | ", sqlite3_column_int64(res, 0));
            printf("%-20ju | ", sqlite3_column_int64(res, 1));
            printf("%-20s | ", sqlite3_column_text(res, 2));
            printf("%-20s | ", sqlite3_column_text(res, 3));
            printf("%-20s\n", sqlite3_column_text(res, 4));
        }
    } else {

        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(res);
}
void deleteRecord(sqlite3 *db){
    system("cls");
    uint64_t user_id, record_id;
    puts("Enter user id:");
    while(!scanf("%ju", &user_id))scanf("%*[^\n]");
    if(isUserExists(user_id, db)){
        printAllRecordsByUserId(db, user_id);
        puts("\nEnter record id:");
        while(!scanf("%ju", &record_id))scanf("%*[^\n]");
        if(isRecordExists(db, user_id, record_id))
        {
            puts("Are you sure ? 1 - Yes");
            if(getch()!=$key_1){
                return;
            }
            sqlite3_stmt *res;
            char *sql_delete_record = "DELETE FROM table_users_sick_and_off WHERE user_id = ? AND id = ?"; 
            if(sqlite3_prepare_v2(db, sql_delete_record, -1, &res, 0) == SQLITE_OK){
                sqlite3_bind_int64(res, 1, user_id);
                sqlite3_bind_int64(res, 2, record_id);
                if(sqlite3_step(res) != SQLITE_DONE){
                    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
                } puts(DELETE_SUCCESS);
            }
            sqlite3_finalize(res);    
        } else puts(NO_RECORD);
    } else puts(NO_ID);
    puts(EXIT_MSG);
    _getch();
}
void updateRecord(sqlite3 *db){
    system("cls");
    uint64_t user_id, record_id;
    puts("Enter user id:");
    while(!scanf("%ju", &user_id))scanf("%*[^\n]");
    printAllRecordsByUserId(db, user_id);
    if(isUserExists(user_id, db)){
        puts("Enter record id:");
        while(!scanf("%ju", &record_id))scanf("%*[^\n]");
        if(isRecordExists(db, user_id, record_id))
        {

            printRecord(db, user_id, record_id);
            char *sql_update_record;
            puts("1 - Type | 2 - Start date | 3 - End date");
                switch (getch()){
                    case $key_1:{
                        bool is_valid_key;
                        puts("Select type of record:\n1 - Vacation | 2 - Sick");
                        char *type;
                        switch (getch()){
                            case $key_1:{
                                type = "Vacation";
                                is_valid_key=true;
                                break;
                            }
                            case $key_2:{
                                type = "Sick";
                                is_valid_key=true;
                                break;
                            }
                            default:{
                                is_valid_key=false;
                                break;
                            }
                        
                        }
                        if(is_valid_key){
                            updateRecordByParam(db, "UPDATE table_users_sick_and_off SET type = ? WHERE user_id = ? AND id = ?", type ,user_id, record_id);
                        } else puts("Unknown type");
                        break;
                    }
                    case $key_2:{
                        struct date new_start;
                        char new_start_string[20], *sql_update_record;
                        puts("Enter new start date(DD.MM.YYYY):");
                        scanf("%hi.%hi.%hi", &new_start.day, &new_start.month, &new_start.year);
                        sprintf(new_start_string, "%hi.%hi.%hi", new_start.day, new_start.month, new_start.year);
                        updateRecordByParam(db, "UPDATE table_users_sick_and_off SET start = ? WHERE user_id = ? AND id = ?", new_start_string, user_id, record_id);
                        break;
                    }
                    case $key_3:{
                        struct date new_end;
                        char new_end_string[20], *sql_update_record;
                        puts("Enter new end date(DD.MM.YYYY):");
                        scanf("%hi.%hi.%hi", &new_end.day, &new_end.month, &new_end.year);
                        sprintf(new_end_string, "%hi.%hi.%hi", new_end.day, new_end.month, new_end.year);
                        updateRecordByParam(db, "UPDATE table_users_sick_and_off SET end = ? WHERE user_id = ? AND id = ?", new_end_string, user_id, record_id);
                        break;
                    }
                }
        } else puts(NO_RECORD);
    } else puts(NO_ID); 
}
void sortRecordsCommand(sqlite3 *db, char *sql_command){
    sqlite3_stmt *res;
    if(sqlite3_prepare_v2(db, sql_command, -1, &res, 0) == SQLITE_OK){
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
void sortRecordsMenu(sqlite3 *db){
    puts("1 - Record id | 2 - User id | 3 - Start | 4 - End | 5 - Type");
    switch(getch()){
        case $key_1:{
            puts(SORT_ID);
            sortRecordsCommand(db, "SELECT * FROM table_users_sick_and_off ORDER BY id ASC");
            break;
        }
        case $key_2:{
            puts(SORT_USER_ID);
            sortRecordsCommand(db ,"SELECT * FROM table_users_sick_and_off ORDER BY user_id ASC");
            break;
        }
        case $key_3:{
            puts(SORT_START);
            sortRecordsCommand(db, "SELECT * FROM table_users_sick_and_off ORDER BY start ASC");
            break;
        }
        case $key_4:{
            puts(SORT_END);
            sortRecordsCommand(db, "SELECT * FROM table_users_sick_and_off ORDER BY end ASC");
            break;
        }
        case $key_5:{
            puts(SORT_TYPE);
            sortRecordsCommand(db, "SELECT * FROM table_users_sick_and_off ORDER BY type ASC");
            break;
        }
        default:{
            puts(UNKNOWN_COMMAND);
            break;
        }
    }
}
void adminRecordMenu(sqlite3 *db){
    bool continue_record_menu_flag = true;
    do{
        title_admin_record;
        system("cls");
        puts("1 - Add Record\n2 - Delete Record\n3 - Edit Record\n4 - Read record\n5 - Print all records\n6 - Sort Records\nESC - Return");
        switch(getch()){
            case $key_1:{
                title_admin_add_record;
                addRecord(db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_2:{
                title_admin_delete_record;
                deleteRecord(db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_3:{
                title_admin_update_record;
                updateRecord(db);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_4:{
                title_admin_read_record;
                uint64_t user_id, record_id;
                puts("Enter user id:");
                while(!scanf("%ju", &user_id))scanf("%*[^\n]");
                puts("Enter record id:");
                while(!scanf("%ju", &record_id))scanf("%*[^\n]");
                if(isRecordExists(db, user_id, record_id)){
                    printRecord(db, user_id, record_id);
                } else puts(NO_RECORD);
                puts(EXIT_MSG);
                _getch();
                break;
            }
            case $key_5:{
                title_admin_read_record;
                system("cls");
                printAllRecords(db);
                puts(EXIT_MSG);
                getch();
                break;
            }
            case $key_6:{
                title_admin_sort_records;
                sortRecordsMenu(db);
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
    }while(1);
}