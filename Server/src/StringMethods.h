#ifndef SRC_LIB_STRINGMETHODS_H_
#define SRC_LIB_STRINGMETHODS_H_

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <iostream>
#include <chrono>

//======================================================================
// Various string methods.
//
// These are a bunch of string methods that SHOULD be part of the standard, but aren't. The first set
// change the argument. The _copy methods return a copy of the object.
//
// I got these from a thread on stack exchange. The only change I made was I have all of them return
// the string, so you can do trim(foo).whatever instead of requiring a second line of code.
//
//======================================================================

bool stringStartsWith(const char *str, const char *prefix);

std::ostream & log(std::ostream &stream);

namespace vitac {
    std::string toLower(const std::string &str);
    std::string toUpper(const std::string &str);
};

// trim from start (in place)
std::string & ltrim(std::string &s);

// trim from end (in place)
std::string & rtrim(std::string &s);

// trim from both ends (in place)
std::string & trim(std::string &s);

// trim from start (copying)
// Note this is pass by value, not by reference, which is why it's a copy.
std::string ltrim_copy(std::string s);

// trim from end (copying)
// Note this is pass by value, not by reference, which is why it's a copy.
std::string rtrim_copy(std::string s);

// trim from both ends (copying)
// Note this is pass by value, not by reference, which is why it's a copy.
std::string trim_copy(std::string s);

std::string replaceAll(const std::string &input, const std::string &searchFor, const std::string &replaceWith);

std::string digitsOnly(const std::string &str);

bool stringStartsWith(const std::string &str, const std::string &with);
std::string trimFrom(const std::string &str, const std::string &startingWithAny);


std::string controlStringFromBytes(const char *data, const int length);
std::string intToZeroPaddedString(int value, int digits);
std::string readFileContents(const std::string& path);
std::string getEnvWithDefault(const std::string &key, const std::string &defaultValue);

/**
 * This template method converts a class enum value to its integer representation.
 */
template <typename Enumeration>
auto asInteger(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

//======================================================================
// Time conversion.
//======================================================================
std::string nowAsString();
std::string todayAsString();
std::string nowAsMDY();

std::string systemTimePointToString(const std::chrono::system_clock::time_point &tp, const std::string &delim = std::string(" "), const std::string &suffix = std::string(""));

std::string systemTimePointToStringTwelveHour(const std::chrono::system_clock::time_point &tp, const std::string &delim = std::string(" "));

long timePointToMillis(const std::chrono::time_point<std::chrono::system_clock> &tp);

std::chrono::time_point<std::chrono::system_clock> millisToSystemTimePoint(const long millis);

std::string zuluTime(const std::chrono::system_clock::time_point &tp);

//======================================================================
// Base64 encoding.
//======================================================================
std::string base64Encode(const std::string &);
std::string base64Decode(const std::string &);

#endif /* SRC_LIB_STRINGMETHODS_H_ */
