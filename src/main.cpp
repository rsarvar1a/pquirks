
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

  for (auto entry : table)
  {
    std::cout 
      << "\033[1;4m\"" + entry.first + "\"\033[0m\n\n"
      << "\033[3m" << entry.second->description() << "\033[0m\n\n";
  }
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
    std::cout << "=> \033[1;3m";
    getline(std::cin, line);
    std::cout << "\033[0m\n";

    size_t nargs;
    std::vector<std::string> cmdline = cmd_parse(line, nargs);
    if (nargs == 0) continue;

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
            history = {};
            in_effect->initialize(history);
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
          if (in_effect)
          {
            U_LOGI("Restarting with active rule '", in_effect->name(), "'.");
          }
          else 
          {
            U_LOGW("Clearing history, but there is no rule in effect.");
          }
          break;
        }
      case "s"_:
      case "state"_:
        {
          U_LOGI("State:");
          if (in_effect) in_effect->print_state(std::cout, history);
          else history.state_format(std::cout);
          std::cout << std::endl;
          break;
        }
      case "?"_:
        {
          std::string helptext = 
            "clear"
            "\n\taliases: 'cls'"
            "\n\tclears the screen"
            "\n"
            "\nguess   <word>"
            "\n\taliases: 'g'"
            "\n\tguesses a word with respect to the active rule"
            "\n"
            "\nhistory"
            "\n\taliases: 'h'"
            "\n\tshows this game's guesses in chronological order"
            "\n"
            "\nlist"
            "\n\taliases: 'l', 'ls'"
            "\n\tlists all available rules"
            "\n"
            "\nnewgame <rule>"
            "\n\taliases: 'ng', 'new'"
            "\n\tloads the given rule and starts a new game"
            "\n"
            "\nquit"
            "\n\taliases: 'q'"
            "\n\tquits pquirks"
            "\n"
            "\nrestart"
            "\n\taliases: 'rs'"
            "\n\tclears the history"
            "\n"
            "\n?"
            "\n\tshows this help menu"
            "\n";

          U_LOGI("Available commands:");
          std::cout << helptext << std::endl;
          break;
        }
      default:
        {
          U_LOGE("Unknown command '", cmd, "'; type `?` for help.");
          break;
        }
    }
  }
}

