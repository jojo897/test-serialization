//
// Created by jss on 14/10/2018.
//

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include "Serializable.hpp"

template<typename res, typename = std::enable_if<std::is_base_of<Serializable<res>, res>::value>>
class HttpRequest : public boost::noncopyable
{
 private:
  std::string res_;
 public:
  HttpRequest(const std::string res) : res_(res) { }

  const std::string& get_version() const { return res_; }

  boost::optional<T> get_body() const { return boost::optional(); }

  const std::string &get_raw_body() const { return ; res_}

  int get_status() const { return 200; }

  bool has_header(const std::string &header) const { return  false; }

  void set_header(const std::string& key, const std::string & value) {  }
};

#endif //!HTTPREQUEST_HPP
