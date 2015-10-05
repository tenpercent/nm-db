#include "db_handle.hpp"

#include <iostream>
#include <sstream>

using std::string;
using std::ostringstream;
using std::tuple;
using std::unique_ptr;
using std::cerr;
using std::endl;
using std::cout;
using std::get;

template<typename T>
static
string optional_value_to_string(const unique_ptr<T> &c)
{
  if (!c)
    return {"NULL"};

  ostringstream c_to_str;
  c_to_str << *c;
  return c_to_str.str();
}

db_handle::db_handle(const string& filename)
{
  connection = sqlite3_open_v2(
    filename.c_str(),
    &pDb,
    SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
    nullptr
  );

  const string CREATE_TABLE = [this] () -> string
  {
    ostringstream os;
    os << "CREATE TABLE " << table_name
       << "( ID INTEGER, ADDRESS INTEGER, SIZE INTEGER, MODE TEXT, NAME TEXT );" << endl;
    return os.str();
  } ();

  char *error_buf;

  sqlite3_exec(
    pDb,
    CREATE_TABLE.c_str (),
    nullptr,
    nullptr,
    &error_buf);

  cerr << "executed:\n" << CREATE_TABLE << endl;

  if (error_buf)
  {
    cerr << "Failed in constructor with:\n" << string { error_buf } << endl;
    sqlite3_free(error_buf);
  }
}

db_handle::~db_handle()
{
  int return_status = sqlite3_close(pDb);
#ifdef DEBUG
  if (return_status)
    cerr << "Warning: sqlite3 return status: " << return_status << endl;
#else
  (void) return_status;
#endif
}

void db_handle::push(tuple<
                           unique_ptr<size_t>,
                           unique_ptr<size_t>,
                           unique_ptr<char>,
                           unique_ptr<string>
                          > &entry)
{
  const string DELIMITER {", "};
  const string QUOTE {"\""};
  const string PUSH_ENTRY = [&] () {
    return string { "INSERT INTO "}
           + table_name
           + string { " (ID, ADDRESS, SIZE, MODE, NAME) \n" }
           + string { "VALUES( "}
           + optional_value_to_string(table_size)
           + DELIMITER + optional_value_to_string(get<0>(entry))
           + DELIMITER + optional_value_to_string(get<1>(entry))
           + DELIMITER + QUOTE + optional_value_to_string(get<2>(entry)) + QUOTE
           + DELIMITER + QUOTE + optional_value_to_string(get<3>(entry)) + QUOTE
           + string { " );\n" };
  } ();

  char *error_buf;

  sqlite3_exec(
    pDb,                                  /* An open database */
    PUSH_ENTRY.c_str(),                           /* SQL to be evaluated */
    nullptr, // int (*callback)(void*,int,char**,char**),  /* Callback function */
    nullptr, // void *,                                    /* 1st argument to callback */
    &error_buf                              /* Error msg written here */
  );

  ++*table_size;

  if (error_buf)
  {
    cerr << "SQLite error: " << string { error_buf } << endl;
    cerr << "with statement:" << endl << PUSH_ENTRY << endl;
    sqlite3_free(error_buf);
  }
}
