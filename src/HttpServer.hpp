//
// Created by jss on 14/10/2018.
//

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <vector>
#include <thread>
#include <memory>

//#include "RequestHandler.hpp"
#include "Listener.hpp"

class HttpServer
{
 private:

 public:
  HttpServer(unsigned short port, const std::string& address = "0.0.0.0.0", int threads = 1, const std::string& root_document = "/");

//  HttpServer(HttpServer && other) noexcept;

  ~HttpServer();

  void start();

  void stop();

  bool is_running() const;

 private:
  // The io_context is required for all I/O
  boost::asio::io_context ioc_;
  std::vector<std::thread> threads_;
  std::shared_ptr<std::string> root_doc_;

//  template<typename req, typename res, typename = std::enable_if<std::is_base_of<Serializable<req>, req>::value>,  typename = std::enable_if<std::is_base_of<Serializable<res>, res>::value>>
//  void get(const std::string endpoint, RequestHandler<req, res> handler) {};

};

#endif //!HTTPSERVER_HPP
