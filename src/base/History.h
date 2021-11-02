
#ifndef PQ_HISTORY_H_
#define PQ_HISTORY_H_

#include <nlohmann/json.hpp>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Error.h"
#include "Guess.h"

/**
 *  Stores the word data for a given rule.
 *
 *  Also includes an arbitrary json object that can be used to get and set arbitrary states.
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

    /**
     *  Dumps the state to the given stream.
     */ 
    std::ostream & state_format(std::ostream & out) const;

    /**
     *  Gets the value located at the given key.
     */
    template<typename T>
    T state_get(const std::string & key) const;

    /**
     *  Determines whether the state has a value at the given key.
     */ 
    bool state_has(const std::string & key) const;

    /**
     *  Sets the value at the given key.
     */ 
    template<typename T>
    void state_set(const std::string & key, T t) const;

  private:

    std::vector<Guess> m_guessed;
    std::vector<Guess> m_accepted;
    std::vector<Guess> m_rejected;
    nlohmann::json     m_state;
};

template<typename T>
T History::state_get(const std::string & key) const
{
  if (! state_has(key))
  {
    THROW_ERROR("The history does not contain the key '", key, "'.");
  }

  T ret = m_state.at(key);
  return ret;
}

template<typename T>
void History::state_set(const std::string & key, T t) const
{
  const_cast<nlohmann::json &>(m_state)[key] = t;
}

#endif

