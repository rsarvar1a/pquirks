
#include <mutex>

#include "Error.h"
#include "Logging.h"

void Error::print() const noexcept
{
  std::stringstream ss {};

  const CallError & top = m_stack.back();
  ss << log_print(GET_TAGE(top.file, top.func, top.line));
  ss << top.message;

  for (int i = m_stack.size() - 2; i >= 0; i--)
  {
    const CallError & cur = m_stack[i];
    ss << std::endl << std::endl;
    ss << "· · ·\033[3m\033[38;5;248m due to\033[0m ";
    ss << log_print(GET_TAGE(cur.file, cur.func, cur.line));
    ss << "· · · " << cur.message;
  }

  U_LOGBARE(ss.str());
}

const char * Error::what() const noexcept
{
  const CallError & top = m_stack.back();
  U_LOGD_FULL(top.file, top.func, top.line, "Never printed.");
  static std::string help = "Error; stack size " + std::to_string(m_stack.size()) + " (use Error::print() to print this stacktrace.)";
  return help.c_str();
}

