
#include <iomanip>

#include "Error.h"
#include "Logging.h"
#include "History.h"

size_t History::count() const
{
  return m_guessed.size();
}

size_t History::count_accepted() const
{
  return m_accepted.size();
}

size_t History::count_rejected() const
{
  return m_rejected.size();
}

std::ostream & History::format(std::ostream & out) const
{
  if (m_guessed.empty())
  {
    U_LOGW("No guesses have been made yet.");
    return out;
  }

  size_t pad = 9;
  for (Guess guess : m_guessed)
  {
    pad = std::max(pad, guess.word.size());
  }

  U_LOGI("History:");

  out 
    << std::setw(pad) << std::left << "ACCEPTED:"
    << " | "
    << std::setw(pad) << std::left << "REJECTED:"
    << "\n"
    << std::setw(2 * pad + 5) << std::setfill('-') << std::left << "-" 
    << "\n"
    << std::setfill(' ');

  for (Guess guess : m_guessed)
  {
    if (guess.accepted)
    {
      out
        << std::setw(pad) << std::left << guess.word
        << " | "
        << std::setw(pad) << std::left << " "
        << "\n";
    }
    else
    {
      out
        << "."
        << std::setw(pad - 1) << std::left << " " 
        << " | " 
        << std::setw(pad) << std::left << guess.word 
        << "\n";
    }
  }

  out << "\n";
  return out;
}

std::vector<Guess> History::get(size_t n) const
{
  if (n > m_guessed.size())
  {
    THROW_ERROR("Requested ", n, " guesses, but this history only has ", count(), " of them.");
  }

  std::vector<Guess> retval {};
  for (int i = count() - 1; i > count() - 1 - n; i--)
  {
    retval.push_back(m_guessed[i]);
  }

  return retval;
}

std::vector<std::string> History::get_accepted(size_t n) const
{
  if (n > m_accepted.size())
  {
    THROW_ERROR("Requested ", n, " accepted words, but this history only has ", count_accepted(), " of them.");
  }

  std::vector<std::string> retval {};
  for (int i = count_accepted() - 1; i > count_accepted() - 1 - n; i--)
  {
    retval.push_back(m_accepted[i].word);
  }

  return retval;
}

std::vector<std::string> History::get_rejected(size_t n) const
{
  if (n > m_rejected.size())
  {
    THROW_ERROR("Requested ", n, " rejected words, but this history only has ", count_rejected(), " of them.");
  }

  std::vector<std::string> retval {};
  for (int i = count_rejected() - 1; i > count_rejected() - 1 - n; i--)
  {
    retval.push_back(m_rejected[i].word);
  }

  return retval;
}

Guess History::peek() const
{
  return m_guessed.empty() ? Guess() : m_guessed.back();
}

std::string History::peek_accepted() const
{
  return m_accepted.empty() ? "" : m_accepted.back().word;
}

std::string History::peek_rejected() const
{
  return m_rejected.empty() ? "" : m_rejected.back().word;
}

void History::push(const Guess & guess)
{
  m_guessed.push_back(guess);
  (guess.accepted ? m_accepted : m_rejected).push_back(guess);
}

void History::push_accept(const std::string & word)
{
  try
  {
    Guess guess = Guess(word, true);
    push(guess);
  }
  catch (Error & e)
  {
    STACK_ERROR(e, "Cannot accept the empty word.");
  }
}

void History::push_reject(const std::string & word)
{
  try
  {
    Guess guess = Guess(word, false);
    push(guess);
  }
  catch (Error & e)
  {
    STACK_ERROR(e, "Cannot reject the empty word.");
  }
}

std::ostream & History::state_format(std::ostream & out) const
{
  out << m_state.dump() << std::endl;
  return out;
}

bool History::state_has(const std::string & key) const
{
  return m_state.count(key) > 0;
}

