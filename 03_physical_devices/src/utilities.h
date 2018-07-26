#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>

namespace utilities
{
  inline void log()
  {
    #ifndef NDEBUG
      std::cout << '\n';
    #endif
  }

  inline void log(std::string const &message)
  {
    #ifndef NDEBUG
      std::cout << message << '\n';
    #endif
  }

  inline void log(std::string const &level, std::string const &message)
  {
    #ifndef NDEBUG
      std::cout << level << ": " << message << '\n';
    #endif 
  }
}

#endif //UTILITIES_H
