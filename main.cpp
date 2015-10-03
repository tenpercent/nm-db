#include <iostream>
#include <memory>

#include "db_handle.hpp"
#include "convert_nm_entry_to_tuple.hpp"

using std::cin;
using std::string;

int main ()
{
  db_handle h {"names-db"};

  while (cin) {
    string s;
    cin >> s;
    auto t = convert_nm_entry_to_tuple (s);
    h.push (t);
    /* code */
  }
  return 0;
}
