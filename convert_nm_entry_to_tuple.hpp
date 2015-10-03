#include <memory>
#include <string>
#include <tuple>

using std::tuple;
using std::unique_ptr;
using std::string;

tuple<
unique_ptr<size_t>,
unique_ptr<size_t>,
unique_ptr<char>,
unique_ptr<string>
>
convert_nm_entry_to_tuple (const string &nm_out);
