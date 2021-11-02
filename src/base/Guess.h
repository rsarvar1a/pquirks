
#ifndef PQ_GUESS_H_
#define PQ_GUESS_H_

#include <stdexcept>
#include <string>

/**
 *  Represents a guess and its result. 
 */ 
struct Guess
{
  /**
   *  Constructs the null guess, which is the guess
   *  with an empty word.
   */ 
  explicit Guess ();

  /**
   *  Constructs a guess with the given word, which
   *  must be non-empty.
   */ 
  explicit Guess (const std::string & word, bool accepted);

  /**
   *  Determines if the guess is null.
   */ 
  bool null() const;

  std::string word;
  bool accepted;
};

#endif

