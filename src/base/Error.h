
#ifndef PQ_ERROR_H_
#define PQ_ERROR_H_

#include <sstream>
#include <vector>
#include <stdexcept>

class Error : public std::runtime_error
{
  public:

    /**
     *  Represents an addition to the call stack.
     */ 
    struct CallError
    {
      std::string file;
      std::string func;
      std::string line;
      std::string message;
    };

    /**
     *  Constructs an error.  
     */ 
    template<typename ... Ts>
    Error(const std::string & file, const std::string & func, unsigned line, const Ts & ... args);

    /**
     *  Adds a stack entry to this error.
     */ 
    template<typename ... Ts>
    void append(const std::string & file, const std::string & func, unsigned line, const Ts & ... args);

    /**
     *  Prints this error's full stack trace to the console.
     */ 
    void print() const noexcept;

    /**
     *  Gets the content of the error message.
     */ 
    virtual const char * what() const noexcept override;

  private:

    std::vector<CallError> m_stack;
};

template<typename ... Ts>
Error::Error(const std::string & file, const std::string & func, unsigned line, const Ts & ... args)
  : std::runtime_error {""}
{
  append(file, func, line, args...);
}

template<typename ... Ts>
void Error::append(const std::string & file, const std::string & func, unsigned line, const Ts & ... args)
{
  std::ostringstream ss {};
  (ss << ... << args);

  m_stack.push_back(CallError { file, func, std::to_string(line), ss.str() });
}

#define THROW_ERROR(...)       throw Error { __FILE__, __func__, __LINE__, __VA_ARGS__ }
#define STACK_ERROR(prev, ...) prev.append ( __FILE__, __func__, __LINE__, __VA_ARGS__ ); throw

#endif // UTILITY_ERROR_H_

