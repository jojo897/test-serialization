//
// Created by jss on 15/10/2018.
//

#ifndef SESSION_HPP
#define SESSION_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio/bind_executor.hpp>

#include "LogManager.hpp"
#include "RequestHandler.hpp"

namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

// Handles an HTTP server connection
class Session : public std::enable_shared_from_this<Session>
{
  using tcp = boost::asio::ip::tcp;

  // This is the C++11 equivalent of a generic lambda.
  // The function object is used to send an HTTP message.
  struct send_lambda
  {
    Session& self_;

    explicit send_lambda(Session& self)
        : self_(self) { }

    template<bool isRequest, class Body, class Fields>
    void operator()(http::message<isRequest, Body, Fields>&& msg) const
    {
      // The lifetime of the message has to extend
      // for the duration of the async operation so
      // we use a shared_ptr to manage it.
      auto sp = std::make_shared<http::message<isRequest, Body, Fields>>(std::move(msg));

      // Store a type-erased version of the shared
      // pointer in the class to keep it alive.
      self_.res_ = sp;

      // Write the response
      http::async_write(
          self_.socket_,
          *sp,
          boost::asio::bind_executor(
              self_.strand_,
              std::bind(
                  &Session::on_write,
                  self_.shared_from_this(),
                  std::placeholders::_1,
                  std::placeholders::_2,
                  sp->need_eof())));
    }
  };

  tcp::socket socket_;
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::beast::flat_buffer buffer_;
  std::shared_ptr<std::string const> doc_root_;
  http::request<http::string_body> req_;
  std::shared_ptr<void> res_;
  send_lambda lambda_;
  RequestHandler handler_;

 public:
  // Take ownership of the socket
  explicit Session(tcp::socket socket, std::shared_ptr<std::string const> const& doc_root)
      : socket_(std::move(socket)),
        strand_(socket_.get_executor()),
        doc_root_(doc_root),
        lambda_(*this),
        handler_(*doc_root)
  { }

  // Start the asynchronous operation
  void run()
  {
    do_read();
  }

  void do_read()
  {
    // Make the request empty before reading,
    // otherwise the operation behavior is undefined.
    req_ = {};

    // Read a request
    http::async_read(socket_, buffer_, req_,
                     boost::asio::bind_executor(
                         strand_,
                         std::bind(
                             &Session::on_read,
                             shared_from_this(),
                             std::placeholders::_1,
                             std::placeholders::_2)));
  }

  void on_read(
      boost::system::error_code ec,
      std::size_t bytes_transferred)
  {
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if(ec == http::error::end_of_stream)
      return do_close();

    if(ec)
      return LogManager::instance().error(ec, "read");

    // Send the response
    handler_.handle(std::move(req_), lambda_);
  }

  void on_write(
      boost::system::error_code ec,
      std::size_t bytes_transferred,
      bool close)
  {
    boost::ignore_unused(bytes_transferred);

    if(ec)
      return LogManager::instance().error(ec, "write");

    if(close)
    {
      // This means we should close the connection, usually because
      // the response indicated the "Connection: close" semantic.
      return do_close();
    }

    // We're done with the response so delete it
    res_ = nullptr;

    // Read another request
    do_read();
  }

  void do_close()
  {
    // Send a TCP shutdown
    boost::system::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_send, ec);

    // At this point the connection is closed gracefully
  }
};

#endif //!SESSION_HPP
