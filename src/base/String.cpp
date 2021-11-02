
#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_set>

#include "String.h"

std::string charwise_filter(const std::string & word, std::function<bool (char)> test)
{
  std::string result {};

  for (char c : word)
  {
    if (test(c))
    {
      result.push_back(c);
    }
  }

  return result;
}

std::string charwise_transform(const std::string & word, std::function<std::string (char)> map)
{
  std::stringstream ss {};

  for (char c : word)
  {
    ss << map(c);
  }

  return ss.str();
}

unsigned count(const std::string & word, char ch)
{
  unsigned count = 0;

  for (char c : word)
  {
    if (c == ch)
    {
      count++;
    }
  }

  return count;
}

bool in_dictionary(const std::string & word)
{
  static std::unique_ptr<std::unordered_set<std::string>> dictionary;
  if (! dictionary.get())
  {
    dictionary = std::make_unique<std::unordered_set<std::string>>();

    std::fstream fs {"data/dictionary.txt"};
    std::string line;
    
    while (fs >> line)
    {
      dictionary->insert(trim(lower(line)));  
    }
  }

  return dictionary->find(trim(lower(word))) != dictionary->end(); 
}

std::string join(const std::vector<std::string> & words, const std::string & inner)
{
  if (words.empty())
  {
    return "";
  }

  std::stringstream ss {words[0]};
  for (int i = 1; i < words.size(); i++)
  {
    ss << inner << words[i];
  }

  return ss.str();
}

std::string lower(const std::string & word)
{
  return charwise_transform(word, to_lower);
}

std::vector<std::string> split(const std::string & word, const std::string & delim)
{
  std::vector<std::string> result {};
  std::string s = word;
  std::string token {};
  size_t pos = 0;

  while ((pos = s.find(delim)) != std::string::npos) 
  {
    result.push_back(s.substr(0, pos));
    s.erase(0, pos + delim.length());
  }

  return result;
}

unsigned sum_a1z26(const std::string & word)
{
  std::string intermediate = trim(lower(word));
  unsigned sum = 0;

  for (char c : intermediate)
  {
    sum += (c - 'a' + 1);
  }

  return sum;
}

std::string trim(const std::string & word)
{
  return charwise_filter(word, [](char c){ return c != ' ' && c != '\t' && c != '\n'; });
}

std::string upper(const std::string & word)
{
  return charwise_transform(word, to_upper);
}

