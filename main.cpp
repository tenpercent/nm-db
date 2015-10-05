#include <iostream>
#include <memory>

#include "db_handle.hpp"
#include "convert_nm_entry_to_tuple.hpp"

using std::cin;
using std::string;
using std::cerr;
using std::endl;

int main () {
  db_handle h {"names.db"};

  while (cin) {
    const string s = [] () {
      string s;
      getline (cin, s);
      return s;
    } ();

    if (s.empty ())
      break;

    try {
      auto t = convert_nm_entry_to_tuple (s);
      h.push (t);
    } catch (...) {
      cerr << "problems with:\n" << s << endl;
      return -1;
    }
  }
  return 0;
}

