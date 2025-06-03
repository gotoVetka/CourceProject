
typedef enum $keys{
    $key_esc=27,
    $key_1=49,
    $key_2,
    $key_3,
    $key_4,
    $key_5,
    $key_6,
    $key_7,
    $key_8,
    $key_9
} keys;

typedef struct $RECORD_USER{
    uint64_t id;
    char login[20];
    char passwd[20];
    char *role;
    char name[20];
    char surname[20];
    char second_name[20];
    int hours;
    int lateness;
} RECORD_USER;

struct date{
    int day;
    int month;
    int year;
};

typedef struct $RECORD_DAY{
    uint64_t user_id;
    char *type;
    struct date start;
    char start_string[20];
    struct date end;
    char end_string[20];
} RECORD_DAY;

void foreignKeysOn(sqlite3* db){
    char *exec_res;
    if(sqlite3_exec(db, "PRAGMA foreign_keys=ON", NULL, NULL, &exec_res) != SQLITE_OK){
        fprintf(ERROR_FROM_DB);
    }
}

bool isUserExists(uint64_t user_id, sqlite3 *db){
    sqlite3_stmt *check_res;
    bool check_result;
    char *sql_check_id = "SELECT 1 FROM table_list_users WHERE user_id = ?";
    if(sqlite3_prepare_v2(db, sql_check_id, -1, &check_res, 0) == SQLITE_OK){
        sqlite3_bind_int64(check_res, 1, user_id);
        sqlite3_step(check_res);
        if(sqlite3_column_int(check_res, 0)==1){
            check_result = true;
        } else {
            check_result = false;
        }
    } else fprintf(ERROR_FROM_DB);
    sqlite3_finalize(check_res);
    return check_result;
}

bool isRecordExists(sqlite3 *db, uint64_t user_id, uint64_t record_id){
    sqlite3_stmt *check_res;
    bool check_result;
    char *sql_check_record = "SELECT 1 FROM table_users_sick_and_off WHERE user_id = ? AND id = ?";
    if(sqlite3_prepare_v2(db, sql_check_record, -1, &check_res, 0) == SQLITE_OK){
        sqlite3_bind_int64(check_res, 1, user_id);
        sqlite3_bind_int64(check_res, 2, record_id);
        sqlite3_step(check_res);
        if(sqlite3_column_int(check_res, 0)==1){
            check_result = true;
        } else {
            check_result = false;
        }
    } else fprintf(ERROR_FROM_DB);
    sqlite3_finalize(check_res);
    return check_result;
}

bool isValidDate(struct date record){
    if(!(0<record.day && record.day<32))return false;
    if(!(0<record.month && record.month<13))return false;
    if(!(1900<record.year && record.year < 2030))return false;
    return true;
}

bool isValidInterval(RECORD_DAY record){
    if((record.start.year<record.end.year)){
        return true;
    }
    if((record.start.month<record.end.month)){
        return true;
    }
    if((record.start.day<record.end.day)){
        return true;
    }
    return false;
}

void updateUserByParamText(sqlite3 *db, char *sql_command, char *to_update, uint64_t user_id){
    sqlite3_stmt *res;
    if(sqlite3_prepare_v2(db, sql_command, -1, &res, 0) == SQLITE_OK){
        sqlite3_bind_text(res, 1, to_update, -1, SQLITE_STATIC);
        sqlite3_bind_int64(res, 2, user_id);
        if(sqlite3_step(res) != SQLITE_DONE){
            fprintf(stderr, "Error : %s\n", sqlite3_errmsg(db));
        } else puts(UPDATE_SUCCESS);
    } else {
        fprintf(ERROR_FROM_DB);
    }
    sqlite3_finalize(res);
}

void updateUserByParamInt64(sqlite3 *db, char *sql_command, uint64_t to_update, uint64_t user_id){
    sqlite3_stmt *res;
    if(sqlite3_prepare_v2(db, sql_command, -1, &res, 0) == SQLITE_OK){
        sqlite3_bind_int64(res, 1, to_update);
        sqlite3_bind_int64(res, 2, user_id);
        if(sqlite3_step(res) != SQLITE_DONE){
            fprintf(stderr, "Error : %s\n", sqlite3_errmsg(db));
        } else puts(UPDATE_SUCCESS);
    } else {
        fprintf(ERROR_FROM_DB);
    }
    sqlite3_finalize(res);
}

void updateUserByParamInt(sqlite3 *db, char *sql_command, int to_update, uint64_t user_id){
    sqlite3_stmt *res;
    if(sqlite3_prepare_v2(db, sql_command, -1, &res, 0) == SQLITE_OK){
        sqlite3_bind_int(res, 1, to_update);
        sqlite3_bind_int64(res, 2, user_id);
        if(sqlite3_step(res) != SQLITE_DONE){
            fprintf(stderr, "Error : %s\n", sqlite3_errmsg(db));
        } else puts(UPDATE_SUCCESS);
    } else {
        fprintf(ERROR_FROM_DB);
    }
    sqlite3_finalize(res);
}