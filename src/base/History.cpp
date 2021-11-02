
#include <iomanip>

#include "Error.h"
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
    THROW_ERROR("History should not be empty!");
  }

  size_t pad = 8;
  for (Guess guess : m_guessed)
  {
    pad = std::max(pad, guess.word.size());
  }

  out 
    << std::setw(pad) << "\033[0;4mACCEPTED:\033[0m"
    << " | "
    << std::setw(pad) << "\033[0;4mREJECTED:\033[0m"
    << "\n";

  for (Guess guess : m_guessed)
  {
    if (guess.accepted)
    {
      out
        << std::setw(pad) << guess.word
        << " | "
        << std::setw(pad) << " "
        << "\n";
    }
    else
    {
      out
        << std::setw(pad) << " " 
        << " | " 
        << std::setw(pad) << guess.word 
        << "\n";
    }
  }

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

