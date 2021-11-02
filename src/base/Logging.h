
#ifndef PQ_LOGGING_H_
#define PQ_LOGGING_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>

/**
 *  Logs a string to standard out as well as the logfile. 
 */ 
inline void log(const std::string & s)
{
  std::mutex s_mtx;
  std::lock_guard guard {s_mtx};
  std::string out = s;

  if (s.empty() || s.back() != '\n') out = out + "\n";

  std::cout << out;
}

/**
 *  Returns a string on generic input operators.
 */ 
template<typename ... Ts>
inline std::string log_print(const Ts & ... args)
{
  std::stringstream ss {};
  (ss << ... << args);
  return ss.str();
}

/**
 *  Logs objects to standard error.
 *  @tparam Ts Types compatible with the << operator. 
 */ 
template<typename ... Ts>
inline void log(const Ts & ... args)
{
  std::ostringstream ss {};
  (ss << ... << args);
  log(ss.str());
}

#define U_LOG(...) log(__VA_ARGS__, "\n\n")

#define GET_TAG(severity, file, func, line) "\033[4m", std::this_thread::get_id(), "\033[0m \033[4m", severity, "/", std::filesystem::path{file}.filename(), ":", func, ":", line, ": \033[0m\n"

#define GET_TAGF(file, func, line) GET_TAG("\033[48;5;235;38;5;203m F", file, func, line)
#define GET_TAGE(file, func, line) GET_TAG("\033[48;5;235;38;5;203m E", file, func, line)
#define GET_TAGW(file, func, line) GET_TAG("\033[48;5;235;38;5;221m W", file, func, line)
#define GET_TAGI(file, func, line) GET_TAG("\033[48;5;235;38;5;159m I", file, func, line)
#define GET_TAGD(file, func, line) GET_TAG("\033[48;5;235;38;5;183m D", file, func, line)

#define U_LOGF_FULL(file, func, line, ...) U_LOG(GET_TAGF(file, func, line), __VA_ARGS__); std::abort();
#define U_LOGE_FULL(file, func, line, ...) U_LOG(GET_TAGE(file, func, line), __VA_ARGS__)
#define U_LOGW_FULL(file, func, line, ...) U_LOG(GET_TAGW(file, func, line), __VA_ARGS__)
#define U_LOGI_FULL(file, func, line, ...) U_LOG(GET_TAGI(file, func, line), __VA_ARGS__)

#ifdef DEBUG
#define U_LOGD_FULL(file, func, line, ...) U_LOG(GET_TAGD(file, func, line), __VA_ARGS__)
#else
#define U_LOGD_FULL(file, func, line, ...) (static_cast<void>(0))
#endif

#define U_LOGF(...) U_LOGF_FULL(__FILE__, __func__, __LINE__, __VA_ARGS__) 
#define U_LOGE(...) U_LOGE_FULL(__FILE__, __func__, __LINE__, __VA_ARGS__) 
#define U_LOGW(...) U_LOGW_FULL(__FILE__, __func__, __LINE__, __VA_ARGS__) 
#define U_LOGI(...) U_LOGI_FULL(__FILE__, __func__, __LINE__, __VA_ARGS__)
#define U_LOGD(...) U_LOGD_FULL(__FILE__, __func__, __LINE__, __VA_ARGS__)

#define U_LOGBARE(...) U_LOG("", "", "", "", __VA_ARGS__);

#endif // PQ_LOGGING_H_

