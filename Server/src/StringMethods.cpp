#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <date/date.h>

#include "StringMethods.h"

using namespace std;
using namespace std::chrono;

/**
 * Does this string start with this prefix?
 */
bool
stringStartsWith(const char *str, const char *prefix)
{
    while(*prefix) {
        if( *prefix++ != *str++ ) {
            return false;
        }
    }

    return true;
}

/**
 * For logging, this just lets us start with a timestamp.
 */
ostream &
log(ostream &stream) {
	system_clock::time_point now = system_clock::now();
	string nowStr = systemTimePointToString(now, " ");

	stream << nowStr << ": ";
	return stream;
}

/**
 * Return an all-lower-case version of this string. The boost method
 * transforms the input string, but I want one that returns a copy with
 * force-lower.
 */
std::string
vitac::toLower(const string &str) {
    string copy = str;
    boost::to_lower(copy);
    return copy;
}

/**
 * Return an all-upper-case version of this string.
 */
std::string
vitac::toUpper(const string &str) {
    string copy = str;
    boost::to_upper(copy);
    return copy;
}

//======================================================================
// Methods for trimming strings.
//======================================================================

// trim from start (in place)
std::string & ltrim(std::string &s) {
    // OMG. Let's break this down. The erase isn't bad. It's going to erase
    // from the beginning until the last character before the end. So it's the find_if and everything
    // inside of it that is tricky.
    //
    // The find-if is going iterate through the entire string, one character at a time, passing the
    // contents of his *iter -> that std::not1() predicate thing. If that ever returns true, then
    // the corresponding iterator is returned, forming the tail + 1 of what to trim.
    //
    // And the predicate is basically "is not a space".
    //
    // In other words, delete from the beginning up to but not including the first non-space.
    //
    // Whew!
    //
    // Oh. And isspace takes and returns an int instead of taking a char and returning a boolean.
    // Otherwise we'd see <char,bool> near the end.
    //
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    return s;
}

// trim from end (in place)
std::string & rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());

    return s;
}

// trim from both ends (in place)
std::string & trim(std::string &s) {
    ltrim(s);
    rtrim(s);

    return s;
}

// trim from start (copying)
// Note this is pass by value, not by reference, which is why it's a copy.
std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
// Note this is pass by value, not by reference, which is why it's a copy.
std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
// Note this is pass by value, not by reference, which is why it's a copy.
std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

/**
 * Returns a new string that replaces all instances of searchFor with replaceWith.
 */
std::string replaceAll(const std::string &input, const std::string &searchFor, const std::string &replaceWith) {
    string retVal = input;
    unsigned long searchLen = searchFor.length();

    for (string::size_type pos = retVal.find(searchFor, 0); pos != string::npos; pos = retVal.find(searchFor, pos)) {
        retVal = retVal.replace(pos, searchLen, replaceWith);
    }

    return retVal;
}

std::string digitsOnly(const std::string &str) {
    string retVal;

    for (char c: str) {
        if (isdigit(c)) {
            retVal += c;
        }
    }

    return retVal;
}

template< typename T >
std::string hexString( T i )
{
  std::stringstream stream;
  stream << "0x"
         << std::setfill ('0') << std::setw(sizeof(T)*2)
         << std::hex << i;
  return stream.str();
}

static const char * LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/**
 * Convert this data of raw bytes into a string, using ^X for control characters, \xx for other
 * non-printable characters, or the character itself for everything else. This is being used to
 * dump control-A codes we might receive, for instance.
 */
std::string controlStringFromBytes(const char *data, const int length) {
    string retVal = "";

    if (data != nullptr) {
        for (int index = 0; index < length; ++index) {
            unsigned char c = static_cast<unsigned char>(data[index]);
            if (c == 00) {
                retVal += "\\00";
            }
            else if (c == '\r') {
                retVal += "\\r";
            }
            else if (c == '\n') {
                retVal += "\\n";
            }
            else if (c <= 26) {
                // This is deliberate.
                retVal += string("^") + LETTERS[c-1];
            }
            else if (c < ' ') {
                retVal += "\\" + hexString(c);
            }
            else if (c < 127) {
                retVal += c;
            }
            else {
                retVal += "\\" + hexString(c);
            }
        }
    }

    return retVal;
}

