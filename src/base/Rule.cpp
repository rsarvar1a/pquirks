
#include "Error.h"
#include "Rule.h"

const std::map<std::string, const Rule *> & Rules::get_rule_table()
{
  return instance().m_rules;
}

std::ostream & Rule::print_state(std::ostream & out, const History & history) const
{
  return history.state_format(out);
}

void Rules::register_rule(Rule * rule_instance)
{
  if (instance().m_rules.find(rule_instance->name()) != instance().m_rules.end())
  {
    THROW_ERROR(
        "There is already a rule with name '", rule_instance->name(), 
        "'; its description is '", instance().m_rules[rule_instance->name()]->description(), "'.");
  }

  instance().m_rules[rule_instance->name()] = rule_instance;
}

Rules & Rules::instance()
{
  static Rules inst_ {};
  return inst_;
}

