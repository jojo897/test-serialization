//
// Created by jss on 15/10/2018.
//

#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "Session.hpp"
#include "LogManager.hpp"
#include  <boost/asio/ip/tcp.hpp>

// Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener>
{
 private:
  using tcp = boost::asio::ip::tcp;

  tcp::acceptor acceptor_;
  tcp::socket socket_;
  std::shared_ptr<std::string const> doc_root_;

 public:
  Listener(
      boost::asio::io_context& ioc,
      boost::asio::ip::tcp::endpoint endpoint,
      std::shared_ptr<std::string const> const& doc_root)
      : acceptor_(ioc)
      , socket_(ioc)
      , doc_root_(doc_root)
  {
    boost::system::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if(ec)
    {
      LogManager::instance().error(ec, "open");
      return;
    }

    // Allow address reuse
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if(ec)
    {
      LogManager::instance().error(ec, "set_option");
      return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if(ec)
    {
      LogManager::instance().error(ec, "bind");
      return;
    }

    // Start listening for connections
    acceptor_.listen(
        boost::asio::socket_base::max_listen_connections, ec);
    if(ec)
    {
      LogManager::instance().error(ec, "listen");
      return;
    }
  }

  // Start accepting incoming connections
  void run()
  {
    if(! acceptor_.is_open())
      return;
    do_accept();
  }

  void do_accept()
  {
    acceptor_.async_accept(
        socket_,
        std::bind(
            &Listener::on_accept,
            shared_from_this(),
            std::placeholders::_1));
  }

  void on_accept(boost::system::error_code ec)
  {
    if(ec)
    {
      LogManager::instance().error(ec, "accept");
    }
    else
    {
      // Create the session and run it
      std::make_shared<Session>(
          std::move(socket_),
          doc_root_)->run();
    }

    // Accept another connection
    do_accept();
  }
};

#endif //!LISTENER_HPP
