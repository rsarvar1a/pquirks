
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include <Error.h>
#include <Logging.h>
#include <Rules.h>

constexpr inline uint32_t hash(const char* data, const size_t size) noexcept
{
    uint32_t hash = 5381;

    for(const char *c = data; c < data + size; ++c)
        hash = ((hash << 5) + hash) + (unsigned char) *c;

    return hash;
}

constexpr inline unsigned int operator "" _(const char * p, size_t s) 
{ 
  return hash(p, s); 
}

uint32_t hash(const std::string & s) noexcept
{
    uint32_t hash = 5381;

    for(char c : s)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

void cmd_clear()
{
  std::cout << "\033[2J\033[1;1H";
}

void cmd_ls (const std::map<std::string, const Rule *> & table)
{
  U_LOGI("Available rules:");

  size_t pad_name = 0;
  size_t pad_desc = 0;
  for (auto entry : table)
  {
    pad_name = std::max(pad_name, entry.second->name().size());
    pad_desc = std::max(pad_desc, entry.second->description().size());
  }
  pad_name += 2;

  for (auto entry : table)
  {
    std::cout 
      << std::setw(pad_name) << std::left << "\"" + entry.first + "\""
      << ": "
      << std::setw(pad_desc) << std::left << entry.second->description()
      << "\n";
  }

  std::cout << "\n";
}

std::vector<std::string> cmd_parse(const std::string & line, size_t & nargs)
{
  std::stringstream ss {line};
  std::string fragment;
  std::vector<std::string> cmdline;

  while (ss >> fragment)
  {
    cmdline.push_back(fragment);
  }

  nargs = cmdline.size();
  return cmdline;
}

int main()
{
  // Startup.

  initialize_rules();

  // Main loop.

  const std::map<std::string, const Rule *> & table = Rules::get_rule_table();
  const Rule * in_effect = nullptr;
  History history;

  std::string line;
  bool should_exit = false;

  while (! should_exit)
  {
    getline(std::cin, line);
    size_t nargs;
    std::vector<std::string> cmdline = cmd_parse(line, nargs);
    std::string cmd = cmdline[0];

    switch (hash(cmd)) 
    {
      case "cls"_:
      case "clear"_:
        {
          cmd_clear();
          break;
        }
      case "q"_:
      case "quit"_:
        {
          should_exit = true;
          break;
        }
      case "l"_:
      case "ls"_:
      case "list"_:
        {
          cmd_ls(table);
          break;
        }
      case "g"_:
      case "guess"_:
        {
          if (! in_effect)
          {
            U_LOGE("There is no rule in effect.");
          }
          else if (nargs < 2)
          {
            U_LOGI("Usage: guess <word>");
          }
          else
          {
            try
            {
              Guess g = Guess {cmdline[1], in_effect->test(cmdline[1], history)};
              history.push(g);
            }
            catch (Error & e)
            {
              e.print();
            }
          }
          break;
        }
      case "h"_:
      case "history"_:
        {
          history.format(std::cout);
          break;
        }
      case "ng"_:
      case "new"_:
      case "newgame"_:
        {
          if (nargs < 2)
          {
            U_LOGI("Usage: newgame <rule>");
          }
          else if (table.find(cmdline[1]) != table.end())
          {
            in_effect = table.at(cmdline[1]);
            U_LOGI("Loaded rule '", in_effect->name(), "'.");
          }
          else
          {
            U_LOGE("Unknown rule '", cmdline[1], "'.");
          }
          break;
        }
      case "rs"_:
      case "restart"_:
        {
          history = {};
          U_LOGI(in_effect ? ("Restarting with rule '", in_effect->name(), "'.") : "Clearing history.");
          break;
        }
      default:
        {
          U_LOGE("Unknown command '", cmd, "'.");
          break;
        }
    }
  }
}

