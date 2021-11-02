
#include "Error.h"
#include "Guess.h"

Guess::Guess()
  : word {""}
  , accepted {false}
{}

Guess::Guess(const std::string & word, bool accepted)
  : word {word}
  , accepted {accepted}
{
  if (word == "")
  {
    THROW_ERROR("The word in a guess must not be empty!");
  }
}

bool Guess::null() const
{
  return word == "";
}

