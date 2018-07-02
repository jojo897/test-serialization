//
// Created by jss on 01/07/18.
//

#define BOOST_TEST_MAIN

#if !defined( WIN32 )
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MODULE SerializationTestSuite
#include <Response.hpp>
#include <sstream>
#include <iostream>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

std::string map2json (const std::map<std::string, std::string>& map) {
    ptree pt;
    for (auto& entry: map)
        pt.put (entry.first, entry.second);
    std::ostringstream buf;
    write_json (buf, pt, false);
    return buf.str();
}

BOOST_AUTO_TEST_CASE( test_response_serialization )
{
    // Write json.
    ptree pt;
    pt.put ("foo", "bar");
    ostringstream buf;
    write_json (buf, pt, false);
    string json = buf.str();
    cout << "json = '" << json << "'" << endl;
    BOOST_CHECK(json == "{\"foo\":\"bar\"}\n");

    // Read json.
    ptree pt2;
    istringstream is (json);
    read_json (is, pt2);
    string foo = pt2.get<string> ("foo");
    BOOST_CHECK(foo == "bar");
}