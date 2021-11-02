
#include <iomanip>
#include <iostream>

#include <Rules.h>

int main()
{
  initialize_rules();

  size_t pad_name = 0;
  size_t pad_desc = 0;
  for (auto entry : Rules::get_rule_table())
  {
    pad_name = std::max(pad_name, entry.second->name().size());
    pad_desc = std::max(pad_desc, entry.second->description().size());
  }
  pad_name += 2;

  for (auto entry : Rules::get_rule_table())
  {
    std::cout 
      << std::setw(pad_name) << "\"" + entry.first + "\""
      << ": "
      << std::setw(pad_desc) << entry.second->description()
      << "\n";
  }
}
