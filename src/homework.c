#include "homework.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SQL_CREATE_TABLE                                                       \
  "create table if not exists homework(id INTEGER PRIMARY KEY AUTOINCREMENT, " \
  "description TEXT not null, due DATE default current_timestamp);"
#define SQL_SELECT_HOMEWORK "select description from `homework` where due=?"

static sqlite3 *db;

int homework_init_db() {
  int result = sqlite3_open("homework.db", &db);
  char *errmsg = 0;
  if (result != SQLITE_OK) {
    fprintf(stderr, "Failed to establish database connection: %s\n",
            sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  result = sqlite3_exec(db, SQL_CREATE_TABLE, 0, 0, &errmsg);
  if (result != SQLITE_OK) {
    fprintf(stderr, "Failed to setup database: %s\n", errmsg);
    sqlite3_free(errmsg);
    sqlite3_close(db);
    return 1;
  }

  return 0;
}

homework_s *homework_get_at(time_t date) {
  int rc;
  homework_s *hm = malloc(sizeof(homework_s));

  sqlite3_stmt *res;
  rc = sqlite3_prepare_v2(db, SQL_SELECT_HOMEWORK, -1, &res, 0);
  sqlite3_bind_int64(res, 1, (unsigned long) date);
  hm->description_length = 0;
  hm->description = 0;
  if (rc == SQLITE_OK) {
    if ((rc = sqlite3_step(res)) == SQLITE_ROW) {
      const char *colval = (const char *)sqlite3_column_text(res, 1);
      size_t len = strlen(colval);
      hm->description = malloc(len);
      hm->description_length = len;
      strncpy(hm->description, colval, len);
    }
  }

  sqlite3_finalize(res);

  return hm;
}

void homework_destroy(homework_s *hm);

void homework_shutdown() {
  if (db) {
    sqlite3_close(db);
  }
}
