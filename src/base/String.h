
#ifndef PQ_STRING_H_
#define PQ_STRING_H_

#include <functional>
#include <string>

/**
 *  Returns a word consisting only of the letters that match the given filter.
 */ 
std::string charwise_filter (const std::string & word, std::function<bool(char)> test);

/**
 *  Returns a word where each letter is mapped to its image (which is possibly more 
 *  or less than one character) in the given map function.
 */ 
std::string charwise_transform (const std::string & word, std::function<std::string(char)> map);

/**
 *  Counts the occurences of the given character in the word.
 */ 
unsigned count (const std::string & word, char ch);

/**
 *  Determines if the word is found in the dictionary supplied in the data path.
 */ 
bool in_dictionary (const std::string & word);

/**
 *  Joins the words together with the given inner delimiter.
 */ 
std::string join (const std::vector<std::string> & words, const std::string & inner);

/**
 *  Sugar for charwise_transform(word, [](char c){ return to_lower(c); }).
 */ 
std::string lower (const std::string & word);

/**
 *  Splits the word on the given delimiter.
 */ 
std::vector<std::string> split (const std::string & word, const std::string & delim);

/**
 *  Returns the sum of values of the letters in the trimmed, lowercase,
 *  alphabetically-filtered copy of the given word, where a = 1, b = 2,
 *  ..., z = 26.
 */ 
unsigned sum_a1z26 (const std::string & word);

/**
 *  Sugar for charwise_filter(word, [](char c){ return ! is_space(c); }).
 */ 
std::string trim (const std::string & word);

/**
 *  Sugar for charwise_transform(word, [](char c){ return to_upper(c); }).
 */ 
std::string upper (const std::string & word);

/**
 *  Built-in charwisers.
 */ 

/**
 *  Built-in predicate that determines whether the character is alphanumeric.
 */ 
const std::function<bool(char)> is_alnum = [](char c)
{ 
  return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); 
};

/**
 *  Built-in predicate that determines whether the character is a digit.
 */ 
const std::function<bool(char)> is_digit = [](char c)
{ 
  return ('0' <= c && c <= '9'); 
};

/**
 *  Built-in predicate that determines whether the character is a lowercase letter.
 */ 
const std::function<bool(char)> is_lower = [](char c)
{ 
  return ('a' <= c && c <= 'z'); 
};

/**
 *  Built-in predicate that determines whether the character is a form of whitespace.
 */ 
const std::function<bool(char)> is_space = [](char c)
{ 
  return c == ' ' || c == '\n' || c == '\t'; 
};

/**
 *  Built-in predicate that determines whether the character is an uppercase letter.
 */ 
const std::function<bool(char)> is_upper = [](char c)
{ 
  return ('A' <= c && c <= 'Z'); 
};

/**
 *  Built-in mapper that takes every alphabetical character to its ASCII representation 
 *  and every other character to 0.
 */ 
const std::function<std::string(char)> to_ascii = [](char c) 
{ 
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ? std::to_string((int) c) + " " : "0 "; 
};

/**
 *  Built-in mapper that takes every letter to its lowercase.
 */ 
const std::function<std::string(char)> to_lower = [](char c)
{ 
  return std::string {(char) tolower(c)}; 
};

/**
 *  Built-in mapper that takes every letter to its morse representation. 
 */ 
const std::function<std::string(char)> to_morse = [](char c)
{ 
  static std::vector<std::string> morse = 
    {"-_", "_---", "_-_-", "_--", "-", "--_-", "__-", "----", "--", "-___", "_-_", "-_--", "__", 
      "_-", "___", "-__-", "__-_", "-_-", "---", "_", "--_", "---_", "-__", "_--_", "_-__", "__--"}; 
  return ('a' <= c && c <= 'z') ? morse[c - 'a'] + " " : (('A' <= c && c <= 'z') ? morse[c - 'A'] + " " : ""); 
};

/**
 *  Built-in mapper that standardizes all whitespace to space characters.
 */ 
const std::function<std::string(char)> to_space = [](char c)
{ 
  return (c == ' ' || c == '\n' || c == '\t') ? " " : std::string{c}; 
};

/**
 *  Built-in mapper that takes every letter to its uppercase. 
 */ 
const std::function<std::string(char)> to_upper = [](char c)
{ 
  return std::string {(char) toupper(c)}; 
};

#endif

