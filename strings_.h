//Titles
#define title_common system("title Control system: Sign in")

#define title_admin system("title Control system: Admin")
#define title_admin_profile system("title Control system: Admin - Work with user's profiles")
#define title_admin_record system("title Control system: Admin - Work with user's records")
#define title_admin_create system("title Control system: Admin - Create user")
#define title_admin_read system("title Control system: Admin - Read user")
#define title_admin_update system("title Control system: Admin - Update user")
#define title_admin_delete system("title Control system: Admin - Delete")
#define title_admin_find system("title Control system: Admin - Find")
#define title_admin_sort_users system("title Control system: Admin - Sort Users");
#define title_admin_add_record system("title Control system: Admin - Add Record");
#define title_admin_update_record system("title Control system: Admin - Update Record");
#define title_admin_delete_record system("title Control system: Admin - Delete Record");
#define title_admin_read_record system("title Control system: Admin - Read Record");
#define title_admin_sort_records system("title Control system: Admin - Sort Records");

#define title_user system("title Control system: User")
#define title_user_read system("title Control system: User - My profile")
#define title_user_update system("title Control system: User - Update my profile")
#define title_user_my_records system("title Control system: User - My records")
#define title_user_sort system("title Control system: User - Sort my records")

//Strings
#define LOG_ISSUE "Log issue! Try again\n"
#define EXIT_MSG "\nPress any key to exit tab.\n"
#define NO_ID "\nNo id in DB!\n"
#define NO_RECORD "\nNo record in DB!\n"
#define ADD_SUCCESS "Added successfully.\n"
#define DELETE_SUCCESS "Deleted successfully.\n"
#define UPDATE_SUCCESS "Updated successfully.\n"
#define UNKNOWN_COMMAND "Unknown command!\n"
#define INVALID_DATE_FORMAT "\nInvalid date format"
#define ERROR_FROM_DB stderr, "Error: %s\n", sqlite3_errmsg(db)
#define INVALID_TIME_INTERVAL "\nInvalid time interval"
#define SORT_ASC "\nAscending\n"
#define SORT_DESC "\nDescending\n"
#define SORT_ID "\nAll records sorted by Record Id\n"
#define SORT_USER_ID "\nAll records sorted by User Id\n"
#define SORT_TYPE "\nAll records sorted by Type\n"
#define SORT_START "\nAll records sorted by Start date\n"
#define SORT_END "\nAll records sorted by End date\n"
#define SORT_HOURS "\nAll records sorted by Hours\n"
#define SORT_LATENESS "\nAll records sorted by Lateness\n"
#define DATABASE_FILE_NAME "list.db"
#define NO_DATABASE_FILE "Cant open database!"
