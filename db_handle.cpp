#include <string>
#include <memory>
#include <utility>

using std::string;
using std::tuple;
using std::unique_ptr;

class db_handle ()
{
private:
  sqlite3 *pDb = nullptr;
  int connection;
public:
  explicit db_handle (const string& filename);

  ~db_handle ();

  void push (tuple<
                   unique_ptr<size_t>,
                   unique_ptr<size_t>,
                   unique_ptr<char>,
                   unique_ptr<string>
                  > &entry);
};

db_handle::db_handle (const string& filename)
{
  connection = sqlite3_open (filename.c_str, &pDb);
}

db_handle::~db_handle ()
{
  sqlite3_close (pDb);
}

void db_handle::push (tuple<
                           unique_ptr<size_t>,
                           unique_ptr<size_t>,
                           unique_ptr<char>,
                           unique_ptr<string>
                           > &entry)
{
  sqlite3_stmt *pStmt = nullptr;
  sqlite3_prepare (pDb, "STATEMENT", 1024, &pStmt, nullptr);
}
