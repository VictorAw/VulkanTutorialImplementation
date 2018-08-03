#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>

namespace Debug
{
  inline void Log()
  {
    #ifndef NDEBUG
      std::cout << '\n';
    #endif
  }

  inline void Log(std::string const &message)
  {
    #ifndef NDEBUG
      std::cout << message << '\n';
    #endif
  }

  inline void Log(std::string const &level, std::string const &message)
  {
    #ifndef NDEBUG
      std::cout << level << ": " << message << '\n';
    #endif
  }
}

#endif //DEBUG_H
