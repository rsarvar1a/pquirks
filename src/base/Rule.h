
#ifndef PQ_RULE_H_
#define PQ_RULE_H_

#include <map>
#include <memory>
#include <string>

#include "History.h"

/**
 *  A representation of a Party Quirks rule, expressed programmatically.
 *  You must only implement the description and test methods; the rest 
 *  (including registration) is provided via the DERIVE_RULE(macro).
 */ 
class Rule
{
  public:

    /**
     *  Returns an English description for the rule.
     */ 
    virtual std::string description() const = 0;

    /**
     *  Returns a name or identifier for the rule.
     */ 
    virtual std::string name() const = 0;

    /**
     *  Determines if the word accepts or rejects given the history.
     */ 
    virtual bool test(const std::string & word, const History & history) const = 0;
};

/**
 *  A table of rules.
 */ 
class Rules
{
  public:

    /**
     *  Gets the rules table, allowing for invocations of the corresponding tests.
     */ 
    static const std::map<std::string, const Rule *> & get_rule_table();

    /**
     *  Registers the rule according to its name(). Throws an error if a 
     *  rule with the given name already exists.
     */ 
    static void register_rule(Rule * rule_instance);

  private:

    /**
     *  Constructs an empty rules map.
     */ 
    Rules() = default;

    /**
     *  Internally gets the rules instance.
     */ 
    static Rules & instance();

    std::map<std::string, const Rule *> m_rules;
};

/**
 *  Derives the class header for the rule of the given name.
 */ 
#define DERIVE_RULE(classname, base)                                                       \
  class classname : public base                                                            \
  {                                                                                        \
    public:                                                                                \
                                                                                           \
      using Base = base;                                                                   \
                                                                                           \
      static classname & instance()                                                        \
      {                                                                                    \
        static std::unique_ptr<classname> inst_ = std::make_unique<classname>();           \
        static bool is_registered = false;                                                 \
        if (! is_registered)                                                               \
        {                                                                                  \
          Rules::register_rule(inst_.get());                                               \
          is_registered = true;                                                            \
        }                                                                                  \
        return * inst_;                                                                    \
      }                                                                                    \
                                                                                           \
      virtual std::string description() const override;                                    \
                                                                                           \
      virtual std::string name() const override                                            \
      {                                                                                    \
        return #classname;                                                                 \
      }                                                                                    \
                                                                                           \
      virtual bool test(const std::string & word, const History & history) const override; \
  };

/**
 *  Derives the class header for the rule with superclass Rule.
 */ 
#define DERIVE_BASE(classname) DERIVE_RULE(classname, Rule)

#endif 