std::string intToZeroPaddedString(int value, int digits) {
    char buf[10];

    sprintf(buf, "%%0%dd", digits);
    string str = boost::str(boost::format(buf) % value);
    return str;
}

/**
 * Read file contents.
 */
std::string readFileContents(const std::string& path) {

    auto close_file = [](FILE* f){fclose(f);};

    auto holder = std::unique_ptr<FILE, decltype(close_file)>(fopen(path.c_str(), "rb"), close_file);
    if (!holder) {
      return "";
    }

    FILE* f = holder.get();

    // in C++17 following lines can be folded into std::filesystem::file_size invocation
    if (fseek(f, 0, SEEK_END) < 0) {
      return "";
    }

    const long size = ftell(f);
    if (size < 0) {
      return "";
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        return "";
    }

    std::string res;
    res.resize(size);

    // C++17 defines .data() which returns a non-const pointer
    fread(const_cast<char*>(res.data()), 1, size, f);

    return res;
}

/**
 * Get the environement variable listed but return the original if it's not found.
 */
string getEnvWithDefault(const string &key, const string &defaultValue) {
    const char * value = getenv(key.c_str());
    if (value != NULL) {
        return string(value);
    }
    return defaultValue;
}

bool stringStartsWith(const std::string &str, const std::string &with) {
    return str.find(with, 0) == 0;
}

std::string trimFrom(const std::string &str, const std::string &startingWithAny) {
    string retVal = str;
    unsigned long trimFrom = str.find_first_of(startingWithAny);

    if (trimFrom == string::npos) {
        return retVal;
    }

    retVal.erase(trimFrom);
    return retVal;
}


//======================================================================
// Time conversion methods.
//======================================================================

/**
 * We want the current time like 2020-04-01 10:59:32 in UTC.
 */
string
nowAsString() {
	system_clock::time_point now = system_clock::now();
	string nowStr = systemTimePointToString(now, " ", "");
	return nowStr;
}

/**
 * Just the current YMD.
 */
string
todayAsString() {
    date::year_month_day  today = date::year_month_day{date::floor<date::days>(system_clock::now())};
    ostringstream buffer;

    buffer << today;

    return buffer.str();
}

string
nowAsMDY() {
    date::year_month_day  today = date::year_month_day{date::floor<date::days>(system_clock::now())};

    return date::format("%m-%d-%Y", today);
}


/**
 * Convert from this time to a string. Format is 2020-04-01 14:21:11.123456. But
 * The middle space is from delim, and we can append a suffix.
 */
string
systemTimePointToString(const system_clock::time_point &tp, const string &delim, const string &suffix)
{
    string formatStr = string("%Y-%m-%d") + delim + string("%H:%M:%S") + suffix;
    return date::format(formatStr, date::floor<microseconds>(tp) );
}

std::string zuluTime(const std::chrono::system_clock::time_point &tp) {
    return systemTimePointToString(tp, "T", "Z");
}


/**
 * Convert from this time to a string. It dumps as m-d-y h-m-s AM
 */
string
systemTimePointToStringTwelveHour(const system_clock::time_point &tp, const string &delim)
{
    string formatStr = string("%Y-%m-%d") + delim + string("%I:%M:%S %p");
    return date::format(formatStr, date::floor<microseconds>(tp) );
}

long
timePointToMillis(const time_point<system_clock> &tp)
{
	long millis = duration_cast<milliseconds>(tp.time_since_epoch()).count();
	return millis;
}

time_point<system_clock>
millisToSystemTimePoint(const long millis) {
	milliseconds durMillis(millis);
	time_point<system_clock> tp(durMillis);

	return tp;
}

//======================================================================
// Base-64 encoding.
//======================================================================

std::string base64Encode(const std::string &in) {

    std::string out;

    int val=0, valb=-6;
    for (unsigned char c : in) {
        val = (val<<8) + c;
        valb += 8;
        while (valb>=0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb-=6;
        }
    }
    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}

std::string base64Decode(const std::string &in) {

    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val=0, valb=-8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb>=0) {
            out.push_back(char((val>>valb)&0xFF));
            valb-=8;
        }
    }
    return out;
}
