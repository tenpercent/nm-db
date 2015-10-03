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
string optional_value_to_string (const unique_ptr<T> &c)
{
  if (!c)
    return {"NULL"};

  ostringstream c_to_str;
  c_to_str << *c;
  return c_to_str.str ();
}

db_handle::db_handle (const string& filename)
{
  connection = sqlite3_open_v2 (
    filename.c_str (),
    &pDb,
    SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
    nullptr
  );
}

db_handle::~db_handle ()
{
  int return_status = sqlite3_close (pDb);
#ifdef DEBUG
  if (return_status)
    cerr << "Warning: sqlite3 return status: " << return_status << endl;
#else
  (void) return_status;
#endif
}

void db_handle::push (tuple<
                           unique_ptr<size_t>,
                           unique_ptr<size_t>,
                           unique_ptr<char>,
                           unique_ptr<string>
                           > &entry)
{
  const string DELIMITER {", "};
  const string PUSH_ENTRY = [&] () {
    return string { "INSERT INTO "}
           + table_name
           + string { " (ID, ADDRESS, SIZE, MODE, NAME) \n" }
           + string { "VALUES( "}
           + optional_value_to_string (table_size)
           + DELIMITER
           + optional_value_to_string (get<0> (entry))
           + DELIMITER
           + optional_value_to_string (get<1> (entry))
           + DELIMITER
           + optional_value_to_string (get<2> (entry))
           + DELIMITER
           + optional_value_to_string (get<3> (entry))
           + string {" );\n"};
  } ();

  char *error_buf;

  sqlite3_exec(
    pDb,                                  /* An open database */
    PUSH_ENTRY.c_str (),                           /* SQL to be evaluated */
    nullptr, // int (*callback)(void*,int,char**,char**),  /* Callback function */
    nullptr, // void *,                                    /* 1st argument to callback */
    &error_buf                              /* Error msg written here */
  );

  if (error_buf)
  {
    cerr << string (error_buf) << endl;
  }

}
