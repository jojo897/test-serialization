//
// Created by jss on 14/10/2018.
//

#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

template <typename T>
 class Serializable : public boost::noncopyable
{
  public:
  virtual std::string to_json(bool pretty) const = 0;
  virtual boost::property_tree::iptree to_json() const = 0;
};

#endif //!SERIALIZABLE_HPP
