#include <cstddef>
#include <string>
#include <sstream>
#include <utility>
#include <memory>

using std::make_tuple;
using std::make_unique;
using std::unique_ptr;
using std::istringstream;
using std::string;

#define DEBUG_ASSERT(X)

auto convert_nm_tuple_to_string (const std::string &nm_out)
{
  auto nm_out_unparsed = nm_out;

  // first 16 symbols are either digits or spaces
  const auto digits_len = 16;
  auto address = [&] () -> unique_ptr<size_t>
  {
    if (nm_out_unparsed[0] == '0')
    {
      // it's address!
      const std::string address_s = nm_out_unparsed.substr (0, digits_len);
      istringstream in (address_s);
      size_t address = 0;
      in >> address;
      return make_unique<size_t> (address);
    } else
    {
      DEBUG_ASSERT (nm_out_unparsed[0] == ' ')
      return {nullptr};
    }
  } ();

  nm_out_unparsed = nm_out_unparsed.substr (digits_len + 1);

  auto size = [&] () -> unique_ptr<size_t>
  {
    if (nm_out_unparsed[0] == '0')
    {
      // it's size!
      const std::string size_s = nm_out_unparsed.substr (0, digits_len);
      istringstream in (size_s);
      size_t size = 0;
      in >> size;
      return make_unique<size_t> (size);
    }
    else
    {
      return {nullptr};
    }
  } ();

  if (size.get ())
  {
    nm_out_unparsed = nm_out_unparsed.substr (digits_len + 1);
  }

  auto mode = [&] () -> unique_ptr<char>
  {
    return make_unique<char> (nm_out_unparsed[0]);
  } ();

  nm_out_unparsed = nm_out_unparsed.substr (1 + 1);

  auto name = [&] () -> unique_ptr<string>
  {
    return make_unique<string> (nm_out_unparsed);
  } ();

  // then either address or one of letters U, w
  return make_tuple (move(address), move(size), move(mode), move(name));
}
