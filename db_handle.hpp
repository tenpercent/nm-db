#include <cstddef>
#include <memory>
#include <string>
#include <utility>

#include <sqlite3.h>

struct sqlite3;

using std::string;
using std::tuple;
using std::unique_ptr;
using std::make_unique;

class db_handle
{
private:
  sqlite3 *pDb = nullptr;
  int connection = 0;
  const string table_name {"names-db"};
  unique_ptr<size_t> table_size = make_unique<size_t> (0);
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
