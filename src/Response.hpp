//
// Created by jss on 01/07/18.
//

#ifndef UNTITLED_RESPONSE_H
#define UNTITLED_RESPONSE_H

#include "httplib.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#define CPPHTTPLIB_OPENSSL_SUPPORT

using namespace std;
namespace pt = boost::property_tree;

class Response : protected httplib::Response {
private:
    pt::ptree root;
public:
    Response();
    Response(const string &version, int status, const httplib::Headers &headers, const string &body);

    bool has_header(const string &key) const;
    string get_header_value(const string &key) const;
    void set_header(const string &key, const string &val);

    void set_content(const string &s, size_t n, const string &content_type);
    void set_content(const string &s, const string &content_type);
};


#endif //UNTITLED_RESPONSE_H
