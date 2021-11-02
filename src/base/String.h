
#ifndef PQ_STRING_H_
#define PQ_STRING_H_

#include <functional>
#include <string>

/**
 *  ##############
 *  Filters
 *  ##############
 */ 

/**
 *  Returns a word consisting only of the letters that match the given filter.
 */ 
std::string filter (const std::string & word, std::function<bool(char)> test);

/**
 *  ##############
 *  Transforms
 *  ##############
 */ 

#endif

