//
// Created by jss on 01/07/18.
//

#include "Response.hpp"


Response::Response()
    : httplib::Response()
{}


Response::Response(const string &version, int status, const httplib::Headers &headers, const string &body)
{
    this->version = version;
    this->status = status;
    this->headers = headers;
    this->body = body;
}


bool Response::has_header(const string &key) const {
    return httplib::Response::has_header(key.c_str());
}

string Response::get_header_value(const string &key) const {
    return httplib::Response::get_header_value(key.c_str());
}

void Response::set_header(const string &key, const string &val) {
    httplib::Response::set_header(key.c_str(), val.c_str());
}

void Response::set_content(const string &s, size_t n, const string &content_type) {
    httplib::Response::set_content(s.c_str(), n, content_type.c_str());
}

void Response::set_content(const string &s, const string &content_type) {
    httplib::Response::set_content(s.c_str(), content_type.c_str());
}