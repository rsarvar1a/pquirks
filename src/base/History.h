
#ifndef PQ_HISTORY_H_
#define PQ_HISTORY_H_

#include <iostream>
#include <string>
#include <vector>

#include "Guess.h"

/**
 *  Stores the word data for a given rule.
 */ 
class History
{
  public:

    /**
     *  Creates an empty history.
     */ 
    History() = default;

    /**
     *  Returns the number of guesses.
     */ 
    size_t count() const;

    /**
     *  Returns the number of accepted words.
     */ 
    size_t count_accepted() const;

    /**
     *  Returns the number of rejected words.
     */ 
    size_t count_rejected() const;

    /**
     *  Prints the history to the given output stream.
     */ 
    std::ostream & format(std::ostream & out) const;

    /**
     *  Returns the desired number of guesses in chronological order, with the most recent guesses first.
     */ 
    std::vector<Guess> get(size_t n) const;

    /**
     *  Returns the desired number of accepted words in chronological order, with the most recent guesses first.
     */ 
    std::vector<std::string> get_accepted(size_t n) const;

    /**
     *  Returns the desired number of rejected words in chronological order, with the most recent guesses first.
     */ 
    std::vector<std::string> get_rejected(size_t n) const;

    /**
     *  Returns the most recently-guessed word.
     */ 
    Guess peek() const;

    /**
     *  Returns the most recently guessed accepted word.
     */ 
    std::string peek_accepted() const;

    /**
     *  Returns the most recently guessed rejected word.
     */ 
    std::string peek_rejected() const;

    /**
     *  Adds the guess to the history.
     */ 
    void push(const Guess & guess);

    /**
     *  Accepts the given word and adds the guess to the history.
     */ 
    void push_accept(const std::string & word);

    /**
     *  Rejects the given word and adds the guess to the history.
     */ 
    void push_reject(const std::string & word);

  private:

    std::vector<Guess> m_guessed;
    std::vector<Guess> m_accepted;
    std::vector<Guess> m_rejected;
};

#endif

