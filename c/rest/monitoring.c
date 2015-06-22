#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rest_helpers.h"
#include "../thirdparty/sqlite3.h"

#define NaN 0.0/0.0

static const char* RESPONSE_OPEN = "{\"patients\": [";
static const char* PERSON_OPEN =  "{\"name\": \"%s\", \"room\": \"%s\", \"measurements\": [";
static const char* PERSON_CLOSE = "]}";
static const char* MEASUREMENT =  "{\"min\": %.1f, \"max\": %.1f, \"value\": %.1f, \"name\": \"%s\", \"datetime\": \"%s\", \"unit\": \"%s\"}";
static const char* MEASUREMENT_NO_LIMITS =  "{\"min\": null, \"max\": null, \"value\": %.1f, \"name\": \"%s\", \"datetime\": \"%s\", \"unit\": \"%s\"}";
static const char* RESPONSE_CLOSE = "]}";

void put_patients_from_db(sqlite3* db);
void put_measurements_for_patient(sqlite3* db, int patient_id);
void put_measurement(sqlite3* db, int patient_id, int measurement_type, const unsigned char* datetime, double value);

void put_patients_from_db(sqlite3* db) {
  const char* sql = "SELECT id, name, room FROM patients WHERE device_id IS NOT NULL ORDER BY name;";
  sqlite3_stmt* statement;
  int should_put_comma;

  if (SQLITE_OK != sqlite3_prepare(db, sql, strlen(sql), &statement, NULL)) {
    printf(REST_ERROR, sql);
    return;
  }

  puts(RESPONSE_OPEN);
  should_put_comma = 0;

  while (SQLITE_ROW == sqlite3_step(statement)) {
    if (should_put_comma) {
      puts(", ");
    }

    should_put_comma = 1;
    printf(PERSON_OPEN,
           sqlite3_column_text(statement, 1),
           sqlite3_column_text(statement, 2));
    put_measurements_for_patient(db, sqlite3_column_int(statement, 0));
    printf(PERSON_CLOSE, "false");
  }

  puts(RESPONSE_CLOSE);
  sqlite3_finalize(statement);
}

void put_measurements_for_patient(sqlite3* db, int patient_id) {
  int should_put_comma;
  const char* sql =
    "SELECT MAX(datetime), measurement_type, value FROM measurements WHERE patient_id = ? GROUP BY measurement_type;";
  sqlite3_stmt* statement;

  if (SQLITE_OK != sqlite3_prepare(db, sql, strlen(sql), &statement, NULL)) {
    printf(REST_ERROR, sql);
    return;
  }

  if (SQLITE_OK != sqlite3_bind_int(statement, 1, patient_id)) {
    printf(REST_ERROR, "Failed to bind patient_id to statement.");
    sqlite3_finalize(statement);
    return;
  }

  should_put_comma = 0;

  while (SQLITE_ROW == sqlite3_step(statement)) {
    if (should_put_comma) {
      puts(", ");
    }

    should_put_comma = 1;
    put_measurement(db, patient_id,
                    sqlite3_column_int(statement, 1),
                    sqlite3_column_text(statement, 0),
                    sqlite3_column_double(statement, 2)
                   );
  }

  sqlite3_finalize(statement);
}

void put_measurement(sqlite3* db, int patient_id, int measurement_type, const unsigned char* datetime, double value) {
  const char* type_sql = "SELECT name, unit FROM measurements_types WHERE id = ?;";
  const char* limit_sql = "SELECT min, max FROM patients_limits WHERE patient_id = ? AND measurement_type_id = ?;";
  sqlite3_stmt* type_statement;
  sqlite3_stmt* limit_statement;
  char* name = "?";
  char* unit = "";
  double min = NaN;
  double max = NaN;

  if ((SQLITE_OK == sqlite3_prepare(db, type_sql, strlen(type_sql), &type_statement, NULL))
      && (SQLITE_OK == sqlite3_bind_int(type_statement, 1, measurement_type))
      && (SQLITE_ROW == sqlite3_step(type_statement))) {
    name = (char*) sqlite3_column_text(type_statement, 0);
    unit = (char*) sqlite3_column_text(type_statement, 1);
  }

  if ((SQLITE_OK == sqlite3_prepare(db, limit_sql, strlen(limit_sql), &limit_statement, NULL))
      && (SQLITE_OK == sqlite3_bind_int(limit_statement, 1, patient_id))
      && (SQLITE_OK == sqlite3_bind_int(limit_statement, 2, measurement_type))
      && (SQLITE_ROW == sqlite3_step(limit_statement))) {
    min = sqlite3_column_double(limit_statement, 0);
    max = sqlite3_column_double(limit_statement, 1);
  }

  if (min != min && max != max) { // NaN != NaN
    printf(MEASUREMENT_NO_LIMITS, value, name, datetime, unit);
  } else {
    printf(MEASUREMENT, min, max, value, name, datetime, unit);
  }
}

int main(void) {
  sqlite3* db;
  put_headers();

  if (SQLITE_OK != sqlite3_open("/var/www/database.db", &db)) {
    printf(REST_ERROR, sqlite3_errmsg(db));
  } else {
    put_patients_from_db(db);
  }

  sqlite3_close(db);
  return EXIT_SUCCESS;
}
