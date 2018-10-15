//
// Created by jss on 14/10/2018.
//

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "Serializable.hpp"
#include "MimeType.hpp"

#include <type_traits>
#include <boost/beast/version.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

//template<typename req, typename res, typename = std::enable_if<std::is_base_of<Serializable<req>, req>::value>,  typename = std::enable_if<std::is_base_of<Serializable<res>, res>::value>>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

// This function produces an HTTP response for the given
// request. The type of the response object depends on the
// contents of the request, so the interface requires the
// caller to pass a generic lambda for receiving the response.
class RequestHandler
{
 private:
  boost::beast::string_view doc_root_;

 public:
  RequestHandler(boost::beast::string_view doc_root)
  : doc_root_(doc_root) { }

  template<class Body, class Allocator, class Send>
  void handle(http::request<Body, http::basic_fields<Allocator>> &&req, Send &&send)
  {
    // Returns a bad request response
    auto const bad_request =
        [&req](boost::beast::string_view why)
        {
          http::response<http::string_body> res{http::status::bad_request, req.version()};
          res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
          res.set(http::field::content_type, "text/html");
          res.keep_alive(req.keep_alive());
          res.body() = why.to_string();
          res.prepare_payload();
          return res;
        };

    // Returns a not found response
    auto const not_found =
        [&req](boost::beast::string_view target)
        {
          http::response<http::string_body> res{http::status::not_found, req.version()};
          res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
          res.set(http::field::content_type, "text/html");
          res.keep_alive(req.keep_alive());
          res.body() = "The resource '" + target.to_string() + "' was not found.";
          res.prepare_payload();
          return res;
        };

    // Returns a server error response
    auto const server_error =
        [&req](boost::beast::string_view what)
        {
          http::response<http::string_body> res{http::status::internal_server_error, req.version()};
          res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
          res.set(http::field::content_type, "text/html");
          res.keep_alive(req.keep_alive());
          res.body() = "An error occurred: '" + what.to_string() + "'";
          res.prepare_payload();
          return res;
        };

    // Make sure we can handle the method
    if( req.method() != http::verb::get &&
        req.method() != http::verb::head)
      return send(bad_request("Unknown HTTP-method"));

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != boost::beast::string_view::npos)
      return send(bad_request("Illegal request-target"));

    // Build the path to the requested file
    std::string path = path_cat(doc_root_, req.target());
    if(req.target().back() == '/')
      path.append("index.html");

    // Attempt to open the file
    boost::beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), boost::beast::file_mode::scan, ec);

    // Handle the case where the file doesn't exist
    if(ec == boost::system::errc::no_such_file_or_directory)
      return send(not_found(req.target()));

    // Handle an unknown error
    if(ec)
      return send(server_error(ec.message()));

    // Cache the size since we need it after the move
    auto const size = body.size();

    // Respond to HEAD request
    if(req.method() == http::verb::head)
    {
      http::response<http::empty_body> res{http::status::ok, req.version()};
      res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
      res.set(http::field::content_type, MimeType()(path));
      res.content_length(size);
      res.keep_alive(req.keep_alive());
      return send(std::move(res));
    }

    // Respond to GET request
    http::response<http::file_body> res{
        std::piecewise_construct,
        std::make_tuple(std::move(body)),
        std::make_tuple(http::status::ok, req.version())};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, MimeType()(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
  }

  // Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
  std::string path_cat(boost::beast::string_view base, boost::beast::string_view path)
  {
    if(base.empty())
      return path.to_string();
    std::string result = base.to_string();
#if BOOST_MSVC
    char constexpr path_separator = '\\';
    if(result.back() == path_separator)
      result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for(auto& c : result)
      if(c == '/')
        c = path_separator;
#else
    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
  }
};

#endif //!REQUESTHANDLER_HPP
