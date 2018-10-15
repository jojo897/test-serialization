//
// Created by jss on 15/10/2018.
//

#ifndef LOGMANAGER_HPP
#define LOGMANAGER_HPP

#include <iostream>

class LogManager
{
 public:
  void error(boost::system::error_code ec, char const* what)
  {
    std::cerr << what << ": " << ec.message() << "\n";
  }

  void error(const std::string &str)
  {
    std::cerr << str << std::endl;
  }

  void info(const std::string &str)
  {
    std::cout << str << std::endl;
  }

  static LogManager& instance()
  {
    static LogManager instance_;

    return instance_;
  }

};

#endif //!*LOGMANAGER_HPP
