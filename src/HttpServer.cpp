//
// Created by jss on 14/10/2018.
//

#include <boost/asio/ip/tcp.hpp>
#include "HttpServer.hpp"

using tcp = boost::asio::ip::tcp;

HttpServer::HttpServer(unsigned short port, const std::string& address, int threads, const std::string& root_document)
:  ioc_(threads), threads_(threads), root_doc_(std::make_shared<std::string>(root_document))
{
  // Create and launch a listening port
  std::make_shared<Listener>(
      ioc_,
      tcp::endpoint{boost::asio::ip::make_address(address), port},
      root_doc_)->run();

  // Run the I/O service on the requested number of threads
  for(auto i = threads - 1; i > 0; --i)
    threads_.emplace_back(
        [&]
        {
          ioc_.run();
        });
}

HttpServer::~HttpServer()
{
  if (is_running())
  {
    stop();
  }
}

void HttpServer::start()
{
  ioc_.run();
}


void HttpServer::stop()
{
  ioc_.stop();
}

bool HttpServer::is_running() const
{
  return !ioc_.stopped();
}